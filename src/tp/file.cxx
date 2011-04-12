#include <stdio.h>

#include "tp/file.h"
#include "tp/log.h"

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
	m_handle = fopen(uri.c_str(),mode.c_str());
	return (m_handle != 0L);
}


bool tpFile::close()
{
	fclose(_F(m_handle));
	return true;
}

void tpFile::sync()
{
	fflush(_F(m_handle));
}


tpFile& 
tpFile::seek( tpSizeT pos )
{
	m_gcount = fseek(_F(m_handle),pos,SEEK_SET);
	return *this;
}

tpFile& 
tpFile::read(char* buffer,tpSizeT buffersize)
{
	m_gcount = fread(buffer,1,buffersize,_F(m_handle));
	return *this;
}

tpFile& 
tpFile::write(const char* buffer, tpSizeT buffersize)
{
	m_gcount = fwrite(buffer,1,buffersize,_F(m_handle));
	return *this;
}

tpSizeT tpFile::tell() const
{
	return ftell(_F(m_handle));
}
