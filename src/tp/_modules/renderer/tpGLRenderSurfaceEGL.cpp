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


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	

	HDC hdc;
	int wmId, wmEvent;
	PAINTSTRUCT ps;

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
		//g_hwndCB = CreateRpCommandBar(hWnd);
		//// Initialize the shell activate info structure
		//memset (&s_sai, 0, sizeof (s_sai));
		//s_sai.cbSize = sizeof (s_sai);
		break;
	case WM_CLEAR:
		break;

	case WM_PAINT:
		//PaintProc(hWnd);
		break; 

		//case WM_TIMER:
		//	angle += 1.0f;

		//	x += deltaX;
		//	y += deltaY;
		//	z += deltaZ;

		//	if (x < minX || x > maxX) {
		//		deltaX = -deltaX;
		//		x += 2 * deltaX;
		//	}

		//	if (y < minY || y > maxY) {
		//		deltaY = -deltaY;
		//		y += 2 * deltaY;
		//	}

		//	if (z < minZ || z > maxZ) {
		//		deltaZ = -deltaZ;
		//		z += 2 * deltaZ;
		//	}

		//	InvalidateRect(hWnd, 0, FALSE);
		//	break;

	case WM_DESTROY:
		//CommandBar_Destroy(g_hwndCB);
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		// Notify shell of our activate message
		//SHHandleWMActivate(hWnd, wParam, lParam, &s_sai, FALSE);
		break;
	case WM_SETTINGCHANGE:
		//SHHandleWMSettingChange(hWnd, wParam, lParam, &s_sai);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}



#if defined(WIN32)



#endif

tpGLRenderSurfaceWin32::tpGLRenderSurfaceWin32( tpRenderSurfaceTraits* traits )
:    _handle(0),
	_dc(0),
	_context(0),
	_instance(0),
	tpRenderSurface( traits )	
{
	doCreate();
}

static int __tempwnd_id = 0;

void tpGLRenderSurfaceWin32::doCreate() {

	_classname.format("tpGLRenderSurface_%d",++__tempwnd_id);

	_instance = GetModuleHandle(NULL);

	WNDCLASS wndclass;

#if defined(WINCE)
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= (WNDPROC) WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= _instance;
	wndclass.hIcon			= 0;
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
	wndclass.hCursor       = 0;
	wndclass.hIcon         = 0;
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndclass.lpszMenuName  = 0;                            
	wndclass.lpszClassName = _classname.mb_str();
#endif

	if (!RegisterClass(&wndclass))
		return;

#if !defined(WINCE)
	if (!(_handle = CreateWindowEx( 0,
		_classname.mb_str(),
		_classname.mb_str(),
		WS_BORDER | WS_CAPTION | WS_POPUP, //WS_POPUP is good for fullscreen
		40,
		40,
		640,
		480,
		0,
		0,
		_instance,
		0)))
	{
		doKill();
		return;
	}
#else

	_handle = CreateWindow(
		_classname.mb_str(), 
		_T("Window"), 
		WS_VISIBLE,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		NULL, 
		NULL, 
		_instance, 
		NULL);

	if (!_handle)
	{	
		doKill();
		return;
	}
#endif

	if (!(_dc = GetDC(_handle)))
	{
		tpLogError("%s could not create a DC",__FUNCTION__);
		doKill();
		return;
	}

#if defined(WINCE)
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


	static EGLint const attribute_list[] = {
		EGL_RED_SIZE, 1,
		EGL_GREEN_SIZE, 1,
		EGL_BLUE_SIZE, 1,
		EGL_NONE
	};

	/* get an EGL display connection */
	display = tpGL::eglGetDisplay((NativeDisplayType)EGL_DEFAULT_DISPLAY);

	/* initialize the EGL display connection */
	tpGL::eglInitialize(display, NULL, NULL);

	/* get an appropriate EGL frame buffer configuration */
	tpGL::eglChooseConfig(display, attribute_list, &config, 1, &num_config);

	/* create an EGL rendering context */
	context = tpGL::eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);

	if (_handle) {
		/* create an EGL window surface */
		surface = tpGL::eglCreateWindowSurface(display, config, _handle, NULL);

	}

#else

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
		16,	// < depth
		0,                     
		0,                     
		PFD_MAIN_PLANE,        
		0,                     
		0, 0, 0                
	};

	tpGL gl;

	int visual_id = ChoosePixelFormat(_dc, &pfd); 

	if (!SetPixelFormat(_dc, visual_id, &pfd))
	{
		tpLogError("%s pixel format not supported",__FUNCTION__);

		doKill();
		return;
	}

	if (!(_context = tpGL::CreateContext(_dc)))
	{
		tpLogError("%s could not create GL context",__FUNCTION__);
		doKill();
		return;
	}

#endif

	tpLogNotify("%s created window",__FUNCTION__);
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

	UnregisterClass(_classname.mb_str(), _instance);
	_instance = 0;
}

tpBool tpGLRenderSurfaceWin32::show(tpBool doShow) {

	ShowWindow(_handle, doShow ? SW_SHOW : SW_HIDE );
	UpdateWindow(_handle);


#if 0
	MSG msg;

	tpBool done(false);

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

tpBool tpGLRenderSurfaceWin32::makeCurrent()
{
	bool result(false); 


	if (tpGL::MakeCurrent) {
		result = tpGL::MakeCurrent(_dc, _context) == TRUE ? true : false;
	} else if (tpGL::eglMakeCurrent) {
		result = tpGL::eglMakeCurrent(display, surface, surface, context) == TRUE ? true : false;
	}

	return result;
}

tpBool tpGLRenderSurfaceWin32::swapBuffers()
{
	if (tpGL::eglSwapBuffers) 
	{
		tpGL::eglSwapBuffers(display, surface);
	} else if (tpGL::SwapBuffers) {
		tpGL::SwapBuffers(_dc);
	}

	return true;
}


tpRenderer* tpGLRenderSurfaceWin32::createDefaultRenderer() const
{
	return new tpGLRenderer();
}

tpString tpGLRenderSurfaceWin32::getString( tpUInt glenum )
{
	makeCurrent();
	tpString result;
	result.set(reinterpret_cast<const tpChar*>(tpGL::GetString(glenum)));
	return result;
}

//////////////////////////////////////////////////////////////////////////

class tpRenderSurfaceFactoryWin32 : public tpRenderSurfaceFactory {

	tpRenderSurface* create( tpRenderSurfaceTraits* traits )
	{
		return new tpGLRenderSurfaceWin32( traits );
	}
};

tpRenderSurfaceFactoryLoader<tpRenderSurfaceFactoryWin32> g_rendersurfacefactorywin32;