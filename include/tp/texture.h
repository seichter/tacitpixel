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
