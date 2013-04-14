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

#include <tp/allocator.h>


#if defined(USE_DLMALLOC)
#define USE_DL_PREFIX
#include <dlmalloc/malloc_.h>
#else
#include <memory.h>
#endif


/*static*/ void* 
tpAllocator::malloc(tpSizeT size) 
{
#if defined(USE_DLMALLOC)
	return dlmalloc(size);
#else
	
#endif
}

/*static*/ void 
tpAllocator::free(void* ptr) 
{
#if defined(USE_DLMALLOC)
	dlfree(ptr);
#else
	
#endif
}

/*static*/ void* 
tpAllocator::memalign( tpSizeT size,tpSizeT alignment/* = 16*/)
{
#if defined(USE_DLMALLOC)
	return dlmemalign(alignment,size);
#else
	return 0;
#endif
}

/*static*/ void*
tpAllocator::calloc(tpSizeT elem, tpSizeT elem_size)
{
#if defined(USE_DLMALLOC)
	return dlcalloc(elem,elem_size);
#else 
	return 0;
#endif
}

/*static*/ void*
tpAllocator::realloc(void* p, tpSizeT n)
{
#if defined(USE_DLMALLOC)
	return dlrealloc(p,n);
#else
	return realloc(p,n);
#endif
		
}


