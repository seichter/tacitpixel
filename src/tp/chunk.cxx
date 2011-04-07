#include <tp/chunk.h>

#include <dlmalloc/malloc.h>


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

