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
#include <tp/scene.h>


#include <tp/config.h>

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
	#elif defined(_WIN32) || defined(__MINGW32__)
		#define WIN32_LEAN_AND_MEAN 1
		#include <Windows.h>
		#include <GL/gl.h>
		#define GL_BGR 0x80E0
		#define GL_BGRA 0x80E1
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


	GLenum setGLFilterMode(const tpTexture& t) {

		GLenum minFilter(GL_LINEAR_MIPMAP_LINEAR);
		GLenum magFilter(GL_LINEAR_MIPMAP_LINEAR);

//		if		(t.hasFilter(tpTexture::kFilterNearest | tpTexture::kFilterMipMapNearest,tpTexture::kDownSampling))
//			minFilter = GL_NEAREST_MIPMAP_NEAREST;
//		else if (t.hasFilter(tpTexture::kFilterLinear | tpTexture::kFilterMipMapLinear,tpTexture::kDownSampling))
//			minFilter = GL_LINEAR_MIPMAP_LINEAR;
//		else if (t.hasFilter(tpTexture::kFilterNearest | tpTexture::kFilterMipMapLinear,tpTexture::kDownSampling))
//			minFilter = GL_NEAREST_MIPMAP_LINEAR;
//		else if (t.hasFilter(tpTexture::kFilterLinear | tpTexture::kFilterMipMapLinear,tpTexture::kDownSampling))
//			minFilter = GL_LINEAR_MIPMAP_LINEAR;
//		else if (t.hasFilter(tpTexture::kFilterLinear | tpTexture::kFilterMipMapLinear,tpTexture::kDownSampling))
//			minFilter = GL_LINEAR_MIPMAP_LINEAR;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);


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
//		glActiveTexture(GL_TEXTURE0);
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


	void operator()(tpScene* scene)
	{
		static int count(0);

		count++;

		tpTimer t;
		for (tpRefCameraArray::iterator it = scene->getCameras().begin();
			 it != scene->getCameras().end();
			 ++it)
		{
			this->onCamera((*it).get());
			this->onNode((*it).get());
		}

		if (0 == (count % 100))
		{
			tpLogNotify("t %lf ms",t.getElapsed(tpTimer::kTimeMilliSeconds));
		}

		glErrorCheck
	}


	void
	onCamera(const tpCamera* camera)
	{

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


	}

	void
	onNode(tpNode* node)
	{

		tpTimer t;

		// ok, now we can bail out if there are actually no nodes
		if (0 == node) return;


		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);


//		glEnable(GL_NORMALIZE);
//		glEnable(GL_RESCALE_NORMAL);
//		glShadeModel(GL_FLAT);
//		glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;



		// setup lights
		tpNodeMatrixStackMap nodemap_lights = tpNodeOps::getNodeMatrixStackMap(node,tpLight::getTypeInfo());

		if (nodemap_lights.getSize())
		{
			for (tpNodeMatrixStackMap::iterator i = nodemap_lights.begin();
				 i != nodemap_lights.end();
				 ++i)
			{
				onLight(*static_cast<tpLight*>((*i).getKey()),(*i).getValue());
			}
		}

		// render nodes
		tpNodeMatrixStackMap nodemap_primitives = tpNodeOps::getNodeMatrixStackMap(node,tpPrimitive::getTypeInfo());

		for (tpNodeMatrixStackMap::iterator i = nodemap_primitives.begin();
			 i != nodemap_primitives.end();
			 ++i)
		{
			tpPrimitive* p = static_cast<tpPrimitive*>((*i).getKey());
			onPrimitive(*p,(*i).getValue());
		}

		glErrorCheck;

//		GLint error = glGetError();
//		if (error) tpLogError("glGetError() 0x%x (%d)",error,error);

//        tpLogNotify("%s %d lights %d nodes %3.3fms",
//                    __FUNCTION__,nodemap_lights.getSize(),nodemap_primitives.getSize(),
//                    t.getElapsed(tpTimer::kTimeMilliSeconds));
	}

	void onLight(const tpLight& light, const tpMatrixStack& stack)
	{

//		tpCamera* camera = getActiveCamera();

		// just compile the modelviewprojection matrix (prepared for OpenGL 2.0 / ES 2.0)
//		tpMat<4,4,float> mvp = stack.model * camera->getViewInverse() * camera->getProjection();
		tpMat<4,4,float> mvp = stack.model * stack.view * stack.projection;

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

		//#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
		//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

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
		if (0 == mat) return;

//		const tpMaterial* actual_mat = (mat) ? mat : tpDefaultMaterial;

		glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat->getAmbientColor().getData() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat->getDiffuseColor().getData() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat->getSpecularColor().getData() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, mat->getEmissiveColor().getData() );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mat->getShininess() );
	}

	void onPrimitive(const tpPrimitive& prim,const tpMatrixStack& stack,bool secondPass = false)
	{

		// setup alpha
		if (prim.hasAlpha())
		{
			if (!secondPass)
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable( GL_BLEND );
				glDisable(GL_LIGHTING);
			} else {
//				glDisable(GL_BLEND);
//				glEnable(GL_LIGHTING);
//				glDepthFunc(GL_EQUAL);
			}

		} else {

			// first setup the lighting
			if (prim.getLighting())
			{
				glEnable(GL_LIGHTING);
			} else
			{
				glDisable(GL_LIGHTING);
			}

		}


		if (prim.hasTexture())
		{
			(*this)(const_cast<tpTexture*>(prim.getTexture()));
		}

		if (prim.hasMaterial()) {
			(*this)(prim.getMaterial());
		} else if (prim.hasColors()) {
			glEnable(GL_COLOR_MATERIAL);
		}

//		tpCamera* camera = getActiveCamera();

		// we are using our own transformation stack
		glMatrixMode(GL_MODELVIEW);

//		 hence just compile the modelviewprojection matrix (prepared for OpenGL 2.0 / ES 2.0)
//		tpMat<4,4,float> mvp = stack.model * camera->getViewInverse() * stack.projection;

		// actually the view matrix is the view inverse already
		tpMat<4,4,float> mvp = stack.model * stack.view * stack.projection;

		// load on the stack
		glLoadMatrixf(mvp.data());

		// render scene
		if (prim.hasNormals())
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT,
							prim.getNormals().getStride()*sizeof(float),
							prim.getNormals().getData()
							);
		} else {
			// no normals no fun
			glDisable(GL_LIGHTING);
		}

		if (prim.hasTextureCoordinates())
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(prim.getTextureCoordinates().getStride(),
							  GL_FLOAT,
							  prim.getTextureCoordinates().getStride()*sizeof(float),
							  prim.getTextureCoordinates().getData()
							  );
		}

		if (prim.hasColors())
		{
			glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

			glColorPointer(prim.getColors().getStride(),
						   GL_FLOAT,
						   prim.getColors().getStride()*sizeof(float),
						   prim.getColors().getData()
						   );

			glEnableClientState(GL_COLOR_ARRAY);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(prim.getVertices().getStride(), GL_FLOAT, 0, prim.getVertices().getData());

		glDrawArrays(prim.getPrimitiveType(),0,prim.getVertices().getSize());

		glDisableClientState(GL_VERTEX_ARRAY);

		if (prim.hasNormals())
		{
			glDisableClientState(GL_NORMAL_ARRAY);
		}

		if (prim.hasTextureCoordinates())
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		if (prim.hasColors())
		{
			glDisable(GL_COLOR_MATERIAL);
			glDisableClientState(GL_COLOR_ARRAY);
		}


		// disable state


		if (prim.hasTexture()) {
			const_cast<tpTexture*>(prim.getTexture())->getTextureObject()->deactivate();
		}

		if (prim.hasAlpha()) {
			if (!secondPass) {
//				onPrimitive(prim,stack,true);
				glDepthFunc(GL_LESS);
			}
		}


	}
};

tpGLRendererTraits tpRendererGL1x::mRendererTraits;

TP_TYPE_REGISTER(tpRendererGL1x,tpRenderer,RendererGL1x);

tpModuleInitializer<tpRendererGL1x> g_gl1_renderer;

