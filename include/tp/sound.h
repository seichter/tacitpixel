#ifndef TPSOUND_H
#define TPSOUND_H

#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/array.h>
#include <tp/refptr.h>
#include <tp/string.h>

TP_API void tpSoundTest(const char*);

class TP_API tpSoundStream : public tpReferenced {
public:

	enum {
		kFormatMono8,
		kFormatMono16,
		kFormatMono32,
		kFormatStereo8,
		kFormatStereo16,
		kFormatStereo32
	};

	/**
	  * Opens a sound file
	  *
	  * @param name filename/url of the file
	  * @return true if successful
	  */
	virtual bool open(const tpString& name);

	/**
	  * read data from the stream into a buffer. The size of the
	  * buffer needs to be preset and is adjusted on return.
	  *
	  * @param buffer storage where data is being read into
	  * @param format returns the format of the audio data
	  * @param freq returns the sample rate in Hz
	  * @return true if successful
	  */
	virtual bool read(tpArray<tpChar> &buffer, tpUInt &format, tpUInt &freq);

	/**
	  * Closes the stream. Needs to be explicitly shut down
	  */
	virtual void close();

private:

	tpRefPtr<tpSoundStream> mImp;

};


#endif
