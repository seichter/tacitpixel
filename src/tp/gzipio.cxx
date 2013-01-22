/*
 * Copyright (C) 1999-2011 Hartmut Seichter
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

#include "tp/gzipio.h"
#include "tp/array.h"
#include "tp/log.h"

#include <zlib/zlib.h>

#include <cassert>

struct tpGZipIOHandle {

    tpIO* stream;
    z_stream strm;

};

tpGZipIO::tpGZipIO() : mHandle(0)
{
}

bool tpGZipIO::open( tpIO* input )
{

    tpLogNotify("tpZipStream using %s",zlibVersion());

    mHandle = new tpGZipIOHandle;
	mHandle->stream = input;

	mHandle->strm.zalloc = Z_NULL;
	mHandle->strm.zfree = Z_NULL;
	mHandle->strm.opaque = Z_NULL;
	mHandle->strm.avail_in = Z_NULL;
	mHandle->strm.next_in = Z_NULL;

	const int max_wbits = 15+16; // gzip

    int ret = inflateInit2(&mHandle->strm,max_wbits);

    if (ret == Z_OK) {
        mState = tpIO::kGood;
    } else {
        tpLogError("Couldn't initialize zlib (%d)",ret);
        mState = tpIO::kFail;
    }

	return (ret == Z_ERRNO);
}

bool tpGZipIO::close()
{
	if (mHandle) {
		inflateEnd(&mHandle->strm);
		mHandle->stream = 0;
		delete mHandle;
	}

	return (mHandle == 0);
}

tpIO& tpGZipIO::read( char *buffer, tpSizeT buffer_size )
{
    tpArray<tpUChar> zipdata; zipdata.resize(16384);
    tpUChar* inputbuffer = (tpUChar*)buffer;

    mHandle->stream->read((tpChar*)zipdata.getData(),zipdata.getSize());

    tpLogNotify("data read %d bytes",mHandle->stream->getCount());
    tpLogNotify("inflate arena %d bytes",buffer_size);

    const int flush = Z_NO_FLUSH;

	mHandle->strm.avail_in = mHandle->stream->getCount();
    mHandle->strm.next_in = zipdata.getData();

    if (mHandle->stream->getCount() > 0) {

        mHandle->strm.avail_out = buffer_size;
        mHandle->strm.next_out = inputbuffer;

        // ?? maybe we need flush
        int ret = inflate(&mHandle->strm,flush);
        assert(ret != Z_STREAM_ERROR);
        tpLogError("zlib inflate code (%d)",ret);

        switch (ret) {
            case Z_STREAM_END:
                mState |= tpIO::kEOF;
                tpLogError("Stream end reached");
                break;
            case Z_ERRNO:
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                tpLogError("zlib error (%d)",ret);
                this->close();
                this->mState |= tpIO::kFail;
                break;
            case Z_OK:
                this->mState = tpIO::kGood;
                mGCount = buffer_size-mHandle->strm.avail_out;
                break;
        }

    } else {

        this->mState = tpIO::kEOF;
    }

	return *this;
}

tpIO& tpGZipIO::write( const char *buffer, tpSizeT buffer_size )
{
	return *this;
}

tpIO& tpGZipIO::seek( tpSizeT pos, tpUByte rel )
{
	return *this;
}

tpSizeT tpGZipIO::tell() const
{
	return 0;
}

tpSizeT tpGZipIO::getCount() const
{
	return mGCount;
}

void tpGZipIO::sync()
{
}
