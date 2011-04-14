#ifndef TPFILE_H
#define TPFILE_H

#include <tp/globals.h>
#include <tp/types.h>
#include <tp/string.h>
#include <tp/io.h>


/*!
 * \class tpFile
 * \brief file based io 
 */
class TP_API tpFile : public tpIO {
public:

	tpFile();

	tpFile(const tpFile& file);

	virtual ~tpFile();

	/**
	 * Open a file in the required mode
	 * 
	 * \param uri (const tpString &) uri refers to the filename
	 * \param mode (const tpString &) mode string as define in C
	 * \return (bool) true if the file could be opened in the required mode
	 */
	virtual bool open(const tpString& uri, const tpString& mode = "r");

	/**
	 * Closing the file
	 * 
	 * \return (bool) true if the file could be closes successfully
	 */
	virtual bool close();

	/**
	 * Flush all buffers
	 * 
	 */
	virtual void sync();

	/**
	 * Read from the file into a buffer with a predefined size
	 * 
	 * \param buffer (char *) buffer to read into
	 * \param buffer_size (tpSizeT) size of the buffer provided
	 * \return (tpFile&) return *this for chaining
	 */
	tpFile& read(char* buffer,tpSizeT buffer_size);

	/**
	 * Write the bytes in the provided buffer into the file
	 * 
	 * \param buffer (const char *) buffer to write from
	 * \param buffer_size (tpSizeT) size of buffer provides / bytes to be written
	 * \return (tpFile&) return *this for chaining
	 */
	tpFile& write(const char* buffer,tpSizeT buffer_size);

	/**
	 * Seek implementation. Moves the input pointer in the file
	 * 
	 * \param pos (tpSizeT) position 
	 * \param rel (tpUByte) offset to where to seek from
	 * \return (tpFile&) returns *this 
	 */
	tpFile& seek(tpSizeT pos,tpUByte rel = kSeekSet);

	/**
	 * Get the current read/write pointer position
	 * 
	 * \return (tpSizeT) value of the pointer position
	 */
	tpSizeT tell() const;

	/**
	 * Return the bytecount of the last read/write operation
	 * 
	 * \return (tpSizeT) value of last read write operation
	 */
	tpSizeT getCount() const { return m_gcount; }


	//void operator << (const tpString& str);
	//void operator << (tpInt i);

	//void operator << (const tpMemory<tpChar>&);
	//void operator >> (tpMemory<tpChar>&);

	void setHandle(void* handle) { m_handle = handle; }
	const void* getHandle() const { return m_handle; }

protected:

	tpSizeT m_gcount;

private:

	void* m_handle;

};

#endif
