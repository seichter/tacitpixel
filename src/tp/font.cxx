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
			mImage->allocate(256,156,tpPixelFormat::kGray_8);

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

		float x(0),y(0);

		for (tpSizeT i = 0; i < text.getLength(); ++i) {
			stbtt_aligned_quad q;
			stbtt_GetBakedQuad(cdata, getImage()->getWidth() ,getImage()->getHeight(), text.c_str()[i]-32, &x,&y,&q,1);//1=opengl,0=old d3d

			float h = q.t1 - q.t0;

			// hack! this should be parameterized
//			x-=1;

			tpVec3f normal(0,0,1);

			// GLES doesn't like Quads!
//#define TP_ALLOW_QUAD 1
#if TP_ALLOW_QUAD

			tpLogMessage("T: %3.3f %3.3f",q.t0,q.t1);

			prim.setPrimitiveType(tpPrimitive::kQuads);

//			prim.addVertex(tpVec3f(q.x0,q.y0,0),normal,tpVec2f(q.s0,q.t1)); // v0
//			prim.addVertex(tpVec3f(q.x1,q.y0,0),normal,tpVec2f(q.s1,q.t1)); // v1
//			prim.addVertex(tpVec3f(q.x1,q.y1,0),normal,tpVec2f(q.s1,q.t0)); // v2
//			prim.addVertex(tpVec3f(q.x0,q.y1,0),normal,tpVec2f(q.s0,q.t0)); // v3

			// unlike the suggested version tex-coordinates are upside down and
			// we move the vertices into positive y direction - check with
			// stb_truetype 0.5
			prim.addVertex(tpVec3f(q.x0,-q.y0,0),normal,tpVec2f(q.s0,q.t0)); // v0
			prim.addVertex(tpVec3f(q.x1,-q.y0,0),normal,tpVec2f(q.s1,q.t0)); // v1
			prim.addVertex(tpVec3f(q.x1,-q.y1,0),normal,tpVec2f(q.s1,q.t1)); // v2
			prim.addVertex(tpVec3f(q.x0,-q.y1,0),normal,tpVec2f(q.s0,q.t1)); // v3


#else

			// also here swapped t component of tex-coords and moved vertices into
			// positive y - direction
			prim.setPrimitiveType(tpPrimitive::kTriangles);
			prim.addVertex(tpVec3f(q.x0,-q.y0,0),normal,tpVec2f(q.s0,q.t0)); // v0
			prim.addVertex(tpVec3f(q.x1,-q.y0,0),normal,tpVec2f(q.s1,q.t0)); // v1
			prim.addVertex(tpVec3f(q.x1,-q.y1,0),normal,tpVec2f(q.s1,q.t1)); // v2

			prim.addVertex(tpVec3f(q.x0,-q.y0,0),normal,tpVec2f(q.s0,q.t0)); // v0
			prim.addVertex(tpVec3f(q.x1,-q.y1,0),normal,tpVec2f(q.s1,q.t1)); // v2
			prim.addVertex(tpVec3f(q.x0,-q.y1,0),normal,tpVec2f(q.s0,q.t1)); // v3

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

