#include <tp/renderer.h>
#include <tp/node.h>
#include <tp/camera.h>
#include <tp/nodeops.h>
#include <tp/module.h>
#include <tp/primitive.h>
#include <tp/version.h>
#include <tp/log.h>

//#if defined(__APPLE__)
//    #include <OpenGL/OpenGL.h>
//#elif defined(_WIN32)
////#include <GLES/gl.h>
////#include <Windows.h>
//#else
//#include <GL/gl.h>
//#endif

#if defined(TP_USE_X11)
	#include <GL/gl.h>
	#include <GL/glx.h>
#endif

#if defined(TP_USE_COCOA)
	#include <OpenGL/gl.h>
#endif

struct tpGLRendererTraits : tpRendererTraits {
};

class tpRendererGL1x : public tpRenderer
{
public:

	tpRendererGL1x()
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
		glEnable(GL_RESCALE_NORMAL);

		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		// hack
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		float amb_light[] = {0.2f, 0.2f, 0.2f, 1.0f};

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

		float blueish[] = { 0.9f, 0.8f, 0.8f, 1 };
		glLightfv(GL_LIGHT0,GL_DIFFUSE,blueish);
		glLightfv(GL_LIGHT0,GL_SPECULAR,blueish);

		//#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
//		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

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

	void operator ()(const tpMaterial* mat)
	{
		const tpMaterial* actual_mat = (mat) ? mat : tpDefaultMaterial;

		glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;

		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, actual_mat->getAmbientColor().getData() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, actual_mat->getDiffuseColor().getData() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, actual_mat->getSpecularColor().getData() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, actual_mat->getEmissiveColor().getData() );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, actual_mat->getShininess() );
	}

	void operator()(const tpPrimitive& prim,const tpMat44r& modelmatrix)
	{
		(*this)(prim.getMaterial());

		tpCamera* camera = getActiveCamera();
		tpMat44r view = camera->getViewInverse();
		view *= modelmatrix;

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(camera->getProjection().data());

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(view.data());


		if (prim.hasAttribute(tpPrimitive::kAttributeNormals))
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, prim.getNormals().getData());
		}

		if (prim.hasAttribute(tpPrimitive::kAttributeUV))
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, prim.getTexCoords().getData());
		}

		if (prim.hasAttribute(tpPrimitive::kAttributeColors))
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, 0, prim.getColors().getData());
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, prim.getVertices().getData());

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

tpGLRendererTraits tpRendererGL1x::mRendererTraits;

TP_TYPE_REGISTER(tpRendererGL1x,tpRenderer,RendererGL1x);

tpModuleInitializer<tpRendererGL1x> g_gl1_renderer;

