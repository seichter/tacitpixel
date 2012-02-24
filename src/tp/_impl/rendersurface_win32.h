#ifndef TPRENDERSURFACE_WIN32_H
#define TPRENDERSURFACE_WIN32_H


#include <tp/rendersurface.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


class tpRenderSurfaceWin32 : public tpRenderSurface {

	HWND _handle;
	HINSTANCE _instance;
	tpString _classname;

public:

	TP_TYPE_DECLARE


	tpRenderSurfaceWin32( tpRenderSurfaceTraits* traits );

	void doCreate(tpRenderSurfaceTraits*);

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
};


#endif
