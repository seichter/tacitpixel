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


#include "tp/types.h"
#include "tp/array.h"


//! \brief general formats for image formats
enum tpPixelFormat
{
	TP_RGB = 0,		//!< 24bit RGB format
	TP_BGR,
	TP_RGBA,		//!< 32bit RGB format aka RGBA/BGRA
	TP_BGRA,
	TP_YUV420,		//!< compressed YUV
	TP_YUV422,		//!< YUV
	TP_YUV444,		//!< YUV
	TP_RGB565,
	TP_RGB555,
	TP_ARGB5551
};


/*! \class tpImage
	\brief Image handler.
 */
class TP_CORE_API tpImage : public tpReferenced
{
public:

	TP_TYPE_DECLARE

	/*! Default c'tor.
	 */
    tpImage();

	/*! Copy c'tor.
		\param image image to copy from
	 */
	tpImage(const tpImage& image);

	/*! Initializing c'tor.
		\param data pointer to raw image data.
		\param width width of the image data provided above
		\param height height of the image data provided
		\param format bitmap formating (RGB, BGR ...)
	*/
    tpImage(tpUChar* data,
		const tpUInt& width,
		const tpUInt& height,
		tpPixelFormat format);





	/*! Access the raw data the image uses.
		\return pointer to raw data buffer.
	 */
    tpUByte* getData();


	const tpUByte* getData() const;

	/*! Copies data from the image to an external
		buffer. This function is depreciated!
		\param buffer
		\param length
	 */
	tpUInt copyData(void* buffer,tpUInt length);

	/*! Width of the image.
		\return width of the image data
	 */
    int getWidth() const;

	/*! Height of the image
		\return height of the image data.
	 */
    int getHeight() const;

	/*! Calculates the actual bits per pixel used for
		the image.
		\return bpp
	 */
    tpUInt getBitPerPixel() const;

	/*! Returns the internal data format.
		\return format of the image data
	 */
    tpPixelFormat getFormat() const;

	/*! \return the size of the internal image buffer
	 */
	int getDataSize() const;

    // tpUChar* getRAW(unsigned char* yuv);


	/*! Release the data in the internal data buffer.
	 */
    void release();


	tpVoid getPowerOfTwo(tpUInt* potWidth,tpUInt* potHeight) const;

	tpVoid getPowerOfTwoRatio(tpReal* potH,tpReal* potV) const;


	/*! \TODO member functions loadYUV, savePPM and loadPPM need to be private and
		used within load and save.
	 */
	void savePPM(const char* filename);
    void loadPPM(const char* filename);
    void loadYUV(char *fileName, int width, int height);


	/*! Creates an image from raw data.
	\param data pointer to raw image data.
	\param width width of the image data provided above
	\param height height of the image data provided
	\param format bitmap formating (RGB, BGR ...)
	*/
	tpVoid create( const tpUByte* data, tpUInt width, tpUInt height, tpUByte format );

    /*!
      Copy
    */
	tpImage& operator = (const tpImage& image);



	/*! Load image data from a file. Internally the tpImageManager will
	take care that the right plugin is loaded and the data been loaded.
	\param filename filename being used
	\return TRUE if successfull
	*/
	static tpImage* load(const tpString& name);


	static tpBool save( const tpString& name, const tpImage* img );


	tpBool isValid() const;


	/**
	 * This method converts another input image
	 * into the format of this one. 
	 *
	 * \param input Image to be cloned
	 * \return true if the conversion was successfull
	 */
	tpBool convert( const tpImage& input );

	tpVoid dirty() { m_changecount++; }

	tpUInt getChangeCount() const { return m_changecount; }
	tpVoid setChangeCount(tpUInt count) { m_changecount = count; }


	tpVoid setPixelFormat(tpUInt pixelformat) { m_pixelformat = pixelformat; }
	tpUInt getPixelFormat() const { return m_pixelformat; }

	tpVoid setFileName( const tpString& filename ) { m_filename = filename; }
	const tpString& getFileName() const { return m_filename; }

protected:

	/*! D'tor
	 */
    virtual ~tpImage();

	tpArray<tpUByte> m_data;
	
	tpUInt m_width;
	tpUInt m_height;
	tpUInt m_stride;
	tpUInt m_bpp;
	tpUByte m_pixelformat;
	tpUInt m_pixelcount;

	tpUInt m_changecount;

	tpString m_filename;
};

class tpImageUpdate
{
public:
	virtual tpVoid apply(tpImage& img) = 0;
};

#endif
