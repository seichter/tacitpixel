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

#ifndef TPTEXTURE_H
#define TPTEXTURE_H


#include <tp/object.h>
#include <tp/image.h>
#include <tp/vec.h>

enum tpTextureWrap
{
	TP_TEXTURE_CLAMP,
	TP_TEXTURE_REPEAT
};


/*!
	\class tpTexture
	\author Hartmut Seichter
	\brief Provides support for bitmap textures.

	Adds support for textures to the scenegraph.

*/

class TP_API tpTexture : public tpObject
{
public:

	TP_TYPE_DECLARE;

	tpTexture(const tpString& name = "");

	void createTexture(tpUByte pixelformat);

	/*! 
	 * returns a pointer to the image.
	 */
	tpImage* getImage();


	void setImage(tpImage* image);

	void setWrap(tpUInt wrap_u,tpUInt wrap_v);

	tpVec2<tpUInt> getWrap() const;

	static tpUByte getSupportedTextureFormat(bool needalpha);


	void cloneSupportedTextureFormat( tpImage& img );

protected:

	/*! Destructor.
	 */
	virtual ~tpTexture();

	tpVec2<tpUInt> m_wrap;

	tpRefPtr<tpImage> m_image;

};


#endif
