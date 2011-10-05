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


#include <tp/imagehandler.h>
#include <tp/image.h>

#include <tp/log.h>
#include <tp/module.h>
#include <tp/version.h>
#include <tp/file.h>

#include <png.h>

class tpImageHandler_PNG : public tpImageHandler
{
public:

	TP_TYPE_DECLARE;

	bool getCapability(tpUInt capability,const tpString& name);

	tpImageHandler_PNG();

	virtual tpImage* read(const tpString& name);

	virtual bool write(const tpImage *img, const tpString &name);
};

//////////////////////////////////////////////////////////////////////////

tpImageHandler_PNG::tpImageHandler_PNG()
	: tpImageHandler()
{
	tpLogNotify("%s PNG image support (libpng %s)",tpGetVersionString(),PNG_LIBPNG_VER_STRING);
}

bool
tpImageHandler_PNG::getCapability( tpUInt capability,const tpString& name )
{
	switch (capability)
	{
	case kCanRead:
		return name.afterLast('.') == "png";
		break;

	case kCanWrite:
		return name.afterLast('.') == "png";
		break;
	}
	return false;
}



tpImage*
tpImageHandler_PNG::read( const tpString& name )
{
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;

	png_byte header[8];

	tpFile file;
	file.open(name,"rb");

	if (file.getState() != file.isGood()) return 0;

	tpLogNotify("reading PNG file");

	FILE* fp = (FILE*)file.getHandle();

	fread(header, 1, 8, fp);

	if (png_sig_cmp(header, 0, 8)) return 0;

	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);


	if (!png_ptr) return 0;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) return 0;

	if (setjmp(png_jmpbuf(png_ptr))) return 0;

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	int width = png_get_image_width(png_ptr, info_ptr);
	int height = png_get_image_height(png_ptr, info_ptr);
	int color_type = png_get_color_type(png_ptr, info_ptr);
	int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	/* read file */
	if (setjmp(png_jmpbuf(png_ptr))) return 0;

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (int y=0; y<height; y++)
		  row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

	png_read_image(png_ptr, row_pointers);

	//fclose(fp);
	tpRefPtr<tpImage> image = new tpImage();

	switch(png_get_color_type(png_ptr, info_ptr))
	{
	case PNG_COLOR_TYPE_RGB:
		image->allocate(width,height,tpPixelFormat::kRGB888);
		memcpy(image->getData(),row_pointers,image->getDataSize());
		break;
	case PNG_COLOR_TYPE_RGBA:
		image->allocate(width,height,tpPixelFormat::kRGBA8888);
		memcpy(image->getData(),row_pointers,image->getDataSize());
		break;
	default:
		image = 0;
		break;
	}
	return image.release();
}

bool
tpImageHandler_PNG::write(const tpImage* img, const tpString& name)
{

	/* create file */
	FILE *fp = fopen(name.c_str(), "wb");
	if (!fp) return false;
			//abort_("[write_png_file] File %s could not be opened for writing", file_name);

	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;

	/* initialize stuff */
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr) return false;
			// abort_("[write_png_file] png_create_write_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) return false;
			//abort_("[write_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr))) return false;
			//abort_("[write_png_file] Error during init_io");

	png_init_io(png_ptr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr))) return false;
			//abort_("[write_png_file] Error during writing header");


	int bit_depth = tpPixelFormat::getBitsPerPixel(img->getPixelFormat());
	int color_type = 0;

	switch (img->getPixelFormat())
	{
	case tpPixelFormat::kRGB888:
		color_type=PNG_COLOR_TYPE_RGB;
		break;
	}


	png_set_IHDR(png_ptr, info_ptr, img->getWidth(), img->getHeight(),
				 bit_depth, color_type, PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr))) return false;
			//abort_("[write_png_file] Error during writing bytes");

	png_bytepp img_ptr = static_cast<png_bytepp>(const_cast<void*>(img->getData()));
	png_write_image(png_ptr, img_ptr );


	/* end write */
	if (setjmp(png_jmpbuf(png_ptr))) return false;
			//abort_("[write_png_file] Error during end of write");

	png_write_end(png_ptr, NULL);

//	/* cleanup heap allocation */
//	for (int y=0; y<img->getHeight(); y++)
//			free(row_pointers[y]);
//	free(row_pointers);

	fclose(fp);

	return false;
}

TP_TYPE_REGISTER(tpImageHandler_PNG,tpImageHandler,ImageFactoryNull);

tpModuleInitializer<tpImageHandler_PNG> g_imagefactory_png;

