#include <tp/chunk.h>

#include <dlmalloc/malloc.h>

#include <string.h>

tpChunk::tpChunk(tpSizeT size) : m_ptr(0)
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
	m_ptr = dlrealloc(m_ptr,size);
}

void 
tpChunk::empty()
{
	if (m_ptr) dlfree(m_ptr); 
	m_ptr = 0L;
}

void 
tpSizedChunk::setSize(tpSizeT size)
{
	tpChunk::setSize(size);
	if (m_ptr) m_size = size;
}

void 
tpSizedChunk::copy(const void* ptr, tpSizeT size)
{
	tpChunk::setSize(size);
	memcpy(m_ptr,ptr,size);
}

void 
tpSizedChunk::empty()
{
	tpChunk::empty();
	m_size = 0;
}
