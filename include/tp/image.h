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

#ifndef TPIMAGE_H
#define TPIMAGE_H

#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/chunk.h>
#include <tp/string.h>
#include <tp/pixelformat.h>


class TP_API tpImage : public tpReferenced {

	tpSizedChunk mData;

	tpUInt mWidth;
	tpUInt mHeight;
	
	tpUByte mPixelFormat;
	tpUInt mChangeCount;

public:
	
	TP_TYPE_DECLARE
	
	/** C'tor */
	tpImage();

	/** get width of the image */
	tpUInt getWidth() const { return mWidth; }

	/** get height of the image */
	tpUInt getHeight() const { return mHeight; }

	/** get the pixelformat */
	tpUByte getPixelFormat() const { return mPixelFormat; }

	/**
	  * @brief allocate memory and setup structures for the storing an image
	  * @param w width of the image
	  * @param h height of the image
	  * @param pixelformat layout of the pixels in the image
	  */
	void allocate(unsigned int w, unsigned int h, tpUByte pixelformat = tpPixelFormat::kRGB_888);

	/**
	  * @brief copy data into the internal buffer
	  * @param data pointer to data
	  */
	void assign(const void* data);
	
	/**
	  * @brief check if image holds valid data
	  * @return true if size and dimensions are set
	  */
	bool isValid() const { return (mData.getSize() && mWidth && mHeight); }

	/** const pointer to buffer */
	const void* getData() const { return mData.getData(); }

	/** non const version of pointer to data */
	void* getData() { return mData.getData(); }

	/** get the size of the buffer */
	tpSizeT getDataSize() const { return mData.getSize(); }
	
	/** return change count */
	tpUInt getChangeCount() const { return mChangeCount; }

	/** push the changecount - signal a changed image */
	void setDirty() { ++mChangeCount; }
	
	/**
	  * @brief read an image from a file or url
	  * @param name set the name for the image file
	  * @return image file or NULL if there was an error
	  */
	static tpImage* read(const tpString& name);
	
	/**
	  * @brief writes an image back to a file
	  * @param name of the file
	  * @return true if the file could be written
	  */
	bool write(const tpString& name) const;
	
protected:
	virtual ~tpImage();
};


#endif
