#ifndef TPRENDERCONTEXT_H
#define TPRENDERCONTEXT_H

#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/rtti.h>

class tpRenderTarget;

class TP_API tpRenderContext : public tpReferenced  {
public:

	TP_TYPE_DECLARE;

	tpRenderContext();

	virtual bool create(tpRenderTarget* target) = 0;
	virtual void destroy() = 0;

	virtual bool makeCurrent() = 0;
	virtual bool swapBuffers() = 0;

};


#endif
