#include "rendercontext_wgl.h"

#if defined(_WIN32)

#include <gl/GL.h>
#include <Windows.h>

#include <tp/rendersurface.h>
#include <tp/log.h>
#include <tp/stringtokenizer.h>

// taken from http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
#define WGL_CONTEXT_MAJOR_VERSION_ARB	0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB	0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB		0x2093
#define WGL_CONTEXT_FLAGS_ARB			0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB	0x9126

#define WGL_CONTEXT_DEBUG_BIT_ARB				0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB			0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB	0x00000002


// taken from http://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt
#define WGL_NUMBER_PIXEL_FORMATS_ARB            0x2000
#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_DRAW_TO_BITMAP_ARB                  0x2002
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_NEED_PALETTE_ARB                    0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB             0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB              0x2006
#define WGL_SWAP_METHOD_ARB                     0x2007
#define WGL_NUMBER_OVERLAYS_ARB                 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB                0x2009
#define WGL_TRANSPARENT_ARB                     0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB           0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB         0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB          0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB         0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB         0x203B
#define WGL_SHARE_DEPTH_ARB                     0x200C
#define WGL_SHARE_STENCIL_ARB                   0x200D
#define WGL_SHARE_ACCUM_ARB                     0x200E
#define WGL_SUPPORT_GDI_ARB                     0x200F
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_STEREO_ARB                          0x2012
#define WGL_PIXEL_TYPE_ARB                      0x2013
#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_RED_BITS_ARB                        0x2015
#define WGL_RED_SHIFT_ARB                       0x2016
#define WGL_GREEN_BITS_ARB                      0x2017
#define WGL_GREEN_SHIFT_ARB                     0x2018
#define WGL_BLUE_BITS_ARB                       0x2019
#define WGL_BLUE_SHIFT_ARB                      0x201A
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_ALPHA_SHIFT_ARB                     0x201C
#define WGL_ACCUM_BITS_ARB                      0x201D
#define WGL_ACCUM_RED_BITS_ARB                  0x201E
#define WGL_ACCUM_GREEN_BITS_ARB                0x201F
#define WGL_ACCUM_BLUE_BITS_ARB                 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB                0x2021
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS_ARB                    0x2023
#define WGL_AUX_BUFFERS_ARB                     0x2024

//Accepted as a value in the <piAttribIList> and <pfAttribFList>
//parameter arrays of wglChoosePixelFormatARB, and returned in the
//<piValues> parameter array of wglGetPixelFormatAttribivARB, and the
//<pfValues> parameter array of wglGetPixelFormatAttribfvARB:

#define WGL_NO_ACCELERATION_ARB                 0x2025
#define WGL_GENERIC_ACCELERATION_ARB            0x2026
#define WGL_FULL_ACCELERATION_ARB               0x2027
#define WGL_SWAP_EXCHANGE_ARB                   0x2028
#define WGL_SWAP_COPY_ARB                       0x2029
#define WGL_SWAP_UNDEFINED_ARB                  0x202A
#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_TYPE_COLORINDEX_ARB                 0x202C

#if defined(_WIN32)
#	define __TP_CALL __stdcall
#else
#	define __TP_CALL
#endif

typedef HGLRC (__TP_CALL *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext,const int *attribList);

typedef BOOL  (__TP_CALL *PFNWGLGETPIXELFORMATATTRIBIVARB)(HDC hdc,int iPixelFormat,int iLayerPlane,UINT nAttributes,const int *piAttributes,int *piValues);
typedef BOOL  (__TP_CALL *PFNWGLGETPIXELFORMATATTRIBFVARB)(HDC hdc,int iPixelFormat,int iLayerPlane,UINT nAttributes,const int *piAttributes,int *pfValues);
typedef BOOL  (__TP_CALL *PFNWGLCHOOSEPIXELFORMATARB)(HDC hdc,const int *piAttribIList,const int *pfAttribFList,UINT nMaxFormats,int *piFormats,UINT *nNumFormats);



void 
tpGetGLVersion(tpInt& major,tpInt minor)
{
	tpString version(reinterpret_cast<const tpChar*>(glGetString(GL_VERSION)));
	version = version.beforeFirst(' ');

	tpStringArray vc = tpStringTokenizer::split(version,".");
	major = vc[0].to<tpInt>();
	minor = vc[1].to<tpInt>();
}

tpRenderContextWGL::tpRenderContextWGL()
	: tpRenderContext()
	, _dc(0)
	, _context(0)
{
}

int chooseVisual(HDC hdc)
{
	int PFD(0);

	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	if (DescribePixelFormat(hdc, PFD, pfd.nSize, &pfd) == 0)
		return -1;

	HWND tmpWin = CreateWindowEx(
		WS_EX_APPWINDOW,
		"STATIC",
		"Dummy Tmp Window from NGL",
		WS_POPUP,
		0, 0, 64, 64,
		NULL,
		NULL,
		NULL,
		NULL);
	HDC tmpDC = GetDC(tmpWin);
	SetPixelFormat(tmpDC, PFD, &pfd);
	HGLRC rc = wglCreateContext(tmpDC);
	wglMakeCurrent(tmpDC, rc);


	PFNWGLGETPIXELFORMATATTRIBIVARB wglGetPixelFormatAttribivARB =
		(PFNWGLGETPIXELFORMATATTRIBIVARB)wglGetProcAddress("wglGetPixelFormatAttribivARB");

	PFNWGLCHOOSEPIXELFORMATARB wglChoosePixelFormatARB =
		(PFNWGLCHOOSEPIXELFORMATARB)wglGetProcAddress("wglChoosePixelFormatARB");


	if (wglChoosePixelFormatARB && wglChoosePixelFormatARB) {

		tpArray<int> format;
		format
			.add(WGL_SUPPORT_OPENGL_ARB).add(TRUE)
			.add(WGL_DRAW_TO_BITMAP_ARB).add(TRUE)
			.add(WGL_ACCELERATION_ARB).add(TRUE)
			.add(WGL_DEPTH_BITS_ARB).add(24)
			.add(0);

		tpArray<int> pixformats; pixformats.resize(256);
		UINT numformats(0);

		wglChoosePixelFormatARB(hdc,format.getData(),0,pixformats.getSize(),pixformats.getData(),&numformats);

		tpLogNotify("wglChoosePixelFormatARB returned %d format(s)",numformats);

		for (unsigned int i = 0; i < numformats;++i)
		{
			tpArray<int> query;
			query.add(WGL_COLOR_BITS_ARB)
				.add(WGL_DEPTH_BITS_ARB)
				.add(WGL_RED_BITS_ARB)
				.add(WGL_ALPHA_BITS_ARB);

			tpArray<int> result; result.resize(query.getSize());

			wglGetPixelFormatAttribivARB(hdc,pixformats[i],0,query.getSize(),query.getData(),result.getData());

			// reminder color bits should be *without* the alpha channel
			if ((result[0] == 24) && (result[2] == 8))
			{

				return pixformats[i];
			}

		}

	} else {

		tpLogNotify("wglChoosePixelFormatARB and wglGetPixelFormatAttribivARB not available! (0x%x 0x%x)",
			wglGetPixelFormatAttribivARB,wglChoosePixelFormatARB);
	}

	return -1;
}


bool
tpRenderContextWGL::create(tpRenderTarget *target)
{
	HWND hwnd = reinterpret_cast<HWND>(target->getWindow());

	if (!(_dc = GetDC(hwnd)))
	{
		tpLogError("%s could not create a DC",__FUNCTION__);
		return false;
	}

	
	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                     
		PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL,      
		PFD_TYPE_RGBA,         
		32,                    
		0, 0, 0, 0, 0, 0,      
		0,                     
		0,                     
		0,                     
		0, 0, 0, 0,            
		24,	// < depth
		8,                     
		0,                     
		PFD_MAIN_PLANE,        
		0,                     
		0, 0, 0                
	};
	/*
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	*/

	int visual_id_new = chooseVisual(_dc);
	
	tpLogNotify("visual id %d",visual_id_new);

	int visual_id = ChoosePixelFormat(_dc, &pfd); 

	if (!SetPixelFormat(_dc, visual_id, &pfd))
	{
		tpLogError("%s pixel format not supported",__FUNCTION__);
		return false;
	}

	if (!(_context = wglCreateContext(_dc)))
	{
		tpLogError("%s could not create GL context",__FUNCTION__);
		return false;
	}

	bool res = this->makeCurrent();

	if (res)
	{
		mVersion = this->getString(GL_VERSION);
		mVendor = this->getString(GL_VENDOR);
		mRenderer = this->getString(GL_RENDERER);
		mExtensions = this->getString(GL_EXTENSIONS);

		tpLogNotify("OpenGL %s %s",mVendor.c_str(),mVersion.c_str());

		return true;
	}


	tpInt glmajor(0), glminor(0);

//	tpGetGLVersion(glmajor,glminor);

	tpLogNotify("Initialized an OpenGL %s context",glGetString(GL_VERSION));

	if (res) 
	{
		int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 1,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 
			(PFNWGLCREATECONTEXTATTRIBSARBPROC)this->getProcAddress("wglCreateContextAttribsARB");
		if (wglCreateContextAttribsARB)
		{

			HGLRC newcontext = wglCreateContextAttribsARB(_dc,0,attribs);

			if (newcontext)
			{
				this->destroy();
				_context = newcontext;
				this->makeCurrent();

				tpLogNotify("Re-Initialized an OpenGL %s context",glGetString(GL_VERSION));
			}
		}


	}

	return true;
}

void tpRenderContextWGL::wait( tpUInt e /*= kWaitNone */ )
{
}

bool
tpRenderContextWGL::makeCurrent() 
{
	return (TRUE == wglMakeCurrent(_dc,_context));
}

bool tpRenderContextWGL::swapBuffers()
{
	return (TRUE == SwapBuffers(_dc));
}

void
tpRenderContextWGL::destroy() 
{
	wglMakeCurrent(0,0);
	wglDeleteContext(_context);
}


tpString
tpRenderContextWGL::getString(const tpUInt& e)
{
	const tpChar* str = reinterpret_cast<const tpChar*>(glGetString(e));
	return tpString(str);
}

void*
tpRenderContextWGL::getProcAddress(const char* name)
{
	return wglGetProcAddress(name);
}


TP_TYPE_REGISTER(tpRenderContextWGL,tpRenderContext,RenderContextWGL);

#endif
