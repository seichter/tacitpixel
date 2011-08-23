/* 
 * Copyright (C) 1999-2011 Hartmut Seichter
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

#include <tp/image.h>
#include <tp/log.h>

#include <tp/utils.h>
#include <tp/factory.h>
#include <tp/module.h>
#include <tp/system.h>



inline static tpUShort tpGetBitsPerPixel(const tpUByte& format)
{
    switch (format) {
		case TP_RGB888:
		case TP_BGR888:
			return 24;
		case TP_RGBA8888:
		case TP_BGRA8888:
			return 32;
//		case TP_YUV422:
//		case TP_YUV444:
		case TP_YUV420:
			return 12;
		case TP_RGB565:
		case TP_RGB555:
			return 16;
		case TP_GREY8:
			return 8;
		default:
			tpLogError("%s: format not supported",__FUNCTION__);
			break;
	};
	return 0;
}

tpImage::tpImage() : m_width(0), m_height(0), m_pixelformat(0), m_changecount(0)
{
}

tpImage::~tpImage() 
{
}


void 
tpImage::allocate(unsigned int w, unsigned int h, tpUByte pixelformat )
{
	unsigned int size = (w * h * tpGetBitsPerPixel(pixelformat)) >> 3;
	m_data.setSize(size);
	if (size) 
	{
		m_width = w;
		m_height = h;
		m_pixelformat = pixelformat;
	}
}

void 
tpImage::copy(const void* data)
{
	m_data.copy(data);
}

tpImageHandler::tpImageHandler() : tpReferenced()
{

}

tpImageHandler::~tpImageHandler()
{
}

//
// Global Image Cache
// 
//tpRefCache<tpImage> g_imagecache;

tpImage* 
tpImage::read( const tpString& name )
{
	tpImage* result(0);

	const tpModuleList& ml = tpModuleManager::get()->getModules();

	for (tpSizeT i = 0; i < ml.getSize(); i++)
	{
		const tpRefPtr<tpReferenced>& item = ml[i];

		if (item->getType()->isOfType(tpImageHandler::getTypeInfo()))
		{
			//tpLogNotify("%s found %s",__FUNCTION__,item->getType()->getName().c_str());

			tpRefPtr<tpImageHandler> nf = static_cast<tpImageHandler*>(item.get());

			if (nf.isValid() && nf->getCapability(TP_IMAGE_CAN_READ,name))
			{

				tpString filename = tpSystem::get()->findFile(name);

				//result = reinterpret_cast<tpImage*>(g_imagecache.retrieve(filename));

				if (!result)
				{

					//tpLogNotify("%s using %s",__FUNCTION__,item->getType()->getName().c_str());

					result = nf->read(filename);

					if (result && result->isValid()) {
						//result->setFileName(filename);
						//g_imagecache.add(filename,result);

					}

					//tpLogNotify("%s - 0x%x",__FUNCTION__,result);
				} else {

					//tpLogNotify("%s cached '%s' (0x%x)",__FUNCTION__,filename.c_str(),result);

				}
			}
		}
	}

	return result;

}

bool 
tpImage::write( const tpString& name ) const
{
	bool result(false);

	const tpModuleList& ml = tpModuleManager::get()->getModules();

	for (tpSizeT i = 0; i < ml.getSize(); i++)
	{
		const tpRefPtr<tpReferenced>& item = ml[i];

		if (item->getType()->isOfType(tpImageHandler::getTypeInfo()))
		{
			//tpLogNotify("%s found %s",__FUNCTION__,item->getType()->getName());

			tpRefPtr<tpImageHandler> nf = static_cast<tpImageHandler*>(item.get());

			if (nf.isValid() && nf->getCapability(TP_IMAGE_CAN_WRITE,name))
			{
				return nf->write(this,name);
			}
		}
	}

	return result;

}



TP_TYPE_REGISTER(tpImage,tpReferenced,Image);
TP_TYPE_REGISTER(tpImageHandler,tpReferenced,ImageHandler);



/*
class tpImageManager : public tpFactory<tpImageLoader>
{
	
};
*/


#if 0

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

#include "tpImage.h"
#include "tpSetup.h"
#include "tpSystem.h"

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#include "tpLog.h"


#include "tpModule.h"

#include "tpRefCache.h"

static tpUInt tpGetBPP(const tpUByte& format)
{
    switch (format)
	{
		case TP_RGB:
		case TP_BGR:
			return 24;
		case TP_RGBA:
		case TP_BGRA:
			return 32;
//		case TP_YUV422:
//		case TP_YUV444:
		case TP_YUV420:
			return 12;
		case TP_RGB565:
		case TP_RGB555:
			return 16;
		
		default:
			tpLogError("tpGetBPP() : format not supported");
	};
	return 0;
}


tpImage::tpImage() : tpReference(), m_width(0), m_height(0), m_stride(0), m_bpp(0), m_pixelformat(0), m_pixelcount(0), m_changecount(0)
{
}


tpImage::tpImage(const tpImage& image) : tpReference(image)
{
	*this = image;
}



tpImage& tpImage::operator = (const tpImage& image)
{
	this->release();

	//this->create(image.m_bitmap.buffer,
	//	image.m_bitmap.width,
	//	image.m_bitmap.height,
	//	image.m_bitmap.format);

	return *this;
}

tpImage::~tpImage()
{
    release();
}

tpImage::tpImage(tpUChar* data,
				 const tpUInt& width,
				 const tpUInt& height,
				 tpPixelFormat format)
{
    this->create(data,width,height,format);
};


tpVoid tpImage::create( const tpUByte* data, tpUInt width, tpUInt height, tpUByte format )
{
	if ((width == 0) || (height == 0)) 
	{
		// release all memory
		release();
		return;
	}

	// check the format first - renderers can preset images
	tpUInt bpp = tpGetBPP(format);

	tpUInt buffer_size = (bpp >> 3) * height * width;

	m_data.resize(buffer_size);
	//m_data.fill(0);

	//tpLogNotify("%s allocated %d bytes",__FUNCTION__,m_data.getSize());

	if (data)
	{
		const tpUByte* in_ptr = data; 
		for (tpSizeT i = 0; i < buffer_size; ++i )
		{
			m_data[i] = *in_ptr;
			in_ptr++;
		}
	} else {

	}

	m_width = width;
	m_height = height;
	m_bpp = bpp;
	m_pixelformat = format;
	m_pixelcount = width * height;

	dirty();

	//tpLogNotify("%s image info: %dx%d, %d bpp",__FUNCTION__, m_width, m_height, m_bpp);

}

void tpImage::release()
{
	// release internal memory buffer
	m_data.clear();

	// reset change count
	m_changecount = 0;
}

tpUByte* tpImage::getData()
{
    return this->m_data.getData();
}

const tpUByte* tpImage::getData() const
{
	return this->m_data.getData();
}

tpUInt tpImage::copyData( void* buffer, tpUInt length )
{
	m_data.copy((tpUByte*)buffer,length);


	dirty();

    return m_data.getSize();
}


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

tpVoid tpImage::getPowerOfTwo(tpUInt* potWidth, tpUInt* potHeight) const
{
	*potWidth = nextPowerOfTwo(m_width);
	*potHeight = nextPowerOfTwo(m_height);
}

tpVoid tpImage::getPowerOfTwoRatio(tpReal* potH,tpReal* potV) const
{
	tpUInt potWidth;
	tpUInt potHeight;

	getPowerOfTwo(&potWidth,&potHeight);

	*potH = (tpReal)m_width / potWidth;
	*potV = (tpReal)m_height / potHeight;
}


int tpImage::getWidth() const
{
    return m_width;
}

int tpImage::getHeight() const
{
    return m_height;
}

tpUInt tpImage::getBitPerPixel() const
{
	return tpGetBPP(m_pixelformat);
};

tpPixelFormat tpImage::getFormat() const
{
    return (tpPixelFormat)m_pixelformat;
}

int tpImage::getDataSize() const
{
    return m_data.getSize();
}


/*
tpImage& tpImage::getYUV(unsigned char* yuv)
{
    register int i,j;

    int w = m_width;
    int h = m_height;
    unsigned char *off = yuv+w*h;
    unsigned char *off2 = yuv+w*h+w*h/4;

    for(j=0; j<h; j++) {
      for(i=0; i<w; i++) {
         int b = *m_buffer++;
         int g = *m_buffer++;
         int r = *m_buffer++;
         int y,cb,cr;
         y  =  (int)((0.257 * r) + (0.504 * g) + (0.098 * b) + 16);
         cr =  (int)((0.439 * r) - (0.368 * g) - (0.071 * b) + 128);
         cb =  (int)(-(0.148 * r) - (0.291 * g) + (0.439 * b) + 128);
         if(y<0) y = 0; if(y>255) y = 255;
         if(cb<0) cb = 0; if(cb>255) cb = 255;
         if(cr<0) cr = 0; if(cr>255) cr = 255;
         *yuv++ = y;
         if(j&1&&i&1)
            *off++ = cr;
         if(j&1&&i&1)
            *off2++ = cb;
      }
   };
};

*/

void tpImage::savePPM(const char* filename)
{

/*
    tpLogNotify("tpImage::savePPM(%s)",filename);
    FILE* file;

    file = fopen(filename,"wb");

    if (!file)
    {
        tpLogError("tpImage::savePPM() : could not open file %s",filename);
        return;
    }

    fprintf(file, "P6\n");
    fprintf(file, "%d %d\n", m_width, m_height);
    fprintf(file, "255\n");
    fwrite(m_buffer, 1, m_bufsize, file);

    if (file) fclose(file);
*/
}

void tpImage::loadPPM(const char* filename)
{

/*
    tpLogNotify("tpImage::loadPPM(%s)",filename);
    FILE* file;

    file = fopen(filename,"r");

    if (!file)
    {
        tpLogError("tpImage::loadPPM() : could not open file %s",filename);
        return;
    };

    char line[80];
    fgets(line, sizeof(line), file);

    if ((line[0] != 'P') || (line[1] != '6'))
    {
        tpLogError("tpImage::loadPPM : unsupported format %c%c",line[0],line[1]);
        return;
    };

   for (;;) {
      if (!fgets(line, sizeof(line), file))
      {
            tpLogError("tpImage::loadPPM : damaged file");
            return;

       }
       if ((line[0] == '#') || (line[0] == '\n')) continue;
       if (sscanf(line, "%d %d", &m_width, &m_height) != 2)
       {
           tpLogError("tpImage::loadPPM : incorrect pixmap size");
           return;
       }
       break;
    }
    for (;;) {
        if (!fgets(line, sizeof(line), file)) {
            tpLogError("tpImage::loadPPM : damaged file");
            return;
        }
        if ((line[0] == '#') || (line[0] == '\n')) continue;
        int maxval = 0;
        if (sscanf(line, "%d", &maxval) != 1) {
            tpLogError("tpImage::loadPPM : bitmap size not readable");
            return;
        }
        break;
    }


    tpLogNotify("tpImage::loadPPM : loading %dx%d pixmap",m_width,m_height);

    release();

    m_format = TP_RGB;
    m_buffer = (tpUChar*)malloc(m_width*m_height*3);
    m_bufsize = m_width*m_height*3;

    fread(m_buffer, 3*m_width,m_height, file);

    if (file) fclose(file);
*/

}


void tpImage::loadYUV(char *fileName, int width, int height)
{
	/*
    FILE *fpointer;

	register int y;


    this->release();



    fpointer = fopen(fileName, "r");

    for (y = 0; y < height; y++)
        fread(y_data[y], 1, width, fpointer);

    for (y = 0; y < height / 2; y++)
        fread(cb_data[y], 1, width / 2, fpointer);

    for (y = 0; y < height / 2; y++)
        fread(cr_data[y], 1, width / 2, fpointer);

    fclose(fpointer);
    */
}

tpBool tpImage::isValid() const
{
	return (m_data.getSize() && m_width && m_height && m_bpp);
}

//tpVoid tpImage::createRainBowAlpha() 
//{
//	for (unsigned int x = 0; x < m_width; ++x)
//	{
//		for (unsigned int y = 0; y < m_height) 
//		{
//			
//		}
//	}
//}







/*
tpBool tpImage::save(const tpString& filename)
{
	return tpImageManager::save(filename,this);
}

tpBool tpImage::load(const tpString& filename)
{
	tpImage *_res = 0;
	_res = tpImageManager::load(filename);

	if (_res)
	{
		*this = *_res;
		return TRUE;
	};

	return FALSE;
};
*/


TP_TYPE_REGISTER(tpImage,tpReference,Image);
TP_TYPE_REGISTER(tpImageFactory,tpReference,ImageFactory);



// -------------------------------------------------------


#if 0

tpImageManager::tpImageManager()
{
};


tpImageManager::~tpImageManager()
{
};

/* static */
tpImage* tpImageManager::load(const tpString& filename)
{

	tpImage* _res = NULL;

	tpList<tpPluginInterface*> _plugins = tpThePluginManager->getInterfaces("image:db:");
	tpListIterator<tpPluginInterface*> _i = _plugins.getBegin();

	while (*_i)
	{
		tpImageDBInterface *_handler = static_cast<tpImageDBInterface*>(*_i);


		if (_handler)
		{
			tpString _provides = _handler->provide();

			if (_provides.contains(filename.afterLast('.')))
			{
				_res = new tpImage;
				if (_handler->load(filename.c_str(),_res->m_bitmap))
				{
					return _res;
				} else
				{
					delete _res;

					return NULL;
				}
			}
		}
		_i++;
	}

	return _res;
}



/* static */
tpBool tpImageManager::save(const tpString& filename,tpImage* img)
{

	if (!img) return false;

	tpList<tpPluginInterface*> _plugins = tpThePluginManager->getInterfaces("image:db:");
	tpListIterator<tpPluginInterface*> _i = _plugins.getBegin();

	while (*_i)
	{
		tpImageDBInterface *_handler = static_cast<tpImageDBInterface*>(*_i);


		if (_handler)
		{
			tpString _provides = _handler->provide();

			if (_provides.contains(filename.afterLast('.')))
			{

				return _handler->save(filename.c_str(),
					img->m_bitmap);
			}
		}
		_i++;
	}

	return false;
}

#endif


//////////////////////////////////////////////////////////////////////////

static inline 
void __imp_rgb888_to_rgb565(const tpUByte *src, tpUByte *dst, tpUInt pixelCount)
{
	tpUShort *dst_ptr = (tpUShort *) dst;
	for (tpUInt i = pixelCount; i; --i) {
		*dst_ptr = (((src[0] >> 3) & 0x1f) << 11)  |
			(((src[1] >> 2) & 0x3f) << 5)   |
			(((src[2] >> 3) & 0x1f) << 0);
		dst_ptr++;
		src += 3;
	}
}

static inline 
void __imp_rgb565_to_rgb888(const tpUByte *src, tpUByte *dst, tpUInt pixelCount) 
{
	tpUShort *src_ptr = (tpUShort *) src;
	for (tpUInt i = pixelCount; i; --i) 
	{
	   *dst++ = (((*src_ptr >> 11) & 0x1f) << 3);
	   *dst++ = (((*src_ptr >> 5) & 0x3f) << 2);
	   *dst++ = ((*src_ptr & 0x1f) << 3);
	   src_ptr++;
	}
}


tpBool tpImage::convert( const tpImage& input )
{
	create(0,input.getWidth(),input.getHeight(), getFormat() );

	if (input.getFormat() == TP_RGB && getFormat() == TP_RGB565)
	{
		__imp_rgb888_to_rgb565((unsigned char*)input.getData(),getData(),m_pixelcount);

		//tpLogNotify("%s - Converted to RGB565",__FUNCTION__);

		return true;

	} else if (input.getFormat() == TP_RGB565 && getFormat() == TP_RGB) { 
	
		__imp_rgb565_to_rgb888(input.getData(), getData(), m_pixelcount);
		
		return true;
		
	} else if (input.getFormat() == getFormat())
	{
		copyData((void*)input.getData(),input.getDataSize());

		//tpLogNotify("%s - already in native format",__FUNCTION__);

		return true;
	}
	return false;
}

#endif
