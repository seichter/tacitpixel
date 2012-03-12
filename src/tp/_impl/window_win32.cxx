
#include "window_win32.h"
#include "rendercontext_wgl.h"

#include "tp/log.h"
#include "tp/module.h"
#include "tp/version.h"


#define WM_CREATED (WM_USER+1)


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	

//    HDC hdc;
    int wmId, wmEvent;
	PAINTSTRUCT ps;

	tpUByte mouse_button(0);
	tpUByte mouse_state(0);

	tpWindowWin32* rendersurface = (tpWindowWin32*)GetWindowLongPtr(hWnd,GWL_USERDATA);

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
			rendersurface = (tpWindowWin32*)cs->lpCreateParams;
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
    case WM_SIZE:
    {
        tpWindowEvent e(rendersurface);
        e.setId(tpWindowEvent::kWindowSize);
        rendersurface->getEventHandler().process(e);
    }
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
        mouse_state = tpWindowEvent::kMouseDown;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
        if (!mouse_state) mouse_state = tpWindowEvent::kMouseUp;
	case WM_MOUSEMOVE:
		{
            if (wParam & MK_LBUTTON) mouse_button = tpWindowEvent::kMouseKeyLeft;
            if (wParam & MK_RBUTTON) mouse_button = tpWindowEvent::kMouseKeyRight;
            if (wParam & MK_MBUTTON) mouse_button = tpWindowEvent::kMouseKeyMiddle;

			tpInt mouse_x = LOWORD(lParam);
			tpInt mouse_y = HIWORD(lParam);

            tpWindowEvent e(rendersurface);
            e.setMousePosition(mouse_x,mouse_y);
            e.setMouseState(mouse_state);
            e.setMouseKey(mouse_button);

            rendersurface->getEventHandler().process(e);

            break;
		}
	case WM_KEYDOWN:
		{
			tpWindowEvent e(rendersurface);
			e.setKeyState(tpWindowEvent::kKeyDown);
			e.setKeyCode((TCHAR)wParam);
			rendersurface->getEventHandler().process(e);
		}
		break;
	case WM_KEYUP:
		{
			tpWindowEvent e(rendersurface);
			e.setKeyState(tpWindowEvent::kKeyUp);
			e.setKeyCode((TCHAR)wParam);
			rendersurface->getEventHandler().process(e);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;

}

tpWindowWin32::tpWindowWin32( tpWindowTraits* traits )
	: tpWindow( traits )
	,_handle(0)
	,_instance(0)
{
	tpLogMessage("%s",__FUNCTION__);
	doCreate(traits);
}

static int __tempwnd_id = 0;

void tpWindowWin32::doCreate( tpWindowTraits* traits )
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
		(traits) ? traits->getPosition()(0) : CW_USEDEFAULT,
		(traits) ? traits->getPosition()(1) : CW_USEDEFAULT,
		(traits) ? traits->getSize()(0) : CW_USEDEFAULT,
		(traits) ? traits->getSize()(1) : CW_USEDEFAULT,
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

void tpWindowWin32::doKill()
{
}

tpVec2i
tpWindowWin32::getSize() const {
    tpVec2i result;
    RECT r;
    GetClientRect(_handle,&r);
    result(0) = r.right-r.left;
    result(1) = r.bottom-r.top;
    return result;
}


void tpWindowWin32::destroy()
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

bool tpWindowWin32::show( bool doShow )
{
	BOOL res = ShowWindow(_handle, doShow ? SW_SHOW : SW_HIDE );
	UpdateWindow(_handle);
	return (res != 0L);
}

void tpWindowWin32::update()
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

tpString tpWindowWin32::getName() const
{
	return "Windows RenderSurface";
}

void tpWindowWin32::setCaption( const tpString& caption)
{
	SetWindowText(this->_handle,caption.c_str());
}

tpString tpWindowWin32::getCaption()
{
	tpArray<tpChar> buf; buf.resize(1000);
	GetWindowText(this->_handle,buf.getData(),buf.getSize());
	tpString res; res.set(buf.getData());
	return res;
}

void
tpWindowWin32::setContext(tpRenderContext* context)
{
	if (context == 0)
	{
		context = new tpRenderContextWGL();
	}

	tpRenderTarget::setContext(context);
}

tpRawPtr tpWindowWin32::getDisplay()
{
	return GetWindowDC(_handle);
}

tpRawPtr tpWindowWin32::getWindow()
{
	return _handle;
}

void tpWindowWin32::setSize( tpInt w, tpInt h )
{
    SetWindowPos(_handle,0,0,0,w,h,SWP_NOMOVE | SWP_NOZORDER);
}

tpVec2i tpWindowWin32::getClientAreaSize() const
{
    RECT rect;
    GetClientRect(_handle,&rect);
    return tpVec2i(rect.right-rect.left,rect.bottom-rect.top);
}

void tpWindowWin32::setClientAreaSize(tpUInt w, tpUInt h)
{
    //AdjustWindowRectEx()
}

void tpWindowWin32::setPosition( tpInt x, tpInt y )
{
	SetWindowPos(_handle,0,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
}

tpVec2i tpWindowWin32::getPosition() const
{
	tpVec2i r(-1,-1);
	RECT w;
	GetWindowRect(_handle,&w);
	r(0) = w.left; r(1) = w.top;
	return r;
}


TP_TYPE_REGISTER(tpWindowWin32,tpWindow,RenderSurfaceWin32);


class tpRenderSurfaceFactoryWin32 : public tpWindowFactory {
public:

	TP_TYPE_DECLARE;

	tpRenderSurfaceFactoryWin32() : tpWindowFactory()
	{
		tpLogNotify("%s Windows RenderSurface",tpGetVersionString());
	}

	tpWindow* create( tpWindowTraits* traits )
	{
		return new tpWindowWin32( traits );
	}
};

TP_TYPE_REGISTER(tpRenderSurfaceFactoryWin32,tpWindowFactory,RenderSurfaceFactoryWin32);
TP_MODULE_REGISTER(win32surface,tpRenderSurfaceFactoryWin32);
