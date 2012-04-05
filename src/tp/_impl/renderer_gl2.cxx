
#include "tp/renderer.h"
#include "tp/module.h"
#include "tp/rtti.h"
#include "tp/primitive.h"

struct tpGLRendererTraits : tpRendererTraits {
};

tpGLRendererTraits mTraits;


class tpRendererGL2x : public tpRenderer {
public:
	TP_TYPE_DECLARE

	virtual void operator ()(tpScene* scene);

	virtual const tpRendererTraits& getTraits() const {
		return mTraits;
	}

	void onPrimitive(tpPrimitive *p);
};

void tpRendererGL2x::operator ()(tpScene *scene)
{
}

void tpRendererGL2x::onPrimitive(tpPrimitive* p)
{
}


TP_TYPE_REGISTER(tpRendererGL2x,tpRenderer,RendererGL2x);
TP_MODULE_REGISTER(RendererGL2x,tpRendererGL2x)
TP_MODULE_USE(RendererGL2x)

