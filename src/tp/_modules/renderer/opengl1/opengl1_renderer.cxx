#include <tp/renderer.h>
#include <tp/node.h>
#include <tp/camera.h>
#include <tp/nodeops.h>
#include <tp/module.h>
#include <tp/primitive.h>
#include <tp/version.h>
#include <tp/log.h>
#include <tp/rendersurface.h>

#if defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#endif

struct tpGLRendererTraits : tpRendererTraits {
};

class tpGLRenderer : public tpRenderer
{
public:

	tpGLRenderer()
		: tpRenderer()
	{
		tpLogNotify("%s OpenGL 1.x renderer",tpGetVersionString());
	}

	TP_TYPE_DECLARE;

	static tpGLRendererTraits mRendererTraits;
	const tpRendererTraits& getTraits() const { return mRendererTraits; }

	void operator()(tpNode* node)
	{
		tpCamera* camera = getActiveCamera();

		glEnable(GL_NORMALIZE);
		glEnable(GL_AUTO_NORMAL);
		glShadeModel(GL_SMOOTH);

		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_COLOR_MATERIAL);


		// hack
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		float amb_light[] = {0.2f, 0.2f, 0.2f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0f);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0f);

		//#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

		tpUInt glclearflag(0);
		if (camera->hasClearFlag(tpCamera::kClearColor))
		{
			glClearColor(camera->getClearColor()[0],camera->getClearColor()[1],camera->getClearColor()[2],camera->getClearColor()[3] );
			glclearflag |= GL_COLOR_BUFFER_BIT;
		}
		if (camera->hasClearFlag(tpCamera::kClearDepth))
		{
			glclearflag |= GL_DEPTH_BUFFER_BIT;
		}

		if (glclearflag) glClear(glclearflag);

		tpNodeMatrixMap nodemap_primitives = tpNodeOps::getNodeMatrixMap(node,tpPrimitive::getTypeInfo());

		for (tpNodeMatrixMap::iterator i = nodemap_primitives.begin();
			 i != nodemap_primitives.end();
			 ++i)
		{
			tpPrimitive* p = static_cast<tpPrimitive*>((*i).getKey());
			(*this)(*p,(*i).getValue());
		}

		GLint error = glGetError();
		if (error) tpLogError("glGetError() 0x%x (%d)",error,error);
	}

	void operator()(const tpPrimitive& prim,const tpMat44r& modelmatrix)
	{
		tpCamera* camera = getActiveCamera();
		tpMat44r view = camera->getViewInverse();
		view *= modelmatrix;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(camera->getProjection().data());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixf(view.data());

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, prim.getVertices().getData());

		if (prim.hasAttribute(tpPrimitive::kAttributeNormals))
		{
			//tpLogNotify("%s %d normals",__FUNCTION__,prim.getNormals().getSize());
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, prim.getNormals().getData());
		}

		if (prim.hasAttribute(tpPrimitive::kAttributeUV))
		{
			//tpLogNotify("%s %d texcoords",__FUNCTION__,mesh->getTexCoords().getSize());
			//float tex[] = {0,0, 0,1, 1,0, 1,1};
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, prim.getTexCoords().getData());
		}

		if (prim.hasAttribute(tpPrimitive::kAttributeColors))
		{
			//tpLogNotify("%s %d color",__FUNCTION__,prim.getColors().getSize());
			//float tex[] = {0,0, 0,1, 1,0, 1,1};
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, 0, prim.getColors().getData());
		}

		glDrawArrays(prim.getPrimitiveType(),0,prim.getVertexCount());

		glDisableClientState(GL_VERTEX_ARRAY);

		if (prim.hasAttribute(tpPrimitive::kAttributeNormals))
		{
			glDisableClientState(GL_NORMAL_ARRAY);
		}

		if (prim.hasAttribute(tpPrimitive::kAttributeUV))
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		if (prim.hasAttribute(tpPrimitive::kAttributeColors))
		{
			glDisableClientState(GL_COLOR_ARRAY);
		}
	}
};

tpGLRendererTraits tpGLRenderer::mRendererTraits;

TP_TYPE_REGISTER(tpGLRenderer,tpRenderer,OpenGL1_Renderer);

tpModuleInitializer<tpGLRenderer> g_gl1_renderer;

