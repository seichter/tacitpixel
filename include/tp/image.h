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

#ifndef TPIMAGE_H
#define TPIMAGE_H

#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/chunk.h>


enum tpPixelFormat {
	TP_RGB888,
	TP_BGR888,
	TP_RGBA8888,
	TP_BGRA8888,
	TP_RGB565,
	TP_BGR565,
	TP_RGB555,
	TP_BGR555,
	TP_GREY8,
	TP_YUV420
};


class TP_API tpImage : public tpReferenced {

	tpSizedChunk m_data;

	unsigned int m_width;
	unsigned int m_height;
	
	tpUByte m_pixelformat;
	tpUInt m_changecount;

public:
	
	tpImage();

	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }

	void allocate(unsigned int w, unsigned int h, tpUByte pixelformat = TP_RGB888);
	
	bool isValid() const { return (m_data.getSize() && m_width && m_height); } 

	const void* getData() const { return m_data.getData(); }
	void* getData() { return m_data.getData(); }
	tpSizeT getDataSize() const { return m_data.getSize(); }
	
	tpUInt getChangeCount() const { return m_changecount; }
	void dirty() { ++m_changecount; }

protected:
	virtual ~tpImage();
};


//
struct tpImageOperator {
	virtual bool operator()(const tpImage* in, tpImage* out) = 0;
};


#endif
