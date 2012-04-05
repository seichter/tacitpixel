#include "soundstream_ogg.h"


tpSoundStreamOgg::tpSoundStreamOgg()
	: tpSoundStream()
	, mStreamSection(0)
	, vorbisInfo(0)
	, vorbisComment(0)
{

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
