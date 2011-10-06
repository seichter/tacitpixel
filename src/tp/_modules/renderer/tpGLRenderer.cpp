/*
* Twisted Pair Visualization Engine
*
* Copyright (c) 1999-2009 Hartmut Seichter
*
* This library is open source and may be redistributed and/or modified under
* the terms of the Twisted Pair License (TPL) version 1.0 or (at your option)
* any later version. The full license text is available in the LICENSE file
* included with this distribution, and on the technotecture.com website.
*
*/


#include "tpGLRenderer.h"
#include "tpGLShader.h"
#include "tpGL.h"

#include <tp/renderer.h>
#include <tp/camera.h>
#include <tp/primitive.h>
#include <tp/transform.h>
#include <tp/stack.h>
#include <tp/log.h>
#include <tp/stringformater.h>

#define tpMatRep(mat) \
			{ tpString mstr; \
				mstr << mat; \
				tpLog::get() << mstr.c_str(); \
			}


class tpGLFixedFunctionTraverser : public tpGLTraverser {
public:

	tpStack<tpMat44r> mvs;
	tpStack<tpMaterial> materials;

	tpGLFixedFunctionTraverser() : tpGLTraverser() {
		tpMat44r identity; identity.identity();
	}

	void operator()(tpNode* node,tpCamera* camera)
	{
		mvs.push(camera->getView());

		tpGL::Enable(tpGL::NORMALIZE);
		tpGL::Enable(tpGL::AUTO_NORMAL);

		tpGL::Enable(tpGL::COLOR_MATERIAL);

		tpGL::Enable(tpGL::LIGHTING);
		tpGL::Enable(tpGL::LIGHT0);

		// hack
		float amb_light[] = {0.2f, 0.2f, 0.2f, 1.0f};
		tpGL::LightModelfv(tpGL::LIGHT_MODEL_AMBIENT, amb_light);
		tpGL::LightModelf(tpGL::LIGHT_MODEL_LOCAL_VIEWER, 0.0f);
		tpGL::LightModelf(tpGL::LIGHT_MODEL_TWO_SIDE, 0.0f);

		//#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
		tpGL::LightModeli(tpGL::LIGHT_MODEL_COLOR_CONTROL, 0x81FA);


		tpGL::ShadeModel(tpGL::SMOOTH);


		tpGL::Enable(tpGL::DEPTH_TEST);

		tpGL::glFrontFace(tpGL::CCW);
		//tpGL::Disable(tpGL::CULL_FACE);

		tpGL::Enable(tpGL::LINE_SMOOTH);
		//tpGL::Enable(tpGL::SMOOTH);

		TP_REPORT_GLERROR();

		// deal with the camera
		if (camera)
		{

			//tpGL::Viewport(camera->getViewport()[0],camera->getViewport()[1],camera->getViewport()[2],camera->getViewport()[3]);
			//tpLogMessage("%s - %d,%d - %dx%d",__FUNCTION__,camera->getViewport()[0],camera->getViewport()[1],camera->getViewport()[2],camera->getViewport()[3]);

			TP_REPORT_GLERROR();

			tpUInt glclearflag(0);
			if (camera->hasClearFlag(tpCamera::kClearColor))
			{
				tpGL::ClearColor( camera->getClearColor()[0],camera->getClearColor()[1],camera->getClearColor()[2],camera->getClearColor()[3] );
				glclearflag += tpGL::COLOR_BUFFER_BIT;
			}
			if (camera->hasClearFlag(tpCamera::kClearDepth))
			{
				glclearflag += tpGL::DEPTH_BUFFER_BIT;
			}

			if (glclearflag) tpGL::Clear(glclearflag);
			TP_REPORT_GLERROR();

			tpGL::MatrixMode(tpGL::PROJECTION);
			tpGL::LoadIdentity();
			tpGL::LoadMatrixf(camera->getProjection().data());

			TP_REPORT_GLERROR();

			tpGL::MatrixMode(tpGL::MODELVIEW);
			tpGL::LoadIdentity();

			TP_REPORT_GLERROR();
		}

		// now apply all stuff regarding the nodes
		if (node) node->traverse(*this);

		mvs.pop();
	}


	//template <typename Base, typename Derived>
	//Derived* tpCast(Base* b) { return (Base->getType() == Derived::getTypeInfo()) ? static_cast<Derived>(b) : 0L; }

	//template <typename Base, typename Derived>
	//Derived* tpCastCall(Base* b) { return (Base->getType() == Derived::getTypeInfo()) ? static_cast<Derived>(b) : 0L; }

	void enter(tpNode* node)
	{
		if(node->getType() == tpPrimitive::getTypeInfo()) this->pushPrimitive(static_cast<tpPrimitive*>(node));
		if(node->getType() == tpTransform::getTypeInfo()) this->pushTransform(static_cast<tpTransform*>(node));
	}

	void leave(tpNode* node)
	{
		if(node->getType() == tpTransform::getTypeInfo()) this->popTransform(static_cast<tpTransform*>(node));
	}


	void pushTransform(tpTransform* trans)
	{
		tpMat44r m = mvs.getTop();
		trans->getMatrix(true,m);
		mvs.push(m);
		tpGL::LoadMatrixf(mvs.getTop().data());
	}

	void popTransform(tpTransform* trans)
	{
		mvs.pop();

		tpMatRep(mvs.getTop());

		tpGL::LoadMatrixf(mvs.getTop().data());
	}

	void pushPrimitive(tpPrimitive* prim)
	{

		pushMaterial(prim->getMaterial());

		TP_REPORT_GLERROR();

		tpGL::EnableClientState(tpGL::VERTEX_ARRAY);
		tpGL::VertexPointer(3, tpGL::FLOAT, 0, prim->getVertices().getData());

		if (prim->hasAttribute(tpPrimitive::kAttributeNormals))
		{
			//tpLogNotify("%s %d normals",__FUNCTION__,prim->getNormals().getSize());
			tpGL::EnableClientState(tpGL::NORMAL_ARRAY);
			tpGL::NormalPointer(tpGL::FLOAT, 0, prim->getNormals().getData());
		}

		if (prim->hasAttribute(tpPrimitive::kAttributeUV))
		{
			//tpLogNotify("%s %d texcoords",__FUNCTION__,mesh->getTexCoords().getSize());
			//float tex[] = {0,0, 0,1, 1,0, 1,1};
			tpGL::EnableClientState(tpGL::TEXTURE_COORD_ARRAY);
			tpGL::TexCoordPointer(2, tpGL::FLOAT, 0, prim->getTexCoords().getData());
		}

		if (prim->hasAttribute(tpPrimitive::kAttributeColors))
		{
			//tpLogNotify("%s %d color",__FUNCTION__,prim->getColors().getSize());
			//float tex[] = {0,0, 0,1, 1,0, 1,1};
			tpGL::EnableClientState(tpGL::COLOR_ARRAY);
			tpGL::ColorPointer(4, tpGL::FLOAT, 0, prim->getColors().getData());
		}

		tpGL::DrawArrays(prim->getPrimitiveType(),0,prim->getVertexCount());

		tpGL::DisableClientState(tpGL::VERTEX_ARRAY);

		if (prim->hasAttribute(tpPrimitive::kAttributeNormals))
		{
			tpGL::DisableClientState(tpGL::NORMAL_ARRAY);
		}

		if (prim->hasAttribute(tpPrimitive::kAttributeUV))
		{
			tpGL::DisableClientState(tpGL::TEXTURE_COORD_ARRAY);
		}

		if (prim->hasAttribute(tpPrimitive::kAttributeColors))
		{
			tpGL::DisableClientState(tpGL::COLOR_ARRAY);
		}

		TP_REPORT_GLERROR();
	}

	void pushMaterial(const tpMaterial* mat) {

		if (mat)
		{
			tpGL::Materialfv(tpGL::FRONT_AND_BACK,tpGL::AMBIENT,mat->getAmbientColor().getData());
			tpGL::Materialfv(tpGL::FRONT_AND_BACK,tpGL::DIFFUSE,mat->getDiffuseColor().getData());
			tpGL::Materialfv(tpGL::FRONT_AND_BACK,tpGL::SPECULAR,mat->getSpecularColor().getData());
			tpGL::Materialfv(tpGL::FRONT_AND_BACK,tpGL::EMISSION,mat->getEmissiveColor().getData());

			tpGL::Materialf(tpGL::FRONT_AND_BACK,tpGL::SHININESS,mat->getShininess());

			//materials.push(mat);
		}

		TP_REPORT_GLERROR();
	}

	void popMaterial(const tpMaterial* mat) {

		if (mat)
		{
			//materials.pop();
		}

	}


};



//////////////////////////////////////////////////////////////////////////

tpGLRenderer::tpGLRenderer() : tpRenderer()
{
	if (!tpGL::get().isValid()) tpGL::get().load();
}

tpGLRenderer::~tpGLRenderer()
{
	tpGL::get().release();
}

tpUInt tpGLRenderer::implementsBackend() const { return tpRenderer::kOpenGL; }

void tpGLRenderer::operator()(tpNode* node, tpCamera* camera)
{
#if 0
	tpTimer t;

	//tpGL::glDepthFunc(tpGL::LESS);
	//tpGL::glDepthRange(0,1);

	if (tpGL::Enable)
	{
		//tpGL::Enable(tpGL::LIGHTING);
		//tpGL::Enable(tpGL::NORMALIZE);
		//tpGL::Enable(tpGL::AUTO_NORMAL);
		//tpGL::Enable(tpGL::LIGHT0);

		tpGL::Enable(tpGL::DEPTH_TEST);
		//tpGL::Enable(tpGL::MUL)

		TP_REPORT_GLERROR();


	} else {

		tpLogError("%s invalid OpenGL backend",__FUNCTION__);
		return;
	}

	if (camera)
	{

		}


		//tpVec4f cc = camera->getClearColor();
		//tpGL::ClearColor( cc.vec[0],cc.vec[1],cc.vec[2],cc.vec[3] );


		tpGL::Clear(tpGL::COLOR_BUFFER_BIT | tpGL::DEPTH_BUFFER_BIT);

#if 0
		if ( !tpGL::get().hasShaderSupport() )
		{
			tpGL::MatrixMode(tpGL::PROJECTION);
			tpGL::LoadMatrixf(camera->getProjection().getData());
		}


		// just in case
		if ( !tpGL::get().hasShaderSupport() ) tpGL::MatrixMode(tpGL::MODELVIEW);
#endif
	}

	if (node)
	{
		if (camera && !tpGL::get().hasShaderSupport())
		{
			//tpGL::MatrixMode(tpGL::MODELVIEW);
			//tpGL::LoadIdentity();
			//tpGL::LoadMatrixf( camera->getTransform(false).getData() );
		}

		m_glcontext.setCamera(camera);
		m_glcontext.pushProjection(camera->getProjection());
		m_glcontext.pushModelView(camera->getInverseTransform());

		node->traverse( m_traverser.get(), m_glcontext );

		m_glcontext.popProjection();
		m_glcontext.popModelView();
	}

	TP_REPORT_GLERROR();
	//tpLogNotify("%s - %3.1fms",__FUNCTION__,t.getElapsed(TP_TIME_MILLISEC));
#endif
}

TP_TYPE_REGISTER(tpGLRenderer,tpRenderer,GLRenderer);


//
//
//

#if 0 // needs rewrite

inline static tpUInt nextPowerOfTwo(tpUInt v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	++v;
	return v;
}

struct tpGLImageFormat {
	tpUShort imageformat;
	tpUShort internalformat;
	tpUShort dataformat;
};

tpVoid tpImage2GLFormat( const tpImage& img, tpGLImageFormat& glformat)
{
	glformat.imageformat = 0;
	glformat.internalformat = 0;
	glformat.dataformat = 0;

	if (!img.isValid()) return;

	switch (img.getPixelFormat())
	{
		case TP_RGB565:
			glformat.imageformat = tpGL::RGB;
			glformat.internalformat = tpGL::RGB;
			glformat.dataformat = tpGL::UNSIGNED_SHORT_5_6_5;
			break;
		case TP_RGB888:
			glformat.imageformat = 3;
			glformat.internalformat = tpGL::RGB;
			glformat.dataformat = tpGL::UNSIGNED_BYTE;
			break;
		case TP_RGBA8888:
			glformat.imageformat = 3;
			glformat.internalformat = tpGL::RGBA;
			glformat.dataformat = tpGL::UNSIGNED_BYTE;
			break;
		default:
			tpLogError("Unknown format - need conversion for OpenGL");
			break;
	}
}

class tpGLTraverser : public tpTraverser {
public:

	void push(tpNode* node);
	void pop(tpNode* node);


protected:

	virtual ~tpGLTraverser();
	void applyMaterial(tpNode* node);

};

//class tpGLLightRenderObject : public tpRenderObject {
//
//	tpLight* m_light;
//
//public:
//
//	tpGLLightRenderObject(tpLight* light) : m_light(light)
//	{
//	}
//
//	virtual tpVoid onDraw()
//	{
//		//tpGL::Lightfv(tpGL::AMBIENT,)
//	}
//
//};


#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE0                       0x84C0



class tpGLTextureObject : public tpRenderObject {

	tpRefPtr<tpTexture> m_texture;
	tpRefPtr<tpImage> m_image;

	tpUInt m_texturechanges;


	tpUInt m_glname;
public:
	tpGLTextureObject(tpTexture* texture) : m_texture(texture), m_image(new tpImage()), m_texturechanges(0), m_glname(0)
	{}

	tpVoid createTextureName()
	{

		// ES - does not "enable" a Texture2D - therefore only apply on
		// desktop systems (non-EGL)
		if (! tpGL::get().hasEGL() ) tpGL::Enable(GL_TEXTURE_2D);

		TP_REPORT_GLERROR();

		tpGL::GenTextures(1,&m_glname);

		tpLogNotify("%s id unused (%d)",__FUNCTION__,m_glname);

		TP_REPORT_GLERROR();
	}

	tpVoid onDraw(tpContext& context)
	{
		tpTimer t;

		tpGL::glActiveTexture(GL_TEXTURE0);

		TP_REPORT_GLERROR();

		// HACK!
		onUpdate(context);

		TP_REPORT_GLERROR();

		//! if we do not have a texture
		if (!m_image.isValid()) {

			tpLogNotify("texture not valid");
			//context->setCurrentTexture(NULL);
			tpGL::Disable(tpGL::TEXTURE_2D);

			TP_REPORT_GLERROR();

			return;
		}

		TP_REPORT_GLERROR();

		//tpLogNotify("%s found texture",__FUNCTION__);

		// ES - does not "enable"
		//tpGL::Enable(tpGL::TEXTURE_2D);

		if (!tpGL::IsTexture(m_glname)) createTextureName();

		tpGL::BindTexture(GL_TEXTURE_2D,m_glname);

		TP_REPORT_GLERROR();

		// need upload? - m_texturechanges != m_imagechanges
		// or m_texturechanges = 0
		if (m_texturechanges == 0)
		{
			tpGLImageFormat glformat;
			tpImage2GLFormat(*m_image,glformat);

			tpGL::PixelStorei(tpGL::UNPACK_ALIGNMENT, 1);

			tpInt npot_width = nextPowerOfTwo(m_image->getWidth());
			tpInt npot_height =  nextPowerOfTwo(m_image->getHeight());

			tpGL::TexImage2D(GL_TEXTURE_2D,
				0,
				glformat.imageformat,
				npot_width,
				npot_height,
				0,
				glformat.internalformat,
				glformat.dataformat,
				0);


			tpGL::TexSubImage2D(GL_TEXTURE_2D, 0,
				0,
				0,
				m_image->getWidth(),
				m_image->getHeight(),
				glformat.internalformat,
				glformat.dataformat,
				(void*)m_image->getData());

			m_texturechanges = m_image->getChangeCount();


			tpLogNotify("%s - upload texture with glTexImage2D (%d) %3.1lf '%s'",
				__FUNCTION__,m_texturechanges,t.getElapsed(),
				m_texture->getImage()->getFileName().c_str());

		} else
		if ( m_texturechanges != m_image->getChangeCount() ) {


			tpGLImageFormat glformat;
			tpImage2GLFormat(*m_image,glformat);

			tpGL::PixelStorei(tpGL::UNPACK_ALIGNMENT, 1);

			tpGL::TexSubImage2D(GL_TEXTURE_2D, 0,
				0,
				0,
				m_image->getWidth(),
				m_image->getHeight(),
				glformat.internalformat,
				glformat.dataformat,
				(void*)m_image->getData());

			m_texturechanges = m_image->getChangeCount();

			tpLogNotify("%s - upload texture with glTexSubImage2D (%d) %3.1lf",__FUNCTION__,m_texturechanges,t.getElapsed());

		}

		TP_REPORT_GLERROR();

		tpGL::TexParameteri(tpGL::TEXTURE_2D,tpGL::TEXTURE_MIN_FILTER,tpGL::LINEAR);
		tpGL::TexParameteri(tpGL::TEXTURE_2D,tpGL::TEXTURE_MAG_FILTER,tpGL::LINEAR);

		tpGL::TexParameteri(tpGL::TEXTURE_2D,tpGL::TEXTURE_WRAP_S, tpGL::REPEAT);
		tpGL::TexParameteri(tpGL::TEXTURE_2D,tpGL::TEXTURE_WRAP_T, tpGL::REPEAT);

		TP_REPORT_GLERROR();
/*
		tpGL::TexParameteri(tpGL::TEXTURE_2D,tpGL::TEXTURE_WRAP_S,
			(m_texture->getWrap().vec[0] == TP_TEXTURE_CLAMP) ? tpGL::CLAMP : tpGL::REPEAT);
		tpGL::TexParameteri(tpGL::TEXTURE_2D,tpGL::TEXTURE_WRAP_T,
			(m_texture->getWrap().vec[1] == TP_TEXTURE_CLAMP) ? tpGL::CLAMP : tpGL::REPEAT);

*/
		TP_REPORT_GLERROR();

		//glEnable(GL_TEXTURE_GEN_S);			// Auto Texture Generation
		//glEnable(GL_TEXTURE_GEN_T);			// Auto Texture Generation

		// LABEL
		//tex_out:

		//tpLogNotify("%s - %3.1fms",__FUNCTION__,t.getElapsed());

	}

	tpVoid onUpdate(tpContext& context)
	{

		TP_REPORT_GLERROR();

		// check the preferred image format
		m_image->setPixelFormat(TP_RGB565);

		// check if the image texture has changed already
		if (m_texture->getImage()->getChangeCount() != m_image->getChangeCount())
		{

			// now convert it
			m_image->convert(*m_texture->getImage());

			// texture and image should be in sync now
			m_image->setChangeCount(m_texture->getImage()->getChangeCount());
		}

		TP_REPORT_GLERROR();

	}
};


class tpGLMeshRenderObject : public tpRenderObject {

	tpRefPtr<tpMesh> m_mesh;
	tpRefPtr<tpGLProgram> m_program;

	GLuint m_vbos[3];

	static tpRefCache<tpGLProgram> m_programcache;

public:

	tpGLMeshRenderObject(tpMesh* mesh) : m_mesh(mesh), m_program(0)
	{
		m_vbos[0] = 0;
	}


	void createVBOs()
	{
		tpGL::glGenBuffers(3,m_vbos);


#define GL_ARRAY_BUFFER                                0x8892
#define GL_ELEMENT_ARRAY_BUFFER                        0x8893
#define GL_STATIC_DRAW                                 0x88E4

		tpGL::glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
		tpGL::glBufferData(GL_ARRAY_BUFFER, m_mesh->getVertices().getSize() * sizeof(GLfloat),m_mesh->getVertices().getData(),GL_STATIC_DRAW);

		tpGL::glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
		tpGL::glBufferData(GL_ARRAY_BUFFER, m_mesh->getNormals().getSize() * sizeof(GLfloat),m_mesh->getNormals().getData(),GL_STATIC_DRAW);

		tpGL::glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2]);
		tpGL::glBufferData(GL_ARRAY_BUFFER, m_mesh->getTexCoords().getSize() * sizeof(GLfloat),m_mesh->getTexCoords().getData(),GL_STATIC_DRAW);

		// bind buffer object for element indices
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]); glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		//numIndices * sizeof(GLushort),indices, GL_STATIC_DRAW);
	}

	void drawVBOs()
	{

		if (m_vbos[0] == 0) createVBOs();

		/* VERTICES */
		tpInt attrib_pos = m_program->getAttributeLocation("a_vertex");
		if (attrib_pos > -1)
		{
			tpGL::glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
			tpGL::glVertexAttribPointer(attrib_pos, 3, tpGL::FLOAT, tpGL::_FALSE, 0, 0);
			tpGL::glEnableVertexAttribArray(attrib_pos);
		}

		TP_REPORT_GLERROR();


		/* NORMALS */
		attrib_pos = m_program->getAttributeLocation("a_normal");
		if (attrib_pos > -1)
		{
			tpGL::glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
			tpGL::glVertexAttribPointer(attrib_pos, 3, tpGL::FLOAT, tpGL::_FALSE, 0, 0);
			tpGL::glEnableVertexAttribArray(attrib_pos);
		}

		TP_REPORT_GLERROR();

		/* TEXTURE COORDINATES */
		attrib_pos = m_program->getAttributeLocation("a_texture_coord");
		if (attrib_pos > -1 && m_mesh->getTexCoordsCount())
		{
			tpGL::glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2]);
			tpGL::glVertexAttribPointer(attrib_pos, 2, tpGL::FLOAT, tpGL::_FALSE, 0, 0);
			tpGL::glEnableVertexAttribArray(attrib_pos);
		}

		TP_REPORT_GLERROR();

		//tpLogMessage("%s Mesh with %d Vertices\t%d Normals\t%d TexCoords",__FUNCTION__,
		//	m_mesh->getVertexCount(),m_mesh->getNormalsCount(),m_mesh->getTexCoordsCount());

		// now draw the whole thing
		tpGL::DrawArrays(m_mesh->getMeshType(),0,m_mesh->getVertexCount());

		tpGL::glBindBuffer(GL_ARRAY_BUFFER, 0);

	}


	void onUpdate(tpContext& context)
	{

		TP_REPORT_GLERROR();


		if (m_mesh->getTexture() && !m_program.isValid())
		{
			tpGLTextureObject* texture_obj = new tpGLTextureObject(m_mesh->getTexture());
			setNestedRenderObject(texture_obj);
		}

		if (getNestedRenderObject()) getNestedRenderObject()->onUpdate(context);


		// check if the program exists
		m_program = m_programcache.retrieve("mesh_shader");

		if (m_program.isValid()) return;




		TP_REPORT_GLERROR();


		m_program = new tpGLProgram;

		tpLogMessage("%s - checking for shader support",__FUNCTION__);

		if (tpGL::get().hasShaderSupport())
		{
			if (tpGL::get().hasShaderCompiler())
			{
				tpLogMessage("%s - shader compiler on board. Using inline shaders.",__FUNCTION__);


				/*
				m_program->setVertexShaderSource(
					"uniform mat4 vProjMat;               \n"
					"uniform mat4 vModelMat;              \n"
					"                                     \n"
					"attribute vec4 vPosition;            \n"
					"attribute vec4 vColour;              \n"
					"                                     \n"
					"varying vec4 outColour;              \n"
					"void main()                          \n"
					"{                                    \n"
					"   outColour = vColour;              \n"
					"   gl_Position = vProjMat * vModelMat * vPosition;          \n"
					"}                                   \n"
					);
				m_program->setFragmentShaderSource(
					"varying vec4 outColour;                   \n"
					"void main()                               \n"
					"{                                         \n"
					" gl_FragColor = outColour;                \n"
					"}                                        \n"
					);
				*/



				tpString path_vs = tpSystem::get()->getExecutablePath(true) + tpPathSep + "shaders" + tpPathSep + "mesh.vert";
				tpString path_fs = tpSystem::get()->getExecutablePath(true) + tpPathSep + "shaders" + tpPathSep + "mesh.frag";;

				tpLogMessage("%s - try to load shaders from \n\t%s\n\t%s",__FUNCTION__,
					path_vs.c_str(), path_fs.c_str() );

				m_program->loadSource(path_vs,tpGLProgram::LOAD_AS_VERTEX_SHADER);
				m_program->loadSource(path_fs,tpGLProgram::LOAD_AS_FRAGMENT_SHADER);

				m_program->compile();
				m_program->link();

				// add the program to the cache

				m_programcache.add("mesh_shader",m_program.get());



			} else {

				if (1 /* FIMG */)
				{
					tpString path_vs = tpSystem::get()->getExecutablePath();
					path_vs << "\\shaders\\mesh_vert_fimg.bin";

					tpString path_fs = tpSystem::get()->getExecutablePath();
					path_fs << "\\shaders\\mesh_frag_fimg.bin";

					tpLogMessage("%s - no shader compiler on board (loading from %s)",
						__FUNCTION__,
						path_vs.c_str());


					m_program->loadBinary(path_vs,tpGLProgram::LOAD_AS_VERTEX_SHADER);
					m_program->loadBinary(path_fs,tpGLProgram::LOAD_AS_FRAGMENT_SHADER);
					m_program->link();
					m_program->bindAttributeLocation("vPosition",0);

				}

			}
		}

		TP_REPORT_GLERROR();

	}

	virtual tpVoid onDrawShader(tpContext& context) {


		tpCamera* camera = context.getCamera();

		tpGLContext& glcontext = static_cast<tpGLContext&>(context);

		//tpGL::glFrontFace(tpGL::CW);


		// HACK!
		if (m_program.isValid() /* && m_program->isReady() */)
		{
			m_program->use();

		} else {

			onUpdate(context);
			return;
		}


		if (getNestedRenderObject()) getNestedRenderObject()->onDraw(context);

		TP_REPORT_GLERROR();

		//tpString t_coords = tpArrayToString(m_mesh->getNormals(),", ");
		//tpLogMessage("%s - %d %s",__FUNCTION__,m_mesh->getNormals().getSize(),t_coords.c_str());

		// Uniforms

		TP_REPORT_GLERROR();

		// texture unit id is always "0" until we support multitexturing
		tpGL::glUniform1i(m_program->getUniformLocation("u_texture"),0);

		//tpGL::glUniform4fv(m_program->getUniformLocation("u_colour"), 1, &col[1]);

		TP_REPORT_GLERROR();

		tpVec4f vec(camera->getPosition(),1.0);
		tpGL::glUniform4fv(m_program->getUniformLocation("u_viewer_pos"),
			1, vec.getData());

		TP_REPORT_GLERROR();

		tpGL::glUniformMatrix4fv(m_program->getUniformLocation("u_projection_mat"),
			1, tpGL::_FALSE, glcontext.getCurrentProjection().getData());

		//tpGL::glUniformMatrix4fv(m_program->getUniformLocation("u_projection_mat"),
		//	1, tpGL::_FALSE, camera->getProjection().getData());


		TP_REPORT_GLERROR();

		tpGL::glUniformMatrix4fv(m_program->getUniformLocation("u_modelview_mat"),
			1, tpGL::_FALSE, glcontext.getCurrentModelView().getData());

		TP_REPORT_GLERROR();

		//////////////////////////////////////////////////////////////////////////
		// Attributes
		//////////////////////////////////////////////////////////////////////////

#if 0

		/* VERTICES */
		tpInt attrib_pos = m_program->getAttributeLocation("a_vertex");
		if (attrib_pos > -1)
		{
			tpGL::glVertexAttribPointer(attrib_pos, 3, tpGL::FLOAT, tpGL::_FALSE, 0, m_mesh->getVertices().getData());
			tpGL::glEnableVertexAttribArray(attrib_pos);
		}

		TP_REPORT_GLERROR();


		/* NORMALS */
		attrib_pos = m_program->getAttributeLocation("a_normal");
		if (attrib_pos > -1)
		{
			tpGL::glVertexAttribPointer(attrib_pos, 3, tpGL::FLOAT, tpGL::_FALSE, 0, m_mesh->getNormals().getData());
			tpGL::glEnableVertexAttribArray(attrib_pos);
		}

		TP_REPORT_GLERROR();

		/* TEXTURE COORDINATES */
		attrib_pos = m_program->getAttributeLocation("a_texture_coord");
		if (attrib_pos > -1 && m_mesh->getTexCoordsCount())
		{
			tpGL::glVertexAttribPointer(attrib_pos, 2, tpGL::FLOAT, tpGL::_FALSE, 0, m_mesh->getTexCoords().getData());
			tpGL::glEnableVertexAttribArray(attrib_pos);
		}

		TP_REPORT_GLERROR();

		//tpLogMessage("%s Mesh with %d Vertices\t%d Normals\t%d TexCoords",__FUNCTION__,
		//	m_mesh->getVertexCount(),m_mesh->getNormalsCount(),m_mesh->getTexCoordsCount());

		// now draw the whole thing
		tpGL::DrawArrays(m_mesh->getMeshType(),0,m_mesh->getVertexCount());

#else
		drawVBOs();
#endif

//		tpGL::glUseProgram(0);

		TP_REPORT_GLERROR();

		//getchar();

	}

	virtual tpVoid onDrawFixedFunc(tpContext& context) {

		tpGL::EnableClientState(tpGL::VERTEX_ARRAY);
		tpGL::VertexPointer(3,tpGL::FLOAT,0,m_mesh->getVertices().getData());

		if (m_mesh->getNormals().getSize())
		{
			//tpLogNotify("%s %d normals",__FUNCTION__,m_mesh->getNormals().getSize());
			tpGL::EnableClientState(tpGL::NORMAL_ARRAY);
			tpGL::NormalPointer(tpGL::FLOAT,0,m_mesh->getNormals().getData());
		}

		if (m_mesh->getTexCoords().getSize())
		{
			//tpLogNotify("%s %d texcoords",__FUNCTION__,m_mesh->getTexCoords().getSize());
			tpGL::EnableClientState(tpGL::TEXTURE_COORD_ARRAY);
			//float tex[] = {0,0, 0,1, 1,0, 1,1};
			tpGL::TexCoordPointer(2, tpGL::FLOAT, 0, m_mesh->getTexCoords().getData());
		}

		tpGL::DrawArrays(m_mesh->getMeshType(),0,m_mesh->getVertexCount());

		tpGL::DisableClientState(tpGL::VERTEX_ARRAY);

		if (m_mesh->getNormals().getSize())
		{
			tpGL::DisableClientState(tpGL::NORMAL_ARRAY);
		}


		if (m_mesh->getTexCoords().getSize())
		{
			tpGL::DisableClientState(tpGL::TEXTURE_COORD_ARRAY);
		}
	}

	void onDraw(tpContext& context) {

		// HACK!
		tpGL::get().hasShaderSupport() ?
			onDrawShader(context) :
			onDrawFixedFunc(context);
	}

};


tpRefCache<tpGLProgram> tpGLMeshRenderObject::m_programcache;


void tpGLTraverser::applyMaterial(tpNode* node)
{

	//tpGeometry* geom = dynamic_cast<tpGeometry*>(node);

	//tpMaterial *material = (geom) ? geom->getMaterial() : NULL;

	//if (!material) return;

	//tpGL::Materialfv(tpGL::FRONT_AND_BACK,tpGL::AMBIENT,material->getAmbientColor().getData());
	//tpGL::Materialfv(tpGL::FRONT_AND_BACK,tpGL::DIFFUSE,material->getDiffuseColor().getData());
	//tpGL::Materialfv(tpGL::FRONT_AND_BACK,tpGL::SPECULAR,material->getSpecularColor().getData());
	//tpGL::Materialfv(tpGL::FRONT_AND_BACK,tpGL::EMISSION,material->getEmissiveColor().getData());

	//tpGL::Materialf(tpGL::FRONT_AND_BACK,tpGL::SHININESS,material->getShininess());

};

tpVoid tpGLTraverser::preAction( tpNode* node, tpContext& context )
{

	if (node && false == node->getDepthTest() )
	{
		tpGL::Disable(tpGL::DEPTH_TEST);
	}

	tpGLContext& glcontext = static_cast<tpGLContext&>(context);
	if (node->isOfType(tpTransform::getTypeInfo()))
	{
		tpTransform* transform = static_cast<tpTransform*>(node);
		glcontext.pushAndMulModelView(transform->getMatrix());
	}
	if (node->isOfType(tpProjection::getTypeInfo()))
	{
		tpProjection* obj = static_cast<tpProjection*>(node);
		glcontext.pushProjection(obj->getProjectionMatrix());
		glcontext.pushAndMulModelView(obj->getModelViewMatrix());
	}


	if (node->isOfType(tpPrimitive::getTypeInfo()))
	{
		tpPrimitive* mesh = static_cast<tpPrimitive*>(node);

		tpRenderObject* robj = mesh->getRenderObject();

		if (0 == robj) {

			robj = new tpGLMeshRenderObject(mesh);
			mesh->setRenderObject(robj);
		}

		robj->onDraw(context);
	}

	if (node->isOfType(tpTransform::getTypeInfo()))
	{
		//tpTransform* transform = static_cast<tpTransform*>(node);
		//tpGL::PushMatrix();
		//tpGL::MultMatrixf(static_cast<tpTransform*>(node)->getMatrix().getData());
	}

}

tpVoid tpGLTraverser::postAction( tpNode* node, tpContext& context )
{

	if ( node && false == node->getDepthTest() )
	{
		tpGL::Enable(tpGL::DEPTH_TEST);
	}

	tpGLContext& glcontext = static_cast<tpGLContext&>(context);
	if (node->isOfType(tpTransform::getTypeInfo())) glcontext.popModelView();
	if (node->isOfType(tpProjection::getTypeInfo())) {
		glcontext.popProjection();
		glcontext.popModelView();
	}

	//if (node->isOfType(tpGeometry::getTypeInfo())) tpGL::Disable(tpGL::TEXTURE_2D);
	//if (node->isOfType(tpTransform::getTypeInfo())) tpGL::PopMatrix();
}

tpGLTraverser::~tpGLTraverser()
{
}

//////////////////////////////////////////////////////////////////////////





#endif
