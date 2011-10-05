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

	tpSizedChunk m_data;

	unsigned int m_width;
	unsigned int m_height;
	
	tpUByte m_pixelformat;
	tpUInt m_changecount;

public:
	
	TP_TYPE_DECLARE;
	
	tpImage();

	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }
	tpUByte getPixelFormat() const { return m_pixelformat; }

	void allocate(unsigned int w, unsigned int h, tpUByte pixelformat = tpPixelFormat::kRGB888);
	void copy(const void* data);
	
	bool isValid() const { return (m_data.getSize() && m_width && m_height); } 

	const void* getData() const { return m_data.getData(); }
	void* getData() { return m_data.getData(); }
	tpSizeT getDataSize() const { return m_data.getSize(); }
	
	tpUInt getChangeCount() const { return m_changecount; }
    void makeDirty() { ++m_changecount; }
	
	static tpImage* read(const tpString& name);
	
	bool write(const tpString& name) const;
	
protected:
	virtual ~tpImage();
};


#endif
