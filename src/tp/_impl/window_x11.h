#ifndef TPRENDERSURFACE_X11_H
#define TPRENDERSURFACE_X11_H

#if defined(TP_USE_X11)

#include <tp/window.h>

#include <X11/Xlib.h>

class tpWindowX11 : public tpWindow {
protected:

	Display *dpy;
	Window win;
	Atom wmDeleteWindow;

    virtual ~tpWindowX11();

public:

	TP_TYPE_DECLARE

    tpWindowX11(tpWindowTraits *traits);

	bool show(bool doShow);
    void doCreate(tpWindowTraits *traits);

	void setCaption(const tpString &caption);

    tpVec2i getSize() const;

    void setClientAreaSize(tpUInt w, tpUInt h);
    tpVec2i getClientAreaSize() const;


	void update();

	tpRawPtr getDisplay() { return dpy; }
	tpRawPtr getWindow() { return reinterpret_cast<tpRawPtr>(win); }

    void setContext(tpRenderContext* context);

	void destroy();
};

#endif

#endif
