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


#if defined(USE_DLMALLOC)
	#define USE_DL_PREFIX 1
	#include <dlmalloc/malloc.h>
#endif

#include <tp/chunk.h>

#include <string.h>

tpChunk::tpChunk(tpSizeT size)
	: m_ptr(0)
{
	this->setSize(size);
}

tpChunk::~tpChunk()
{
	this->empty();
}

void 
tpChunk::setSize(tpSizeT size)
{
#if defined(USE_DLMALLOC)
	m_ptr = dlrealloc(m_ptr,size);
#else
	m_ptr = ::realloc(m_ptr,size);
#endif
}

void 
tpChunk::empty()
{
	if (m_ptr) 
#if defined(USE_DLMALLOC)
	dlfree(m_ptr); 
#else
	::free(m_ptr);
#endif
	m_ptr = 0L;
}

void 
tpSizedChunk::setSize(tpSizeT size)
{
	tpChunk::setSize(size);
	if (m_ptr) m_size = size;
}

void 
tpSizedChunk::assign(const void* ptr)
{
	memcpy(m_ptr,ptr,getSize());
}

void 
tpSizedChunk::empty()
{
	tpChunk::empty();
	m_size = 0;
}
