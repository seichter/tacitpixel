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


#include <tp/image.h>
#include <tp/imagehandler.h>
#include <tp/pixelformat.h>

#include <tp/log.h>
#include <tp/module.h>
#include <tp/version.h>
#include <tp/file.h>

#include <libpng/png.h>

class tpImageHandler_PNG : public tpImageHandler
{
public:

    TP_TYPE_DECLARE

	bool getCapability(tpUInt capability,const tpString& name);

	tpImageHandler_PNG();

    virtual tpImage* read(const tpString& name) const;

    virtual bool write(const tpImage *img, const tpString &name) const;
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

	tpLogNotify("PNG support can't read %s",name.c_str());

	return false;
}



tpImage*
tpImageHandler_PNG::read( const tpString& name ) const
{
    tpLogNotify("%s loading %s",__FUNCTION__,name.c_str());
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;

	png_byte header[8];

	tpFile file;
	if (!file.open(name,"rb")) return 0;

	FILE* fp = (FILE*)file.getHandle();

	fread(header, 1, 8, fp);

	if (png_sig_cmp(header, 0, 8)) return 0;

	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);


	if (!png_ptr) return 0;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) return 0;

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	int width = png_get_image_width(png_ptr, info_ptr);
	int height = png_get_image_height(png_ptr, info_ptr);
	int color_type = png_get_color_type(png_ptr, info_ptr);
	int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (int y=0; y<height; y++)
		  row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));


	tpLogNotify("PNG row stride %d",png_get_rowbytes(png_ptr,info_ptr));

	png_read_image(png_ptr, row_pointers);

	tpRefPtr<tpImage> image = new tpImage();

	switch(png_get_color_type(png_ptr, info_ptr))
	{
	case PNG_COLOR_TYPE_RGB:
		image->allocate(width,height,tpPixelFormat::kRGB_888);
		image->assign(row_pointers[0]);
		break;
	case PNG_COLOR_TYPE_RGBA:
		image->allocate(width,height,tpPixelFormat::kRGBA_8888);
		image->assign(&row_pointers[0]);
		break;
	case PNG_COLOR_TYPE_GRAY:
		image->allocate(width,height,tpPixelFormat::kGray_8);
		image->assign(row_pointers[0]);
		break;
	default:
		image = 0;
		break;
	}

	return image.release();
}

bool
tpImageHandler_PNG::write(const tpImage* img, const tpString& name) const
{

	/* create file */
	FILE *fp = fopen(name.c_str(), "wb");
	if (!fp) return false;

	/* initialize stuff */
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_init_io(png_ptr, fp);

	int bit_depth = 8; // in libpng bit_depths is per channel
	png_byte color_type = 0;

	switch (img->getPixelFormat())
	{
	case tpPixelFormat::kGray_8:
		color_type=PNG_COLOR_TYPE_GRAY;
		break;
	case tpPixelFormat::kRGB_888:
		color_type=PNG_COLOR_TYPE_RGB;
		break;
	case tpPixelFormat::kRGBA_8888:
		color_type=PNG_COLOR_TYPE_RGB_ALPHA;
		break;
	}

	tpLogNotify("PNG %dx%d@%d",img->getWidth(),img->getHeight(),bit_depth);


	png_set_IHDR(png_ptr, info_ptr, img->getWidth(), img->getHeight(),
				 bit_depth, color_type, PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_bytepp row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * img->getHeight());
	for (int y=0; y<img->getHeight(); y++)
		  row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));


	tpLogNotify("%d == %d",sizeof(png_bytep) * img->getHeight() * img->getHeight(),img->getDataSize());

	//memcpy(row_pointers,img->getData(),img->getDataSize());


	png_write_info(png_ptr, info_ptr);

	png_bytep img_ptr = (png_bytep)img->getData();
	png_write_image(png_ptr, row_pointers );

	png_write_end(png_ptr, info_ptr);

	//!wrong
	free(row_pointers);

	fclose(fp);

	return false;
}

TP_TYPE_REGISTER(tpImageHandler_PNG,tpImageHandler,ImageFactoryPNG);
tpModuleInitializer<tpImageHandler_PNG> g_imagefactory_png;

