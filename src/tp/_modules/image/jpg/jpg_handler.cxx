// includes from libTAP
#include <tp/imagehandler.h>
#include <tp/image.h>
#include <tp/log.h>
#include <tp/module.h>
#include <tp/version.h>
#include <tp/file.h>

#include <string.h>


// need to force C linking
extern "C" {
#include "jpeglib.h"
};


#define TP_JPEG_QUALITY 75

tpImage* tpJPEGLoader(const tpString& filename)
{

    struct  jpeg_decompress_struct cinfo;      
    struct  jpeg_error_mgr jerr;          

    tpUChar *_ptr;
    tpULong _imgsize;                      

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

	tpLogNotify("%s - %s",__FUNCTION__,filename.c_str());

    FILE *_file=fopen(filename.c_str(), "rb");

	if (!_file) return 0;


    jpeg_stdio_src(&cinfo, _file);
    
    jpeg_read_header(&cinfo, TRUE);
    cinfo.out_color_space = JCS_RGB;
    
    jpeg_start_decompress ( &cinfo );
    
	tpUShort pix_format = tpPixelFormat::kRGB_888;

	tpImage* img = new tpImage();
	//img->create(0,cinfo.image_width,cinfo.image_height,TP_RGB);
	img->allocate(cinfo.image_width,cinfo.image_height, tpPixelFormat::kRGB_888);
        
    _imgsize = cinfo.image_width * cinfo.image_height * 3;
       
    tpUInt width = cinfo.image_width;
    tpUInt height = cinfo.image_height;
        
    _ptr = (tpUByte*)img->getData();
    
    unsigned stride = cinfo.output_width * 3;

    JSAMPARRAY tempbuf = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, stride, 1 );

    
    while ( cinfo.output_scanline < cinfo.output_height )
    {
		jpeg_read_scanlines( &cinfo, tempbuf, 1 );
		memcpy( _ptr, tempbuf[0], stride );
		_ptr += stride;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose (_file);

	return img;
};


bool tpJPEGLoader_Save(const tpString& filename,const tpImage* img)
{
	if (!img || !img->isValid())
	{
		tpLogError("can't save '%s' - image or data invalid",filename.c_str());
		return false;
	}

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr       jerr;
	
    jpeg_create_compress(&cinfo);

	FILE* file = fopen(filename.c_str(),"wb");

	//tpFile file;
	//if (file.open(filename),"wb")
	if (file)
	{
		
		cinfo.err = jpeg_std_error(&jerr); //Use default error handling (ugly!)
	    jpeg_stdio_dest(&cinfo,file);
    
	    cinfo.image_width      = img->getWidth();  //Image width and height, in pixels
	    cinfo.image_height     = img->getHeight();

		tpLogNotify("%s compressing image sized %dx%d",__FUNCTION__,img->getWidth(),img->getHeight());

	    int stride = 0;//img->getWidth() * img->get
                         
			
		switch (img->getPixelFormat())
		{
			case tpPixelFormat::kRGB_888:
				stride = cinfo.image_width * 3;
				cinfo.input_components = 3;              //Color components per pixel
		        cinfo.in_color_space   = JCS_RGB;      //Colorspace of input image
				break;
				
			case tpPixelFormat::kRGBA_8888:
				stride = cinfo.image_width * 4;
				cinfo.input_components = 3;              //Color components per pixel
				cinfo.in_color_space   = JCS_RGB;        //Colorspace of input image
				break;

			case tpPixelFormat::kGray_8:
				stride = cinfo.image_width;
				cinfo.input_components = 1;              //Color components per pixel
		        cinfo.in_color_space   = JCS_GRAYSCALE;      //Colorspace of input image
				break;
		
			case tpPixelFormat::kYUV_420:
				stride = cinfo.image_width * 3;
				cinfo.input_components = 2;              //Color components per pixel
		        cinfo.in_color_space   = JCS_YCbCr;      //Colorspace of input image
				break;
			default:
				break;
		}
		jpeg_set_defaults(&cinfo);
	    jpeg_set_quality(&cinfo,
	                     TP_JPEG_QUALITY, //Quality: 0-100 scale
	                     TRUE);    //Limit to baseline-JPEG values
	    jpeg_start_compress(&cinfo, TRUE);

	    JSAMPROW row_pointer[1];
	    JSAMPLE *image_buffer = (JSAMPLE*)img->getData();
	    while (cinfo.next_scanline < cinfo.image_height) 
		{
	        row_pointer[0] = &image_buffer[cinfo.next_scanline * stride];
	        jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	    }

	    jpeg_finish_compress(&cinfo); //Always finish
	
		fclose(file);
	}

    jpeg_destroy_compress(&cinfo); //Free resources

	return TRUE;
};


class tpImageFactoryJPEG : public tpImageHandler {

public:

	TP_TYPE_DECLARE;

	tpImageFactoryJPEG() : tpImageHandler() 
	{
		tpLogNotify("%s JPEG image support (libjpeg %d)",tpGetVersionString(),JPEG_LIB_VERSION);
	}

	bool getCapability(tpUInt capability,const tpString& name)
	{
		switch (capability)
		{
		case kCanRead:
			return name.afterLast('.') == "jpg" || name.afterLast('.') == "jpeg";
			break;

		case kCanWrite:
			return name.afterLast('.') == "jpg" || name.afterLast('.') == "jpeg";
			break;
		}

		return false;
	}

	virtual tpImage* read(const tpString& name)
	{
		tpImage* img = tpJPEGLoader(name);

		return img;
	}

	virtual bool write(const tpImage* img, const tpString& name)
	{
		return tpJPEGLoader_Save(name,img);
	}
};


TP_TYPE_REGISTER(tpImageFactoryJPEG,tpImageHandler,ImageFactoryJPEG);

//TP_MODULE_HOOK(ImageJPEG)

tpModuleInitializer<tpImageFactoryJPEG> g_imagefactory_jpeg;

