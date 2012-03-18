
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
