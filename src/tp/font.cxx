#include <tp/font.h>
#include <tp/log.h>

#define STB_TRUETYPE_IMPLEMENTATION

#define STBTT_malloc(x,u)  malloc(x)
#define STBTT_free(x,u)    free(x)

#include <stb_truetype/stb_truetype.h>

struct tpFontRasterizer {

	tpRefPtr<tpImage> mImage;

	tpImage* getImage() {
		return mImage.get();
	}

	stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs

	void onLoad(const tpString& name) {

		tpFile ffile;

		if (ffile.open(name,"rb")) {

			mImage = new tpImage();
			mImage->allocate(512,512,tpPixelFormat::kGray_8);

			if (name.afterLast('.') == "ttc") {

			}

			tpArray<unsigned char> buffer;
			buffer.resize(1<<20);

			ffile.read((char*)&buffer[0],buffer.getSize());

			stbtt_BakeFontBitmap( &buffer[0],
								  0, 32.0, (tpUChar*)mImage->getData(),mImage->getWidth(),mImage->getHeight(), 32,96, cdata);
		}
	}

	void onText(const tpString& text,tpPrimitive& prim) {

		prim.clearAll();
		prim.setAttributes(tpPrimitive::kAttributeVertex|tpPrimitive::kAttributeNormals|tpPrimitive::kAttributeUV);

		for (tpSizeT i = 0; i < text.getLength(); ++i) {
			stbtt_aligned_quad q;
			float x(0),y(0);
			stbtt_GetBakedQuad(cdata, 512 ,512, text.c_str()[i]-32, &x,&y,&q,1);//1=opengl,0=old d3d

			tpVec3f normal(0,0,1);

			// GLES doesn't like Quads!
#define TP_ALLOW_QUAD 1
#if TP_ALLOW_QUAD

			//
//			glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y0);
//			glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y0);
//			glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y1);
//			glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y1);

//			res->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(0,0,0,1)); // v0
//			res->addVertex(tpVec3r(1,0,0),tpVec3r(0,0,1),tpVec2r(1,1),tpVec4r(1,0,0,1)); // v1
//			res->addVertex(tpVec3r(0,1,0),tpVec3r(0,0,1),tpVec2r(0,0),tpVec4r(0,1,0,1)); // v3
//			res->addVertex(tpVec3r(1,1,0),tpVec3r(0,0,1),tpVec2r(1,0),tpVec4r(1,1,1,1)); // v2


			prim.setPrimitiveType(tpPrimitive::kQuads);
			prim.addVertex(tpVec3f(q.x0,q.y0,0),normal,tpVec2f(q.s0,q.t1)); // v0
			prim.addVertex(tpVec3f(q.x1,q.y0,0),normal,tpVec2f(q.s1,q.t1)); // v1
			prim.addVertex(tpVec3f(q.x1,q.y1,0),normal,tpVec2f(q.s1,q.t0)); // v2
			prim.addVertex(tpVec3f(q.x0,q.y1,0),normal,tpVec2f(q.s0,q.t0)); // v3
#else

			prim.setPrimitiveType(tpPrimitive::kTriangles);

			prim.addVertex(tpVec3f(q.x0,q.y0,0),normal,tpVec2f(q.s0,q.t1)); // v0
			prim.addVertex(tpVec3f(q.x1,q.y0,0),normal,tpVec2f(q.s1,q.t1)); // v1
			prim.addVertex(tpVec3f(q.x1,q.y1,0),normal,tpVec2f(q.s1,q.t0)); // v2

			prim.addVertex(tpVec3f(q.x0,q.y0,0),normal,tpVec2f(q.s0,q.t1)); // v0
			prim.addVertex(tpVec3f(q.x1,q.y1,0),normal,tpVec2f(q.s1,q.t0)); // v2
			prim.addVertex(tpVec3f(q.x0,q.y1,0),normal,tpVec2f(q.s0,q.t0)); // v3

#endif


		}
	}
};

tpFont::tpFont()
	: tpReferenced()
	, mRasterizer(new tpFontRasterizer())
{}

bool
tpFont::load(const tpString& name) {

	mRasterizer->onLoad(name);
}

void
tpFont::text(const tpString& text,tpPrimitive& p) {

	mRasterizer->onText(text,p);

	tpTexture* rasterfont = new tpTexture();
	rasterfont->setFormat(tpTexture::kFormatAlpha);
	rasterfont->setImage(getImage());

	p.setTexture(rasterfont);
}

tpImage *
tpFont::getImage()
{
	return (mRasterizer.isValid()) ? mRasterizer->getImage() : 0;
}

