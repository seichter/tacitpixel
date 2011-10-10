/*
* Twisted Pair Visualization Engine
*
* Copyright (c) 1999-2009 Hartmut Seichter 
* 
* This library is open source and may be redistributed and/or modified under  
* the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
* any later version. The full license text is available in the LICENSE file 
* included with this distribution, and on the technotecture.com website.
*
*/

#include "tpGLRenderSurfaceWin32.h"
#include "tpGLRenderer.h"

#include <tp/system.h>
#include <tp/camera.h>
#include <tp/module.h>
#include <tp/version.h>

#define WM_CREATED (WM_USER+1)

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	

	HDC hdc;
	int wmId, wmEvent;
	PAINTSTRUCT ps;

	tpUByte mouse_button(0);
	tpUByte mouse_state(0);


	tpGLRenderSurfaceWin32* rendersurface = 
		(tpGLRenderSurfaceWin32*)GetWindowLong(hWnd, GWL_USERDATA);

	//tpLogNotify("%s: 0x%x",__FUNCTION__,rendersurface);

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{	
			case IDOK:
				SendMessage (hWnd, WM_CLOSE, 0, 0);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		{
			CREATESTRUCT *cs = (CREATESTRUCT*)lParam;
			rendersurface = (tpGLRenderSurfaceWin32*)cs->lpCreateParams;
			SetWindowLong(hWnd,GWL_USERDATA,(long)rendersurface);
			PostMessage(hWnd,WM_CREATED,0,0);
			return 0;
		};

		// Should Render Here!
		break;

	case WM_ERASEBKGND:
	case WM_MOVE:
		rendersurface->frame();
		break;

	case WM_CLEAR:
		break;

	case WM_PAINT:
		//rendersurface->frame();
		//PaintProc(hWnd);
		break;

	case WM_DESTROY:
		//CommandBar_Destroy(g_hwndCB);
		tpLogNotify("%s destroy",__FUNCTION__);
		if (rendersurface) {
			tpLogNotify("%s destroy dispatched",__FUNCTION__);
			rendersurface->setDone(true);
		}
		//PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		// Notify shell of our activate message
		//SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
		break;
	case WM_SETTINGCHANGE:
		//SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		mouse_state = 1;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		if (!mouse_state) mouse_state = 2;
	case WM_MOUSEMOVE:
	{
		if (wParam & MK_LBUTTON) mouse_button = 1;
		if (wParam & MK_RBUTTON) mouse_button = 2;
		if (wParam & MK_MBUTTON) mouse_button = 3;

		tpInt mouse_x = LOWORD(lParam);
		tpInt mouse_y = HIWORD(lParam);

		if (rendersurface->getMouseAdapter()) 
		{
			rendersurface->getMouseAdapter()->onMouseMotion(mouse_x,mouse_y,mouse_button,mouse_state);
		}
		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;

}


tpGLRenderSurfaceWin32::tpGLRenderSurfaceWin32( tpRenderSurfaceTraits* traits )
:    _handle(0),
	_dc(0),
	_context(0),
	_instance(0),
	tpRenderSurface( traits )	
{
	tpLogMessage("%s",__FUNCTION__);
	doCreate(traits);
}

static int __tempwnd_id = 0;

void tpGLRenderSurfaceWin32::doCreate( tpRenderSurfaceTraits* traits )
{

	tpGL::get().load(tpGL::TP_GL_PREFETCH);


	_classname = tpStringFormat("tpGLRenderSurface_%d",__tempwnd_id++);

	_instance = GetModuleHandle(NULL);

	WNDCLASS wndclass;

#if defined(WINCE)
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= (WNDPROC) WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= _instance;
	wndclass.hIcon			= 0; // LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName	= 0;
	wndclass.lpszClassName	= _classname.mb_str();	
#else 
	wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wndclass.lpfnWndProc   = (WNDPROC) WndProc;//DefWindowProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = _instance;
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndclass.lpszMenuName  = 0;                            
	wndclass.lpszClassName = _classname.c_str();
#endif

	if (!RegisterClass(&wndclass))
	{
		tpLogError("%s could not register class",__FUNCTION__);
		return;
	}

	DWORD windowstyle = 
#if defined(_WIN32_WCE)
		WS_VISIBLE;
#else
		WS_OVERLAPPEDWINDOW;
#endif

#if !defined(_WIN32_WCE)
	if (!(_handle = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		_classname.c_str(),
		(traits) ? traits->getTitle().c_str() : _classname.c_str(),
		windowstyle,//WS_BORDER | WS_CAPTION | WS_POPUP, //WS_POPUP is good for fullscreen
		(traits) ? traits->getPosition()[0] : CW_USEDEFAULT,
		(traits) ? traits->getPosition()[1] : CW_USEDEFAULT,
		(traits) ? traits->getSize()[0] : CW_USEDEFAULT,
		(traits) ? traits->getSize()[1] : CW_USEDEFAULT,
		0,
		0,
		_instance,
		this
		)
		) // createwindowEX
		) // if
	{
		doKill();
		return;
	}
#else

	tpInt width(640),height(480);
	tpSystem::get()->getDisplaySize(&width,&height);

	_handle = CreateWindowEx(
		WS_EX_CAPTIONOKBTN,
		_classname.mb_str(), 
		(traits) ? traits->getTitle().mb_str() : _classname.mb_str(), 
		WS_VISIBLE,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		NULL, 
		NULL, 
		_instance, 
		this);

	if (!_handle)
	{	
		tpLogError("%s - could not create window",__FUNCTION__);
		doKill();
		return;
	}

	ShowWindow(_handle, SW_SHOWNORMAL);

#endif

	if (!(_dc = GetDC(_handle)))
	{
		tpLogError("%s could not create a DC",__FUNCTION__);
		doKill();
		return;
	}

	if (tpGL::get().hasEGL())
	{

	/* EGL aliases */
	#define EGL_FALSE			0
	#define EGL_TRUE			1

	/* Out-of-band handle values */
	/* These values may vary depending on semantics of native concepts */
	#define EGL_DEFAULT_DISPLAY		((void *)0)
	#define EGL_NO_CONTEXT			((EGLContext)0)
	#define EGL_NO_DISPLAY			((EGLDisplay)0)
	#define EGL_NO_SURFACE			((EGLSurface)0)

	/* Config attributes */
	#define EGL_BUFFER_SIZE			0x3020
	#define EGL_ALPHA_SIZE			0x3021
	#define EGL_BLUE_SIZE			0x3022
	#define EGL_GREEN_SIZE			0x3023
	#define EGL_RED_SIZE			0x3024
	#define EGL_DEPTH_SIZE			0x3025
	#define EGL_STENCIL_SIZE		0x3026
	#define EGL_CONFIG_CAVEAT		0x3027
	#define EGL_CONFIG_ID			0x3028
	#define EGL_LEVEL			0x3029
	#define EGL_MAX_PBUFFER_HEIGHT		0x302A
	#define EGL_MAX_PBUFFER_PIXELS		0x302B
	#define EGL_MAX_PBUFFER_WIDTH		0x302C
	#define EGL_NATIVE_RENDERABLE		0x302D
	#define EGL_NATIVE_VISUAL_ID		0x302E
	#define EGL_NATIVE_VISUAL_TYPE		0x302F
	#define EGL_PRESERVED_RESOURCES		0x3030
	#define EGL_SAMPLES			0x3031
	#define EGL_SAMPLE_BUFFERS		0x3032
	#define EGL_SURFACE_TYPE		0x3033
	#define EGL_TRANSPARENT_TYPE		0x3034
	#define EGL_TRANSPARENT_BLUE_VALUE	0x3035
	#define EGL_TRANSPARENT_GREEN_VALUE	0x3036
	#define EGL_TRANSPARENT_RED_VALUE	0x3037
	#define EGL_NONE			0x3038	/* Attrib list terminator */
	#define EGL_BIND_TO_TEXTURE_RGB		0x3039
	#define EGL_BIND_TO_TEXTURE_RGBA	0x303A
	#define EGL_MIN_SWAP_INTERVAL		0x303B
	#define EGL_MAX_SWAP_INTERVAL		0x303C
	#define EGL_LUMINANCE_SIZE		0x303D
	#define EGL_ALPHA_MASK_SIZE		0x303E
	#define EGL_COLOR_BUFFER_TYPE		0x303F
	#define EGL_RENDERABLE_TYPE		0x3040
	#define EGL_CONTEXT_CLIENT_VERSION 0x3098
	
	/* Config attribute mask bits */
	#define EGL_PBUFFER_BIT			0x0001	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_PIXMAP_BIT			0x0002	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_WINDOW_BIT			0x0004	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_VG_COLORSPACE_LINEAR_BIT	0x0020	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_VG_ALPHA_FORMAT_PRE_BIT	0x0040	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_MULTISAMPLE_RESOLVE_BOX_BIT 0x0200	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_SWAP_BEHAVIOR_PRESERVED_BIT 0x0400	/* EGL_SURFACE_TYPE mask bits */

	#define EGL_OPENGL_ES_BIT		0x0001	/* EGL_RENDERABLE_TYPE mask bits */
	#define EGL_OPENVG_BIT			0x0002	/* EGL_RENDERABLE_TYPE mask bits */
	#define EGL_OPENGL_ES2_BIT		0x0004	/* EGL_RENDERABLE_TYPE mask bits */
	#define EGL_OPENGL_BIT			0x0008	/* EGL_RENDERABLE_TYPE mask bits */

	/* QueryString targets */
	#define EGL_VENDOR			0x3053
	#define EGL_VERSION			0x3054
	#define EGL_EXTENSIONS			0x3055
	#define EGL_CLIENT_APIS			0x308D


	static EGLint const attribute_list[] = {

		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, // OPENGL_ES 1.x == 1 2.0 == 4
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT /*| EGL_PBUFFER_BIT*/,
		EGL_DEPTH_SIZE, 16,
		/*
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		*/
		EGL_NONE
	};

	/* get an EGL display connection */
	display = tpGL::eglGetDisplay((NativeDisplayType)EGL_DEFAULT_DISPLAY);

	EGLint glMajor(0),glMinor(0);

	/* initialize the EGL display connection */
	if (!tpGL::eglInitialize(display, &glMajor, &glMinor))
	{
		tpLogError("%s - eglInitialize failed",__FUNCTION__);
	} else {

		tpLogNotify("%s - EGL got implementation %d.%d\n\tVendor: %s, Version: %s\n\tExtensions:%s\n\tClient APIs:%s"
			,__FUNCTION__,glMajor,glMinor,
			tpGL::eglQueryString(display,EGL_VENDOR),tpGL::eglQueryString(display,EGL_VERSION),
			tpGL::eglQueryString(display,EGL_EXTENSIONS),
			tpGL::eglQueryString(display,EGL_CLIENT_APIS)
		);
	}


	EGLint num_config = 1;

	//if (!tpGL::eglChooseConfig(display, attribute_list, NULL, 0, &num_config))
	//{
	//	tpLogError("%s - eglChooseConfig failed",__FUNCTION__);
	//}
	//config = (EGLConfig*)malloc(num_config * sizeof(EGLConfig));

	EGLConfig config = 0;
	/* get an appropriate EGL frame buffer configuration */
	if (!tpGL::eglChooseConfig(display, attribute_list, &config, num_config, &num_config))
	{
		tpLogError("%s - eglChooseConfig failed",__FUNCTION__);
	} else
	{
		tpLogNotify("%s - eglChooseConfig returned %d configuration(s)",__FUNCTION__,num_config);

		EGLBoolean status;
		int attribute, *value;
		//status = tpGL::eglGetConfigAttrib(display, config, attribute, value);
	}


	/* create an EGL window surface */
	/* This needs to be adapted for ES 1.x */

	EGLint ContextAttributes[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE, EGL_NONE 
	};

	surface = tpGL::eglCreateWindowSurface(display, config, _handle, NULL);
	if (surface == 0)
	{
		tpLogError("%s - eglCreateWindowSurface failed (%s)",__FUNCTION__,tpGL::GetString(tpGL::eglGetError()));
	} else {

		tpLogMessage("%s - eglCreateWindowSurface successful",__FUNCTION__);

	}

	/* create an EGL rendering context */
	context = tpGL::eglCreateContext(display, config, EGL_NO_CONTEXT,(const EGLint*)&ContextAttributes);
	if (EGL_NO_CONTEXT == context)
	{
		tpLogError("%s - eglCreateContext failed (%s)",__FUNCTION__,tpGL::GetString(tpGL::eglGetError()));
	
	} else {

		tpLogMessage("%s - eglCreateContext successful",__FUNCTION__);

		GLboolean res = tpGL::eglMakeCurrent(display,surface,surface,context);
		tpLogMessage("%s - eglMakeCurrent for attaching %d",__FUNCTION__,res);

#define EGL_HEIGHT 0x3056
#define EGL_WIDTH 0x3057
		tpInt w(0),h(0);
		tpGL::eglQuerySurface(display,surface,EGL_WIDTH,&w);
		tpGL::eglQuerySurface(display,surface,EGL_HEIGHT,&h);
		tpLogMessage("%s - actual surface area %dx%d",__FUNCTION__,w,h);

		//getCamera()->setViewport(new tpViewport(0,0,w,h));
	}

	//////////////////////////////////////////////////////////////////////////
	// non-EGL version ahead
	//////////////////////////////////////////////////////////////////////////
	} else {

		tpLogMessage("%s - no EGL",__FUNCTION__);

#if !defined(_WIN32_WCE)

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

	int visual_id = ChoosePixelFormat(_dc, &pfd); 

	if (!SetPixelFormat(_dc, visual_id, &pfd))
	{
		tpLogError("%s pixel format not supported",__FUNCTION__);

		doKill();
		return;
	}

	if (0 == tpGL::wglCreateContext || !(_context = tpGL::wglCreateContext(_dc)))
	{
		tpLogError("%s could not create GL context",__FUNCTION__);
		doKill();
		getchar();
		return;
	}

#endif // _WIN32_WCE

	} // hasEGL

	this->makeCurrent();

	tpLogNotify("%s - OpenGL driver\n\t%s %s %s\n%s",__FUNCTION__,
		tpGL::GetString(tpGL::VENDOR),
		tpGL::GetString(tpGL::RENDERER),
		tpGL::GetString(tpGL::VERSION),
		tpGL::GetString(tpGL::EXTENSIONS));

	//tpLogNotify("%s - EGL driver\n\t%s %s %s",__FUNCTION__,
	//	tpGL::eglGetString(EGL_VENDOR),tpGL::eglGetString(EGL_VERSION),tpGL::eglGetString(EGL_CLIENT_APIS)
	//	);


	tpGL::get().load(tpGL::TP_GL_FULL);
}


tpGLRenderSurfaceWin32::~tpGLRenderSurfaceWin32()
{
	tpLogNotify("%s destroy window",__FUNCTION__);
	doKill();
}

void tpGLRenderSurfaceWin32::doKill()
{
	if (_context) {
		_context = 0;
	}

	if (_dc)
	{
		ReleaseDC(_handle, _dc);
		_dc = 0;
	}

	if (_handle)
	{
		DestroyWindow(_handle);
		_handle = 0;
	}

	UnregisterClassA(_classname.c_str(), _instance);
	_instance = 0;
}

bool tpGLRenderSurfaceWin32::show(bool doShow) {

	ShowWindow(_handle, doShow ? SW_SHOW : SW_HIDE );
	UpdateWindow(_handle);


#if 0
	MSG msg;

	bool done(false);

	while (!done) {
		if (PeekMessage(&msg,0,0,0,PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				done = true;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			//Update Here
		}
	}

#endif

	return true;

}

bool tpGLRenderSurfaceWin32::makeCurrent()
{
	if (tpGL::eglMakeCurrent) {
		return tpGL::eglMakeCurrent(display, surface, surface, context) == TRUE ? true : false;
	} else if (tpGL::MakeCurrent) {
		return tpGL::MakeCurrent(_dc, _context) == TRUE ? true : false;
	}
	return false;
}

bool tpGLRenderSurfaceWin32::swapBuffers()
{
	if (tpGL::eglSwapBuffers) 
	{
		return tpGL::eglSwapBuffers(display, surface);
	} else if (tpGL::SwapBuffers) {
		return tpGL::SwapBuffers(_dc);
	}

	return true;
}


tpRenderer* tpGLRenderSurfaceWin32::createDefaultRenderer() const
{
	return new tpGLRenderer();
}

void tpGLRenderSurfaceWin32::setCaption(const tpString& caption)
{
	SetWindowText(this->_handle,caption.c_str());
}


tpString tpGLRenderSurfaceWin32::getString( tpUInt glenum )
{
	makeCurrent();
	tpString result;
	result.set(reinterpret_cast<const tpChar*>(tpGL::GetString(glenum)));
	return result;
}

tpVoid tpGLRenderSurfaceWin32::frame()
{
	MSG msg;

	if (PeekMessage(&msg, _handle, 0, 0, PM_NOREMOVE)) 
	{
		if (GetMessage(&msg, _handle, 0, 0)) 
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}

	tpRenderSurface::frame();
}


//////////////////////////////////////////////////////////////////////////

class tpRenderSurfaceFactoryWin32 : public tpRenderSurfaceFactory {
public:

	TP_TYPE_DECLARE;

	tpRenderSurfaceFactoryWin32() 
	{
		tpLogNotify("%s OpenGL support on",tpGetVersionString());
	}

	~tpRenderSurfaceFactoryWin32()
	{
		tpLogNotify("%s OpenGL support off",tpGetVersionString());
	}

	tpRenderSurface* create( tpRenderSurfaceTraits* traits )
	{
		return new tpGLRenderSurfaceWin32( traits );
	}
};

TP_TYPE_REGISTER(tpRenderSurfaceFactoryWin32,tpRenderSurfaceFactory,RenderSurfaceFactoryWin32);
TP_TYPE_REGISTER(tpGLRenderSurfaceWin32,tpRenderSurface,GLRenderSurfaceWin32);

TP_MODULE_REGISTER(win32surface,tpRenderSurfaceFactoryWin32);
