#include <tp/renderer.h>
#include <tp/node.h>
#include <tp/camera.h>
#include <tp/nodeops.h>
#include <tp/module.h>
#include <tp/primitive.h>
#include <tp/version.h>
#include <tp/log.h>
#include <tp/logutils.h>
#include <tp/light.h>
#include <tp/timer.h>


//#if defined(__APPLE__)
//    #include <OpenGL/OpenGL.h>
//#elif defined(_WIN32)
////#include <GLES/gl.h>
////#include <Windows.h>
//#else
//#include <GL/gl.h>
//#endif


#if defined(TP_USE_OPENGL)
	#if defined(__APPLE__)
		#include <OpenGL/gl.h>
	#else
		#include <GL/gl.h>
	#endif
#endif


#define glErrorCheck \
		GLenum err = glGetError(); \
		if (err) tpLogNotify("OpenGL error 0x%x %d",err,err);

class tpTextureObjectGL : public tpTextureObject {
protected:

	GLuint mGLID;
	tpInt mChangeCount;

	tpVec2i npot;

public:
	tpTextureObjectGL()
		: mGLID(0)
		, mChangeCount(-1)
	{}

	tpVec2i getSize() const {
		return npot;
	}

	GLenum getWrapMode(tpUInt mode) {
		switch (mode) {
		case tpTexture::kWrapModeClamp:
			return GL_CLAMP;
		case tpTexture::kWrapModeRepeat:
			return GL_REPEAT;
		}
		return 0;
	}


	GLenum getFilterMode(const tpTexture& t) {
		return 0;
	}


	GLenum getFormat(const tpTexture& texture) {
		switch (texture.getFormat()) {
		case tpTexture::kFormatAlpha:
			return GL_ALPHA;
		case tpTexture::kFormatLuminance:
			return GL_LUMINANCE;
		case tpTexture::kFormatRGB:
			return GL_RGB;
		case tpTexture::kFormatRGBA:
			return GL_RGBA;
		case tpTexture::kFormatBGR:
			return GL_BGR;
		case tpTexture::kFormatBGRA:
			return GL_BGRA;
		}

		tpLogError("%s invalid image format for texture",__FUNCTION__);
		return 0;
	}

	GLint getInternalFormat(const tpTexture& texture) {

		// GL_ALPHA and GL_LUMINANCE need to be set simultanously
		if (texture.getFormat() == tpTexture::kFormatAlpha) {
			return GL_ALPHA;
		}

		// some graphics cards insist on the symbolic name - screw them!
		return (tpPixelFormat::getBitsPerPixel(texture.getImage()->getPixelFormat())/8);
	}

	void create(tpTexture &texture) {
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1,&mGLID);

		glErrorCheck;
	}

	void update(tpTexture &texture) {

		if (texture.getImage()->getChangeCount() != mChangeCount) {

			// bind!
			this->activate();

			// hack!
			glEnable(GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			GLenum format = getFormat(texture);


			if (mChangeCount < 0) {

				npot[0] = tpNextPowerOfTwo(texture.getImage()->getWidth());
				npot[1] = tpNextPowerOfTwo(texture.getImage()->getHeight());

				GLint internalFormat = getInternalFormat(texture);

				//
				tpLogNotify("%s 0x%x 0x%x %dx%d (%dx%d)",__FUNCTION__,format,internalFormat,
							texture.getImage()->getWidth(),texture.getImage()->getHeight(),
							npot[0],npot[1]);

				glPixelStorei(GL_UNPACK_ALIGNMENT,1);

				// only works for OpenGL 1.4 upwards
//				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);


				glTexImage2D(GL_TEXTURE_2D,0,internalFormat,npot[0],npot[1],0,format,GL_UNSIGNED_BYTE,0);


			}

			// subload
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,
							texture.getImage()->getWidth(),texture.getImage()->getHeight(),
							format,GL_UNSIGNED_BYTE,texture.getImage()->getData());


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getWrapMode(texture.getWrapMode()[0]));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getWrapMode(texture.getWrapMode()[0]));

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

//				tpGL::TexParameteri(tpGL::TEXTURE_2D,tpGL::TEXTURE_MIN_FILTER,tpGL::LINEAR);
//				tpGL::TexParameteri(tpGL::TEXTURE_2D,tpGL::TEXTURE_MAG_FILTER,tpGL::LINEAR);

//				tpGL::TexParameteri(tpGL::TEXTURE_2D,tpGL::TEXTURE_WRAP_S, tpGL::REPEAT);
//				tpGL::TexParameteri(tpGL::TEXTURE_2D,tpGL::TEXTURE_WRAP_T, tpGL::REPEAT);



			mChangeCount = texture.getImage()->getChangeCount();

//			if (!texture.getStatic()) texture.setImage(0);

		}
	}

	void activate() {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,mGLID);
	}

	void deactivate() {
		glDisable(GL_TEXTURE_2D);
	}

	void destroy() {
		glDeleteTextures(1,&mGLID);
	}

};

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

	TP_TYPE_DECLARE

	static tpGLRendererTraits mRendererTraits;
	const tpRendererTraits& getTraits() const { return mRendererTraits; }

	void operator()(tpNode* node)
	{

		tpTimer t;

		tpCamera* camera = getActiveCamera();

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


		// ok, now we can bail out if there are actually no nodes
		if (0 == node) return;


		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);


//		glEnable(GL_NORMALIZE);
//		glEnable(GL_RESCALE_NORMAL);
//		glShadeModel(GL_FLAT);
		glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;


		//#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
//		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

		// setup lights
		tpNodeMatrixMap nodemap_lights = tpNodeOps::getNodeMatrixMap(node,tpLight::getTypeInfo());

		if (nodemap_lights.getSize())
		{
			// hack
			//glEnable(GL_LIGHTING);

			for (tpNodeMatrixMap::iterator i = nodemap_lights.begin();
				 i != nodemap_lights.end();
				 ++i)
			{
				(*this)(*static_cast<tpLight*>((*i).getKey()),(*i).getValue());
			}
		}

		// render nodes
		tpNodeMatrixMap nodemap_primitives = tpNodeOps::getNodeMatrixMap(node,tpPrimitive::getTypeInfo());

		for (tpNodeMatrixMap::iterator i = nodemap_primitives.begin();
			 i != nodemap_primitives.end();
			 ++i)
		{
			tpPrimitive* p = static_cast<tpPrimitive*>((*i).getKey());
			(*this)(*p,(*i).getValue());
		}

		glErrorCheck;

//		GLint error = glGetError();
//		if (error) tpLogError("glGetError() 0x%x (%d)",error,error);

//		tpLogNotify("%s %d lights %d nodes %3.3fms",
//					__FUNCTION__,nodemap_lights.getSize(),nodemap_primitives.getSize(),
//					t.getElapsed(tpTimer::kTimeMilliSeconds));
	}

	void operator ()(const tpLight& light, const tpMat44r& modelmatrix)
	{

		tpCamera* camera = getActiveCamera();

		// just compile the modelviewprojection matrix (prepared for OpenGL 2.0 / ES 2.0)
		tpMat<4,4,float> mvp = modelmatrix * camera->getViewInverse() * camera->getProjection();

		// HACK!
		mvp.at(11) += light.getPosition()[0];
		mvp.at(12) += light.getPosition()[1];
		mvp.at(13) += light.getPosition()[2];

		// load model view
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(mvp.data());

		GLenum lid = light.getID()+GL_LIGHT0;
		glEnable(lid);
		glLightfv(lid,GL_POSITION,light.getPosition().getData());
		glLightfv(lid,GL_AMBIENT,light.getAmbientColor().getData());
		glLightfv(lid,GL_DIFFUSE,light.getDiffuseColor().getData());
		glLightfv(lid,GL_SPECULAR,light.getSpecularColor().getData());

	}

	void operator ()(tpTexture* tex) {

		if (0 == tex->getTextureObject()) {
			tpTextureObjectGL* newtexobj = new tpTextureObjectGL();
			newtexobj->create(*tex);
			tex->setTextureObject(newtexobj);
		}

		tex->getTextureObject()->update(*tex);
		tex->getTextureObject()->activate();
	}

	void operator ()(const tpMaterial* mat)
	{
		const tpMaterial* actual_mat = (mat) ? mat : tpDefaultMaterial;

		glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, actual_mat->getAmbientColor().getData() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, actual_mat->getDiffuseColor().getData() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, actual_mat->getSpecularColor().getData() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, actual_mat->getEmissiveColor().getData() );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, actual_mat->getShininess() );
	}

	void operator()(const tpPrimitive& prim,const tpMat44r& modelmatrix)
	{
		if (prim.hasTexture()) {
			(*this)(const_cast<tpTexture*>(prim.getTexture()));
		} else {
			(*this)(prim.getMaterial());
		}

		tpCamera* camera = getActiveCamera();

		// we are using our own transformation stack
		glMatrixMode(GL_MODELVIEW);

//		 hence just compile the modelviewprojection matrix (prepared for OpenGL 2.0 / ES 2.0)
		tpMat<4,4,float> mvp = modelmatrix * camera->getViewInverse() * camera->getProjection();

		// load on the stack
		glLoadMatrixf(mvp.data());

		// render scene
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

		if (prim.hasTexture()) {
			const_cast<tpTexture*>(prim.getTexture())->getTextureObject()->deactivate();
		}

	}
};

tpGLRendererTraits tpRendererGL1x::mRendererTraits;

TP_TYPE_REGISTER(tpRendererGL1x,tpRenderer,RendererGL1x);

tpModuleInitializer<tpRendererGL1x> g_gl1_renderer;

