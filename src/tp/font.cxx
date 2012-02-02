#include <tp/font.h>

#define STB_TRUETYPE_IMPLEMENTATION

#define STBTT_malloc(x,u)  malloc(x)
#define STBTT_free(x,u)    free(x)

#include <stb_truetype/stb_truetype.h>

tpFont::tpFont() : tpReferenced() {}

bool
tpFont::load(const tpString& name) {

	tpFile ffile;
	if (ffile.open(name,"r")) {

		mImage = new tpImage();
		mImage->allocate(512,512,tpPixelFormat::kGray_8);

		unsigned char ttf_buffer[1<<20];
		ffile.read((char*)ttf_buffer,1<<20);

		stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
		stbtt_BakeFontBitmap( &ttf_buffer[0], 0, 32.0, (tpUChar*)mImage->getData(),mImage->getWidth(),mImage->getHeight(), 32,96, cdata);

		return true;
	}

	return false;
}

tpImage *
tpFont::getImage()
{
	return mImage.get();
}

