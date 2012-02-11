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

	virtual bool open(const tpString& name);

	virtual bool read(tpArray<tpChar> &buffer, tpUInt &format, tpUInt &freq);

	virtual void close();

private:

	tpRefPtr<tpSoundStream> mImp;

};


#endif
