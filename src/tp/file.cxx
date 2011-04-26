#include <stdio.h>

#include "tp/file.h"

#if defined(HAVE_STDIO_H)

//! macro to cast the file pointer
#define _F(h) static_cast<FILE*>(h)

tpFile::tpFile() : m_handle(0L)
{
}

tpFile::tpFile(const tpFile& file) : m_handle(file.m_handle)
{
}

tpFile::~tpFile()
{
	close();
}

bool tpFile::open(const tpString& uri, const tpString& mode)
{
	m_handle = _F(fopen(uri.c_str(),mode.c_str()));
	return (m_handle != 0L);
}


bool tpFile::close()
{
	if (m_handle) fclose(_F(m_handle));
	m_handle = 0L;
	return true;
}

void tpFile::sync()
{
	fflush(_F(m_handle));
}


tpFile& 
tpFile::seek( tpSizeT pos, tpUByte rel )
{
	// set bad bit if seek fails - and good if return == 0
	clear( fseek(_F(m_handle),pos,rel) ? kBad : kGood ); 
	return *this;
}

tpFile& 
tpFile::read(char* buffer,tpSizeT buffersize)
{
	m_gcount = fread(buffer,sizeof(char),buffersize,_F(m_handle));
	if (getCount() != buffersize)
	{
		if (feof(_F(m_handle))) m_state |= kEOF;
	}
	return *this;
}

tpFile& 
tpFile::write(const char* buffer, tpSizeT buffersize)
{
	m_gcount = fwrite(buffer,sizeof(char),buffersize,_F(m_handle));
	if (getCount() != buffersize)
	{
		if (feof(_F(m_handle))) m_state |= kEOF;
	}
	return *this;
}

tpSizeT tpFile::tell() const
{
	return ftell(_F(m_handle));
}

#endif
