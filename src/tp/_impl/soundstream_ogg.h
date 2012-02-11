#ifndef TP_SOUNDSTREAM_OGG_H
#define TP_SOUNDSTREAM_OGG_H

#include <tp/sound.h>
#include <vorbis/vorbisfile.h>
#include <tp/file.h>
#include <tp/log.h>
#include <tp/system.h>

class tpSoundStreamOgg : public tpSoundStream {
public:

	tpSoundStreamOgg();

	bool open(const tpString& name);

	void close();

	bool read(tpArray<tpChar> &buffer, tpUInt &format, tpUInt &freq);

private:

	tpFile mFile;
	OggVorbis_File mStream;
	int mStreamSection;

	vorbis_info*    vorbisInfo;    // some formatting data
	vorbis_comment* vorbisComment; // user comments

};


#endif
