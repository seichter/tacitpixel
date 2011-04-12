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

#include "tp/texture.h"


tpTexture::tpTexture(const tpString& name  /* = TP_NONAME */) :
	tpObject(name),
	m_wrap(tpVec2<tpUInt>(TP_TEXTURE_CLAMP,TP_TEXTURE_CLAMP)),
	m_image(0)
{
}

tpTexture::~tpTexture()
{
}


tpImage* tpTexture::getImage()
{
	return m_image.get();
}


void tpTexture::setImage( tpImage* image )
{
    m_image = image;
}


void tpTexture::setWrap(tpUInt wrap_u,tpUInt wrap_v)
{
	this->m_wrap[0] = wrap_u;
	this->m_wrap[1] = wrap_v;
}


tpVec2<tpUInt> tpTexture::getWrap() const
{
	return m_wrap;
}

void tpTexture::createTexture( tpUByte pixelformat )
{
	//m_image = new tpImageClone(pixelformat);
}

tpUByte tpTexture::getSupportedTextureFormat( bool needalpha )
{
#if 0 //defined(_WIN32_WCE)
	return TP_RGB565;
#endif
	
	return (needalpha) ? TP_RGBA8888 : TP_RGB888;

}

void tpTexture::cloneSupportedTextureFormat( tpImage& img )
{
}

TP_TYPE_REGISTER(tpTexture,tpObject,Texture);
