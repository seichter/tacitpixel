/* 
 * Copyright (C) 1999-2013 Hartmut Seichter
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

#include <stdio.h>

#include "tp/config.h"
#include "tp/file.h"
#include "tp/array.h"
#include "tp/types.h"

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

bool 
tpFile::open(const tpString& uri, const tpString& mode)
{
	if (uri.isEmpty()) return false;
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
		if (feof(_F(m_handle))) mState |= kEOF;
	}
	return *this;
}

tpFile& 
tpFile::write(const char* buffer, tpSizeT buffersize)
{
	m_gcount = fwrite(buffer,sizeof(char),buffersize,_F(m_handle));
	if (getCount() != buffersize)
	{
		if (feof(_F(m_handle))) mState |= kEOF;
	}
	return *this;
}

tpSizeT tpFile::tell() const
{
	return ftell(_F(m_handle));
}

#endif

