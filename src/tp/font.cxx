#include <tp/font.h>

#define STB_TRUETYPE_IMPLEMENTATION

#define STBTT_malloc(x,u)  malloc(x)
#define STBTT_free(x,u)    free(x)

#include <stb_truetype/stb_truetype.h>

struct tpFontRasterizer {

	stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs

	tpImage* onLoad(const tpString& name) {

		tpFile ffile;

		if (ffile.open(name,"rb")) {

			tpImage* image = new tpImage();
			image->allocate(512,512,tpPixelFormat::kGray_8);

			if (name.afterLast('.') == "ttc") {

			}

			tpArray<unsigned char> buffer;
			buffer.resize(1<<20);

			ffile.read((char*)&buffer[0],buffer.getSize());

			stbtt_BakeFontBitmap( &buffer[0],
								  0, 32.0, (tpUChar*)image->getData(),image->getWidth(),image->getHeight(), 32,96, cdata);

			return image;
		}

		return 0;
	}

	void onText(const tpString& text,tpPrimitive& prim) {

		prim.clearAll();
		prim.setAttributes(tpPrimitive::kAttributeVertex|tpPrimitive::kAttributeUV);

		for (tpSizeT i = 0; i < text.getLength(); ++i) {
			stbtt_aligned_quad q;
			float x(0),y(0);
			stbtt_GetBakedQuad(cdata, 512 ,512, text.c_str()[i]-32, &x,&y,&q,1);//1=opengl,0=old d3d

			tpVec3f normal(0,0,1);
#define TP_ALLOW_QUAD 1
#if TP_ALLOW_QUAD
			prim.setPrimitiveType(tpPrimitive::kQuads);
			prim.addVertex(tpVec3f(q.x0,q.y0,0),normal,tpVec2f(q.s0,q.t1)); // v0
			prim.addVertex(tpVec3f(q.x1,q.y0,0),normal,tpVec2f(q.s1,q.t1)); // v1
			prim.addVertex(tpVec3f(q.x1,q.y1,0),normal,tpVec2f(q.s1,q.t0)); // v2
			prim.addVertex(tpVec3f(q.x0,q.y1,0),normal,tpVec2f(q.s0,q.t0)); // v3
#else

			prim.setPrimitiveType(tpPrimitive::kQuads);

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

	mImage = mRasterizer->onLoad(name);
}

void
tpFont::text(const tpString& text,tpPrimitive& p) {
	mRasterizer->onText(text,p);
	// add texture
}

tpImage *
tpFont::getImage()
{
	return mImage.get();
}

