
#include "rendersurface_win32.h"
#include "rendercontext_wgl.h"

#include "tp/log.h"
#include "tp/module.h"
#include "tp/version.h"


#define WM_CREATED (WM_USER+1)


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	

    HDC hdc;
    int wmId, wmEvent;
	PAINTSTRUCT ps;

	tpUByte mouse_button(0);
	tpUByte mouse_state(0);

	tpRenderSurfaceWin32* rendersurface = (tpRenderSurfaceWin32*)GetWindowLongPtr(hWnd,GWL_USERDATA);

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
			rendersurface = (tpRenderSurfaceWin32*)cs->lpCreateParams;
			SetWindowLongPtr(hWnd,GWL_USERDATA,(LONG_PTR)rendersurface);
			PostMessage(hWnd,WM_CREATED,0,0);
			return 0;
		};

		// Should Render Here!
		break;

	case WM_ERASEBKGND:
	case WM_MOVE:
		//rendersurface->frame();
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
		PostQuitMessage(0);
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

			//if (rendersurface->getCallback()) 
			//{
			//	rendersurface->getCallback()->onMouseMotion(mouse_x,mouse_y,mouse_button,mouse_state);
			//}
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;

}

tpRenderSurfaceWin32::tpRenderSurfaceWin32( tpRenderSurfaceTraits* traits )
	: tpRenderSurface( traits )
	,_handle(0)
	,_instance(0)
{
	tpLogMessage("%s",__FUNCTION__);
	doCreate(traits);
}

static int __tempwnd_id = 0;

void tpRenderSurfaceWin32::doCreate( tpRenderSurfaceTraits* traits )
{
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


}

void tpRenderSurfaceWin32::doKill()
{
}


void tpRenderSurfaceWin32::destroy()
{
	if (_handle)
	{
		DestroyWindow(_handle);
		_handle = 0;
	}

	UnregisterClassA(_classname.c_str(), _instance);
	_instance = 0;

	this->mContext = 0;
	doKill();
}

bool tpRenderSurfaceWin32::show( bool doShow )
{
	BOOL res = ShowWindow(_handle, doShow ? SW_SHOW : SW_HIDE );
	UpdateWindow(_handle);
	return (res != 0L);
}

void tpRenderSurfaceWin32::update()
{

	if (this->mDone) return;

	MSG msg;

	if (PeekMessage(&msg, _handle, 0, 0, PM_NOREMOVE)) 
	{
		if (GetMessage(&msg, _handle, 0, 0)) 
		{
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
	}

	if (!isValid()) {
		this->doKill();
	}
}

tpString tpRenderSurfaceWin32::getName() const
{
	return "Windows RenderSurface";
}

void tpRenderSurfaceWin32::setCaption( const tpString& caption)
{
	SetWindowText(this->_handle,caption.c_str());
}

tpString tpRenderSurfaceWin32::getCaption()
{
	tpArray<tpChar> buf; buf.resize(1000);
	GetWindowText(this->_handle,buf.getData(),buf.getSize());
	tpString res; res.set(buf.getData());
	return res;
}

void
tpRenderSurfaceWin32::setContext(tpRenderContext* context)
{
	if (context == 0)
	{
		context = new tpRenderContextWGL();
	}

	tpRenderTarget::setContext(context);
}

tpRawPtr tpRenderSurfaceWin32::getDisplay()
{
	return GetWindowDC(_handle);
}

tpRawPtr tpRenderSurfaceWin32::getWindow()
{
	return _handle;
}


TP_TYPE_REGISTER(tpRenderSurfaceWin32,tpRenderSurface,RenderSurfaceWin32);


class tpRenderSurfaceFactoryWin32 : public tpRenderSurfaceFactory {
public:

	TP_TYPE_DECLARE;

	tpRenderSurfaceFactoryWin32() : tpRenderSurfaceFactory()
	{
		tpLogNotify("%s Windows RenderSurface",tpGetVersionString());
	}

	tpRenderSurface* create( tpRenderSurfaceTraits* traits )
	{
		return new tpRenderSurfaceWin32( traits );
	}
};

TP_TYPE_REGISTER(tpRenderSurfaceFactoryWin32,tpRenderSurfaceFactory,RenderSurfaceFactoryWin32);
TP_MODULE_REGISTER(win32surface,tpRenderSurfaceFactoryWin32);
