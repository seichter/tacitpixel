#ifndef TPRENDERSURFACE_X11_H
#define TPRENDERSURFACE_X11_H

#include <tp/rendersurface.h>

#include <X11/Xlib.h>

class tpRenderSurfaceX11 : public tpRenderSurface {
protected:

	Display *dpy;
	Window win;
	Atom wmDeleteWindow;

public:

	TP_TYPE_DECLARE

	tpRenderSurfaceX11(tpRenderSurfaceTraits *traits);

	bool show(bool doShow);
	void doCreate(tpRenderSurfaceTraits *traits);

	void setCaption(const tpString &caption);
	void update();

	tpRawPtr getDisplay() { return dpy; }
	tpRawPtr getWindow() { return reinterpret_cast<tpRawPtr>(win); }
};


#endif
