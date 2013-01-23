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
#include "soundstream_ogg.h"


tpSoundStreamOgg::tpSoundStreamOgg()
	: tpSoundStream()
	, mStreamSection(0)
	, vorbisInfo(0)
	, vorbisComment(0)
{

}

tpSoundStreamOgg::~tpSoundStreamOgg()
{
    this->close();
}


bool
tpSoundStreamOgg::open(const tpString &name)
{
	if (!mFile.open(name,"rb")) return false;

	FILE* f = reinterpret_cast<FILE*>(mFile.getHandle());
	if (ov_open(f,&mStream,0,0) < 0)
	{
		tpLogError("Ogg/Vorbis could not open file");
		return false;
	}

	vorbisInfo = ov_info(&mStream, -1);
	vorbisComment = ov_comment(&mStream, -1);

	tpLogNotify("Ogg/Vorbis stream v:%d ch:%d %dHz",
				vorbisInfo->version,
				vorbisInfo->channels,
				vorbisInfo->rate);

    return true;
}

bool
tpSoundStreamOgg::read(tpArray<tpChar>& buffer,tpUInt& format,tpUInt& freq)
{
	if (vorbisInfo->channels == 1) {
		format = tpSoundStream::kFormatMono16;
	} else {
		format = tpSoundStream::kFormatStereo16;
	}

	freq = vorbisInfo->rate;

	int result = ov_read(&mStream,
						 buffer.getData(),
						 buffer.getSize(),
						 tpSystem::get()->getEndian(),
						 2,
						 1,
						 &mStreamSection
						 );

	if (result >= 0) {
		buffer.resize(result);
	} else {
		tpLogError("Ogg/Vorbis read error");
		return false;
	}

	return true;
}

void
tpSoundStreamOgg::close()
{
	ov_clear(&mStream);
	mFile.close();
}
