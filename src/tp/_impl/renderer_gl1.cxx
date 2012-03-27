/**
  * A small OpenGL fixed function pipeline renderer
  */
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
#include <tp/thread.h>
#include <tp/mutex.h>
#include <tp/scopelock.h>

#include <tp/config.h>

#if defined (TP_OS_IOS)
	#include <OpenGLES/es1/gl.h>
#elif defined(TP_OS_OSX) && defined(TP_USE_OPENGL)
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#elif (defined(_WIN32) || defined(__MINGW32__))  && defined(TP_USE_OPENGL)
	#define WIN32_LEAN_AND_MEAN 1
	#include <Windows.h>
	#include <GL/gl.h>
	#define GL_BGR 0x80E0
	#define GL_BGRA 0x80E1
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif


void
tpDebugPrimitive(const tpPrimitive& p)
{
	for (int i = 0; i < p.getVertices().getSize();++i)
	{
		tpVec4f v(p.getVertices().getData()[i*4],
				  p.getVertices().getData()[i*4+1],
				  p.getVertices().getData()[i*4+2],
				  p.getVertices().getData()[i*4+3]
				  );

		tpVec4f n(p.getNormals().getData()[i*p.getNormals().getStride()],
				  p.getNormals().getData()[i*p.getNormals().getStride()+1],
				  p.getNormals().getData()[i*p.getNormals().getStride()+2],
				  p.getNormals().getData()[i*p.getNormals().getStride()+3]
				  );

//        tpVec4f c(p.getColors().getData()[i*4],
//                  p.getColors().getData()[i*4+1],
//                  p.getColors().getData()[i*4+2],
//                  p.getColors().getData()[i*4+3]
//                  );


		tpLog::get() << "\nv " << v <<
						"\nn " << n <<
//                        "\nc " << c <<
						"\n";
	}
}


struct tpTimerProbe {

	tpArray<tpDouble> probes;

	tpDouble getAverage() const {
		tpDouble result(0);
		for (tpArray<tpDouble>::const_iterator it = probes.begin();
			 it != probes.end();
			 ++it)
			result += (*it);
		return (probes.getSize() ? result/probes.getSize() : 0);
	}
};

#define glErrorCheck \
		{ GLenum err = glGetError(); \
		if (err) tpLogNotify("OpenGL error 0x%x %d line:%d",err,err,__LINE__); }

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
			return GL_CLAMP_TO_EDGE;
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
#if defined (GL_BGR)
		case tpTexture::kFormatBGR:
			return GL_BGR;
		case tpTexture::kFormatBGRA:
			return GL_BGRA;
#endif
		}

		tpLogError("%s invalid image format for texture",__FUNCTION__);
		return 0;
	}

	GLint getInternalFormat(const tpTexture& texture) {

		// GL_ALPHA and GL_LUMINANCE need to be set simultaneously
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

			//// hack!
			//glEnable(GL_BLEND);
			//glBlendFunc (GL_SRC_ALPHA, GL_ONE);

			GLenum format = getFormat(texture);

			if (mChangeCount < 0) {

				npot(0) = tpNextPowerOfTwo(texture.getImage()->getWidth());
				npot(1) = tpNextPowerOfTwo(texture.getImage()->getHeight());

				GLint internalFormat = getInternalFormat(texture);

				//
				tpLogNotify("%s 0x%x 0x%x %dx%d (%dx%d)",__FUNCTION__,format,internalFormat,
							texture.getImage()->getWidth(),texture.getImage()->getHeight(),
							npot(0),npot(1));

				glPixelStorei(GL_UNPACK_ALIGNMENT,1);

				// only works for OpenGL 1.4 upwards
//				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);


				glTexImage2D(GL_TEXTURE_2D,0,internalFormat,npot(0),npot(1),0,format,GL_BYTE,0);


			}

			// subload
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,
							texture.getImage()->getWidth(),texture.getImage()->getHeight(),
							format,GL_UNSIGNED_BYTE,texture.getImage()->getData());


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getWrapMode(texture.getWrapMode()(0)));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getWrapMode(texture.getWrapMode()(1)));




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

	tpInt mMaxVertices;
	tpInt mMaxElements;

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
//        tpMutex mutex;
//        tpScopeLock<tpMutex> lock(mutex);

		static int count(0);

		count++;

		// defaults
		glEnable(GL_DEPTH_TEST);

		for (tpRefCameraArray::iterator it = scene->getCameras().begin();
			 it != scene->getCameras().end();
			 ++it)
		{
			onCamera((*it).get());
			onNode((*it).get());
		}

		glErrorCheck;

		glFinish();
		glFlush();

		glErrorCheck;
	}


	bool
	onCamera(const tpCamera* camera)
	{
		glErrorCheck;

		// if the viewport is not being set just skip
		if ((camera->getViewport()(2) > 0) && (camera->getViewport()(3) > 0))
			glViewport(0,0,camera->getViewport()(2),camera->getViewport()(3));

		glErrorCheck;

		tpUInt glclearflag(0);
		if (camera->hasClearFlag(tpCamera::kClearColor))
		{
			glClearColor(camera->getClearColor()(0),camera->getClearColor()(1),camera->getClearColor()(2),camera->getClearColor()(2) );
			glclearflag |= GL_COLOR_BUFFER_BIT;
		}

		if (camera->hasClearFlag(tpCamera::kClearDepth))
		{
			glclearflag |= GL_DEPTH_BUFFER_BIT;
		}

		if (glclearflag) glClear(glclearflag);

		glErrorCheck;


		return true;
	}

	void
	onNode(tpNode* node)
	{
		tpTimer t;

		t.start();

		// ok, now we can bail out if there are actually no nodes
		if (0 == node) return;

		//glShadeModel(GL_SMOOTH);
		//glEnable(GL_DEPTH_TEST);


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

		glErrorCheck;

		// render nodes
		tpNodeMatrixStackMap nodemap_primitives = tpNodeOps::getNodeMatrixStackMap(node,tpPrimitive::getTypeInfo());

		for (tpNodeMatrixStackMap::iterator i = nodemap_primitives.begin();
			 i != nodemap_primitives.end();
			 ++i)
		{
			tpPrimitive* p = static_cast<tpPrimitive*>((*i).getKey());
			onPrimitive(*p,(*i).getValue());

//            tpDebugPrimitive(*p);
		}

		glErrorCheck;

//		GLint error = glGetError();
//		if (error) tpLogError("glGetError() 0x%x (%d)",error,error);

		static int loops = 0;

		if (loops > 100) {
			tpLogNotify("%s %d lights %d nodes %3.3fms",
						__FUNCTION__,nodemap_lights.getSize(),nodemap_primitives.getSize(),
						t.getElapsed(tpTimer::kTimeMilliSeconds));
			loops = 0;
		}
		loops++;
	}

	void
	onLight(const tpLight& light, const tpMatrixStack& stack)
	{
		applyStack(stack);

		// enable the light with the respective id
		GLenum lid = light.getID()+GL_LIGHT0;


		tpVec4f pos,dir;

		if (light.isSpot()) {

			tpLogNotify("Spot light");

			float focus = 1.f;
			float angle = 45.f;

			// just all zero
			glLightfv(lid, GL_POSITION,			light.getPosition().data());
			glLightfv(lid, GL_SPOT_DIRECTION,   dir.data());

			// set parameters and position
			glLightf (lid, GL_SPOT_EXPONENT,    focus);
			glLightf (lid, GL_SPOT_CUTOFF,      angle);

		} else {

			// default light to avoid a bug in the r200 drivers on linux
			// if the position is being set wrong
			GLfloat defaultPos[] = {0.0, 0.0, 1.0, 0.0};

			glLightfv(lid, GL_POSITION, light.isValid() ? light.getPosition().data() : defaultPos);
		}


		glLightfv(lid,GL_AMBIENT,light.getAmbientColor().data());
		glLightfv(lid,GL_DIFFUSE,light.getDiffuseColor().data());
		glLightfv(lid,GL_SPECULAR,light.getSpecularColor().data());

//        glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION,    0.0f);
//        glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
//        glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION,  0.0);

		glEnable(lid);

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

	void
	onMaterial(const tpMaterial* mat) const
	{
		if (0 == mat) return;

		// we are simulating OpenGL ES - hence front and back are always set together
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat->getAmbientColor().data() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat->getDiffuseColor().data() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat->getSpecularColor().data() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, mat->getEmissiveColor().data() );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mat->getShininess() );
	}


	void quickTest()
	{
		float vertices[] = { 0,0,0, 0,1,0, 0,0,1, 0,0,1 };
		float normals[] =  { 0,0,1, 0,0,1, 0,0,1, 0,0,1 };


		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glNormalPointer(GL_FLOAT, 0, normals);
		glVertexPointer(3, GL_FLOAT, 0, vertices);

		glDrawArrays(GL_TRIANGLES,0,4);

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

	}

	GLenum getGL(const tpUInt blendval) {

//        tpMap<tpUInt,GLenum> tp2glblend;
//        tp2glblend.add(tpRenderFlag::kBlendOneMinusAlpha,GL_ONE_MINUS_SRC_ALPHA);

		switch (blendval) {
			case tpRenderFlag::kBlendOneMinusAlpha : return GL_ONE_MINUS_SRC_ALPHA;
			case tpRenderFlag::kBlendZero : return GL_ZERO;
			case tpRenderFlag::kBlendOne : return GL_ONE;
			case tpRenderFlag::kBlendSrcAlpha : return GL_SRC_ALPHA;

			// color material
		}
		return 0;
	}


	void pushRenderFlags(const tpRenderFlagMap& map)
	{
		// basically we are going back to OpenGL defaults
		for(tpRenderFlagMap::const_iterator it = map.begin();
			it != map.end();
			it++)
		{
			switch (it->getKey()) {
			case tpRenderFlag::kColorMaterial:
				//glColorMaterial(GL_FRONT_AND_BACK,GL_);
				glEnable(GL_COLOR_MATERIAL);
				break;
			case tpRenderFlag::kLighting:
				glEnable(GL_LIGHTING);
				break;
			case tpRenderFlag::kBlending:
				//glBlendFunc(getGL(it->getValue().value1),getGL(it->getValue().value2));
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				break;
			default:
				break;
			}
		}
	}

	void popRenderFlags(const tpRenderFlagMap& map)
	{
		for(tpRenderFlagMap::const_iterator it = map.begin();
			it != map.end();
			it++)
		{
			switch (it->getKey()) {
			case tpRenderFlag::kColorMaterial:
				glDisable(GL_COLOR_MATERIAL);
				break;
			case tpRenderFlag::kLighting:
				glDisable(GL_LIGHTING);
				break;
			case tpRenderFlag::kBlending:
				glDisable(GL_BLEND);
				glBlendFunc(GL_ONE,GL_ZERO);
				break;
			default:
				break;
			}
		}
	}

	void applyStack(const tpMatrixStack& stack)
	{
		// just sanity check - we are not touching the projection matrix usually
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(stack.projection.data());

		// we are using our own transformation stack - hence all is done in the MV
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(stack.view.data());

		// load on the stack
		glMultMatrixf(stack.model.data());

	}


	void onPrimitive(const tpPrimitive& prim,const tpMatrixStack& stack,bool secondPass = false)
	{

		// set render flags
		pushRenderFlags(prim.getRenderFlags());

		// apply all transforms
		applyStack(stack);

		// update textures/materials
		if (prim.hasTexture()) {
			(*this)(const_cast<tpTexture*>(prim.getTexture()));
		} else if (prim.hasMaterial()) {
			onMaterial(prim.getMaterial());
		}

		// enable all relevant states
		if (prim.hasNormals()) glEnableClientState(GL_NORMAL_ARRAY);
		if (prim.hasColors()) glEnableClientState(GL_COLOR_ARRAY);
		if (prim.hasTextureCoordinates()) glEnableClientState(GL_TEXTURE_COORD_ARRAY);


		// setup rendering state
		glEnableClientState(GL_VERTEX_ARRAY);

		if (prim.hasNormals()) {
			glNormalPointer(GL_FLOAT,
							prim.getNormals().getStride()*sizeof(float),
							prim.getNormals().getData()
							);
		}

		if (prim.hasTextureCoordinates()) {
			glTexCoordPointer(prim.getTextureCoordinates().getStride(),
							  GL_FLOAT,
							  prim.getTextureCoordinates().getStride()*sizeof(float),
							  prim.getTextureCoordinates().getData()
							  );
		}

		if (prim.hasColors()) {
			glColorPointer(prim.getColors().getStride(),
						   GL_FLOAT,
						   prim.getColors().getStride()*sizeof(float),
						   prim.getColors().getData()
						   );
		}

		glVertexPointer(prim.getVertices().getStride(),
						GL_FLOAT, 0,
						prim.getVertices().getData()
						);

		// if indices

//		if (prim.getIndices().getSize()) {
//
//			glDrawElements(prim.get, <#GLsizei count#>, <#GLenum type#>, <#const GLvoid *indices#>)
//
//		} else {

			glDrawArrays(prim.getPrimitiveType(),0,prim.getVertices().getSize());
//		}

		// and all off again
		glDisableClientState(GL_VERTEX_ARRAY);

		if (prim.hasNormals()) glDisableClientState(GL_NORMAL_ARRAY);
		if (prim.hasTextureCoordinates()) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		if (prim.hasColors()) glDisableClientState(GL_COLOR_ARRAY);

		// disable texture state
		if (prim.hasTexture()) {
			const_cast<tpTexture*>(prim.getTexture())->getTextureObject()->deactivate();
		}

		popRenderFlags(prim.getRenderFlags());

	}
};

tpGLRendererTraits tpRendererGL1x::mRendererTraits;

TP_TYPE_REGISTER(tpRendererGL1x,tpRenderer,RendererGL1x);
TP_MODULE_REGISTER(RendererGL1x,tpRendererGL1x)
TP_MODULE_USE(RendererGL1x)

//tpModuleInitializer<tpRendererGL1x> g_gl1_renderer;

