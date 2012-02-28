#ifndef TPRENDERSURFACE_WIN32_H
#define TPRENDERSURFACE_WIN32_H


#include <tp/window.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


class tpWindowWin32 : public tpWindow {

	HWND _handle;
	HINSTANCE _instance;
	tpString _classname;

public:

	TP_TYPE_DECLARE


	tpWindowWin32( tpWindowTraits* traits );

	void doCreate(tpWindowTraits*);

	void doKill();

	virtual void destroy();

	virtual bool show( bool doShow );

	virtual void update();

	virtual tpString getName() const;

	virtual void setCaption( const tpString& );

	virtual tpString getCaption();

	tpRawPtr getDisplay();
	tpRawPtr getWindow();

    void setContext(tpRenderContext* context);

    tpVec2i getSize() const;
	void setSize(tpInt w, tpInt h);

	void setPosition(tpInt x, tpInt y);
	tpVec2i getPosition() const;
};


#endif
