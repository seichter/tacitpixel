#ifndef TPRENDERCONTEXT_H
#define TPRENDERCONTEXT_H

#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/rtti.h>
#include <tp/string.h>

class tpRenderTarget;
class tpRenderContext;

struct TP_API tpRenderContextCallback {

    virtual tpUShort chooseContextAPI(const tpRenderContext& context,
                                      const tpArray<tpUShort>& mRenderAPIs) = 0;

};


class TP_API tpRenderContextFactory : public tpReferenced {
protected:
    friend class tpRenderContext;
public:
    TP_TYPE_DECLARE
    virtual tpRenderContext* create( const tpString& settings ) = 0;
};


class TP_API tpRenderContext : public tpReferenced  {
public:

	TP_TYPE_DECLARE

	enum {
		kWaitNone = 0,
		kWaitGL,
		kWaitUI
	};

    enum {
        kRenderAPI_OpenGL1,
        kRenderAPI_OpenGL2,
        kRenderAPI_OpenGL3,
        kRenderAPI_OpenGL4,
        kRenderAPI_OpenGL_ES1,
        kRenderAPI_OpenGL_ES2,
        kRenderAPI_OpenGL_ES3,
        kRenderAPI_User = 0xFF
    };


	tpRenderContext();

    virtual bool bind(tpRenderTarget* target) = 0;
	virtual void destroy() = 0;

	virtual void wait(tpUInt e = kWaitNone) {}
	virtual bool makeCurrent() = 0;
	virtual bool swapBuffers() = 0;

    virtual tpUInt getRendererTraits() const = 0;
    virtual tpUInt getVisualId() const { return 0; }

	virtual tpString getString(const tpUInt& e);

	const tpString& getVersion() const { return mVersion; }
	const tpString& getVendor() const { return mVendor; }
	const tpString& getExtensions() const { return mExtensions; }
	const tpString& getRenderer() const { return mRenderer; }
	const tpString& getName() const { return mName; }

	void getExtensions(tpStringArray& e) const;

    virtual void* getProcAddress(const char*);


	/**
	  * request a module that implements a render context
	  */
	static tpRenderContext* create(const tpString& configuration = tpString());

protected:

	void reportToConsole();

	tpString mVendor;
	tpString mVersion;
	tpString mExtensions;
	tpString mRenderer;

	tpString mName;



	virtual ~tpRenderContext();
};


#endif
