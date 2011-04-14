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
#include <tp/string.h>


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
	
	TP_TYPE_DECLARE;
	
	tpImage();

	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }
	tpUByte getPixelFormat() const { return m_pixelformat; }

	void allocate(unsigned int w, unsigned int h, tpUByte pixelformat = TP_RGB888);
	void copy(const void* data);
	
	bool isValid() const { return (m_data.getSize() && m_width && m_height); } 

	const void* getData() const { return m_data.getData(); }
	void* getData() { return m_data.getData(); }
	tpSizeT getDataSize() const { return m_data.getSize(); }
	
	tpUInt getChangeCount() const { return m_changecount; }
	void dirty() { ++m_changecount; }
	
	static tpImage* read(const tpString& name);
	
	bool write(const tpString& name) const;
	
protected:
	virtual ~tpImage();
};



enum tpImageFactoryCapability {
	TP_IMAGE_CAN_READ = 0x0,
	TP_IMAGE_CAN_WRITE
};

class TP_API tpImageHandler : public tpReferenced {
public:

	TP_TYPE_DECLARE;

	tpImageHandler();

	virtual bool getCapability(tpUInt capability,const tpString& name) { return false; }

	virtual tpImage* read(const tpString& name) { return 0; };
	virtual bool write(const tpImage* img, const tpString& name) { return false; } 
	
protected:
	
	virtual ~tpImageHandler();

};


#endif
