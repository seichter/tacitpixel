#include "rendercontext_wgl.h"

#if defined(_WIN32)

#include <tp/rendersurface.h>
#include <tp/log.h>

#include <gl/GL.h>


tpRenderContextWGL::tpRenderContextWGL()
	: tpRenderContext()
	, _dc(0)
	, _context(0)
{
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

	return this->makeCurrent();
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


TP_TYPE_REGISTER(tpRenderContextWGL,tpRenderContext,RenderContextWGL);

#endif
