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
	return tpPixelFormat::kRGB565;
#endif
	
	return (needalpha) ? tpPixelFormat::kBGRA_8888 : tpPixelFormat::kRGB_888;

}

void tpTexture::cloneSupportedTextureFormat( tpImage& img )
{
}

TP_TYPE_REGISTER(tpTexture,tpObject,Texture);
