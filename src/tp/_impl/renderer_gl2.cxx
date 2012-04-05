
#include "tp/renderer.h"
#include "tp/module.h"
#include "tp/rtti.h"
#include "tp/primitive.h"
#include "tp/scene.h"

class tpPrimitiveRenderNode : public tpRenderNode {

    void
    operator ()(tpNode* node,tpRenderer* renderer)
    {
        tpPrimitive* p = static_cast<tpPrimitive*>(node);
    }

};

class tpRendererGL2x : public tpRenderer {
public:
	TP_TYPE_DECLARE

    virtual void render(tpScene* scene);

    virtual tpUInt getTraits() const {

        return (tpRenderer::kOpenGL2x | tpRenderer::kOpenGLES2);
	}

	void onPrimitive(tpPrimitive *p);
};

void tpRendererGL2x::render(tpScene *scene)
{
}

void tpRendererGL2x::onPrimitive(tpPrimitive* p)
{
}


TP_TYPE_REGISTER(tpRendererGL2x,tpRenderer,RendererGL2x);
TP_MODULE_REGISTER(RendererGL2x,tpRendererGL2x)
TP_MODULE_USE(RendererGL2x)

