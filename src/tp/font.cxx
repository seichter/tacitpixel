/*
 * Copyright (C) 1999-2013 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <tp/font.h>
#include <tp/log.h>

#define STB_TRUETYPE_IMPLEMENTATION

#define STBTT_malloc(x,u)  malloc(x)
#define STBTT_free(x,u)    free(x)

#include <stb_truetype/stb_truetype.h>

class tpFontRasterizerStb : public tpFontRasterizer {
public:

	tpRefPtr<tpImage> mImage;

	tpImage* getImage() {
		return mImage.get();
	}

	tpFontRasterizerStb()
	{
	}

	~tpFontRasterizerStb()
	{
	}

	stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs

	bool onLoad(const tpString& name) {

		tpFile ffile;

		if (ffile.open(name,"rb")) {

			mImage = new tpImage();
			mImage->allocate(512,512,tpPixelFormat::kGray_8);

			if (name.afterLast('.') == "ttc") {
                tpLogMessage("TTC files not supported out of the box");
			}

			tpArray<unsigned char> buffer;
			buffer.resize(1<<20);

			ffile.read((char*)&buffer[0],buffer.getSize());

			stbtt_BakeFontBitmap( &buffer[0],
                                  0, 12,
                    (tpUChar*)mImage->getData(),
                    mImage->getWidth(),
                    mImage->getHeight(),
                    32,
                    96,
                    cdata);

			return true;
		}
		return false;
	}

	void onText(const tpString& text,tpPrimitive& prim) 
	{
		if (!mImage.isValid() || !mImage->isValid()) {
		
			tpLogError("tpFontRasterizer - no font loaded");
			return;
		}

        prim.clear();

		float x(0),y(0);

        for (tpSizeT i = 0; i < text.getLength(); ++i)
        {
            const tpChar c = text.c_str()[i];

            switch(c) {
            case '\n':
                y += 10;
                x = 0;
                continue;
            case '\r':
                y += 10;
                continue;
            }


			stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata, getImage()->getWidth() ,getImage()->getHeight(), c-32, &x,&y,&q,1);//1=opengl,0=old d3d

//			float h = q.t1 - q.t0;

			tpVec3f normal(0,0,1);

			// GLES doesn't like Quads!
//#define TP_ALLOW_QUAD 1
#if TP_ALLOW_QUAD

			prim.setPrimitiveType(tpPrimitive::kQuads);

			// unlike the example the tex-coordinates are upside down and
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
			prim.addVertexNormalTextureCoordinate(tpVec3f(q.x0,-q.y0,0),normal,tpVec2f(q.s0,q.t0)); // v0
			prim.addVertexNormalTextureCoordinate(tpVec3f(q.x1,-q.y0,0),normal,tpVec2f(q.s1,q.t0)); // v1
			prim.addVertexNormalTextureCoordinate(tpVec3f(q.x1,-q.y1,0),normal,tpVec2f(q.s1,q.t1)); // v2

			prim.addVertexNormalTextureCoordinate(tpVec3f(q.x0,-q.y0,0),normal,tpVec2f(q.s0,q.t0)); // v0
			prim.addVertexNormalTextureCoordinate(tpVec3f(q.x1,-q.y1,0),normal,tpVec2f(q.s1,q.t1)); // v2
			prim.addVertexNormalTextureCoordinate(tpVec3f(q.x0,-q.y1,0),normal,tpVec2f(q.s0,q.t1)); // v3

#endif


		}
	}

};

tpFont::tpFont()
	: tpReferenced()
	, mRasterizer(new tpFontRasterizerStb())
{}

bool
tpFont::load(const tpString& name)
{
	return mRasterizer->onLoad(name);
}

void
tpFont::text(const tpString& text,tpPrimitive& p) {

    if (0 == p.getTexture())
    {
        tpTexture* rasterfont = new tpTexture();
        rasterfont->setImage(getImage());
		rasterfont->setFormat(tpTexture::kFormatAlpha);
		p.clearRenderFlags();
        p.addRenderFlag(tpRenderFlag::kBlending,
                        tpRenderFlag::kBlendSrcAlpha,
                        tpRenderFlag::kBlendOneMinusAlpha);
		p.addRenderFlag(tpRenderFlag::kLighting);

//        p.setMaterial(0);
        p.setTexture(rasterfont);
    }

	mRasterizer->onText(text,p);

}

tpImage *
tpFont::getImage()
{
	return (mRasterizer.isValid()) ? mRasterizer->getImage() : 0;
}


