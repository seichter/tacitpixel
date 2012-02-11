#include <tp/sound.h>

#include <tp/config.h>
#include <tp/log.h>
#include <tp/node.h>
#include <tp/scopeptr.h>
#include <tp/mat.h>
#include <tp/rendercontext.h>
#include <tp/file.h>
#include <tp/system.h>


#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>

#if defined(TP_USE_OPENAL)
	#if defined(__APPLE__)
		#include <OpenAL/al.h>
		#include <OpenAL/alc.h>
	#else
		#include <AL/al.h>
		#include <AL/alc.h>
	#endif
#endif

static
const char* alGetErrorName(ALuint error) {
	switch (error) {
	case AL_NO_ERROR:
		return "no error";
	case AL_INVALID_NAME:
		return "invalid name";
	case AL_INVALID_ENUM:
		return "invalid enum";
	case AL_INVALID_VALUE:
		return "invalid value";
	case AL_INVALID_OPERATION:
		return "invalid operation";
	case AL_OUT_OF_MEMORY:
		return "out of memory";
  }
	return "N/A";
}

#define alCheckError() \
	if (int err = alGetError()) tpLogNotify("OpenAL error %d (0x%x) '%s' %d",err,err,alGetErrorName(err),__LINE__)



class tpSoundStreamOgg : public tpSoundStream {
public:

	bool open(const tpString& name);

	void close();

	bool read(tpArray<tpChar> &buffer, tpUInt &format, tpUInt &freq);

private:

	tpFile mFile;
	OggVorbis_File mStream;

	vorbis_info*    vorbisInfo;    // some formatting data
	vorbis_comment* vorbisComment; // user comments

};



bool
tpSoundStream::open(const tpString& name)
{

	if (name.afterLast('.') == "ogg") {
		mImp = new tpSoundStreamOgg();
	}

	if (!mImp.isValid()) return false;

	return mImp->open(name);
}


void
tpSoundStream::close()
{
	if (mImp.isValid()) mImp->close();
	mImp = 0;
}

bool
tpSoundStream::read(tpArray<tpChar> &buffer, tpUInt &format, tpUInt &freq)
{
	return (mImp.isValid()) ? mImp->read(buffer,format,freq) : false;
}

//class tpSoundStream : public tpReferenced {
//public:
//	enum {
//		kFormatMono8,
//		kFormatMono16,
//		kFormatMono32,
//		kFormatStereo8,
//		kFormatStereo16,
//		kFormatStereo32
//	};

//	virtual bool open(const tpString& name) = 0;

//	virtual bool read(tpArray<tpChar> &buffer, tpUInt &format, tpUInt &freq) = 0;

//	virtual void close() = 0;

//};




bool
tpSoundStreamOgg::open(const tpString &name)
{
	if (!mFile.open(name,"rb")) return false;

	FILE* f = reinterpret_cast<FILE*>(mFile.getHandle());
	if (ov_open(f,&mStream,0,0) < 0)
	{
		tpLogError("Could not open file");
		return false;
	}

	vorbisInfo = ov_info(&mStream, -1);
	vorbisComment = ov_comment(&mStream, -1);

	tpLogNotify("version %d",vorbisInfo->version);
	tpLogNotify("channels %d",vorbisInfo->channels);
	tpLogNotify("rate (Hz) %ld",vorbisInfo->rate);

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

	buffer.resize(2048);

	int section = 0;
	tpUInt32 size = 0;

	while (size < buffer.getSize()) {
		int result = ov_read(&mStream,
							 buffer.getData()+size,
							 buffer.getSize()-size,
							 tpSystem::get()->getEndian(),
							 2,
							 1,
							 &section
							 );

		if (result > 0) {
			size += result;
		} else {
			if (result < 0) {
				buffer.resize(0);
				tpLogError("Error reading file");
				return false;
			} else {
				break;
			}
		}
		tpLogNotify("%s %d",__FUNCTION__,result);
	}

	buffer.resize(size);

	if (size == 0) return false;

	return true;
}

void
tpSoundStreamOgg::close()
{
	ov_clear(&mStream);
	mFile.close();
}

class tpSoundContext_OAL : public tpReferenced {

	ALCcontext* mContext;
	ALCdevice* mDevice;
public:

	tpSoundContext_OAL()
		: tpReferenced()
		, mContext(0)
		, mDevice(0)
	{}

	void create()
	{
		const ALCchar *default_device;

		default_device = alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);

		if ((mDevice = alcOpenDevice(default_device)) == NULL) {
			tpLogNotify("failed to open sound device");
		}

		mContext = alcCreateContext(mDevice, NULL);

		makeCurrent();

		tpLogNotify("OpenAL on device '%s'\n\t%s\n\t%s\n\t%s\n\t%s",
					default_device,
					alGetString(AL_VENDOR),
					alGetString(AL_VERSION),
					alGetString(AL_RENDERER),
					alGetString(AL_EXTENSIONS));
	}

	void makeCurrent()
	{
		alcMakeContextCurrent(mContext);
		alcProcessContext(mContext);
	}

	void suspend()
	{
		alcSuspendContext(mContext);
	}

	void destroy()
	{
		alcDestroyContext(mContext);
		alcCloseDevice(mDevice);
	}
};


class tpSoundStorage : public tpReferenced {


};

class tpSound : public tpReferenced {
public:

protected:
};


class tpSoundOpenAL : public tpSound {

	tpVec3f mVelocity;
	tpMat44f mPosition;

//    ALuint  bufferID;
	ALuint sourceID;
	ALuint buffers[2];

public:

	tpSoundOpenAL()
	{
	}

	void create()
	{

		tpVec4f pos(0.0,0.0,4.0,1.0);
		tpVec4f vel(0,0,0,1);

		float listenerPos[]={0.0,0.0,4.0};
		float listenerVel[]={0.0,0.0,0.0};
		float listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};

//		alListenerfv(AL_POSITION,listenerPos);
//	    alListenerfv(AL_VELOCITY,listenerVel);
//	    alListenerfv(AL_ORIENTATION,listenerOri);

		ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
		ALfloat source0Vel[]={ 0.0, 0.0, 0.0};


		alGetError(); // clear any error messages

		// Generate buffers, or else no sound will happen!
		alGenBuffers(2,&buffers[0]);
		alGenSources(1,&sourceID);

		alCheckError();

//		alSourcef(sourceID, AL_PITCH, 1.0f);
//		alSourcef(sourceID, AL_GAIN, 1.0f);
		alSourcefv(sourceID, AL_POSITION, source0Pos);
		alSourcefv(sourceID, AL_VELOCITY, source0Vel);

		alCheckError();
//        alSourcei(sourceID, AL_BUFFER,bufferID);
//        alSourcei(sourceID, AL_LOOPING, AL_TRUE);


	}

	void write(ALuint bufferID, const tpArray<tpChar>& buffer,tpUInt format, tpUInt rateFreq)
	{

		if (0 == buffer.getSize()) return;

		ALenum alFormat(0);
		switch (format) {
		case tpSoundStream::kFormatMono16:
			alFormat = AL_FORMAT_MONO16;
			break;
		case tpSoundStream::kFormatStereo16:
			alFormat = AL_FORMAT_STEREO16;
			break;
		}

		tpLogNotify("%s 0x%x %ld %d",__FUNCTION__,alFormat,buffer.getSize(),rateFreq);

		alBufferData(bufferID,alFormat,buffer.getData(),buffer.getSize(),rateFreq);
		alCheckError();
	}


	bool playing()
	{
		ALenum state;
		alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
		alCheckError();
	}

	bool update(tpSoundStream& stream)
	{
		int processed;
		bool active = true;

		alGetSourcei(sourceID, AL_BUFFERS_PROCESSED, &processed);

		while(processed--)
		{
			ALuint buffer;

			alSourceUnqueueBuffers(sourceID, 1, &buffer);
//            check();
			alCheckError();

			active = this->stream(buffer,stream);
//            active = stream(buffer);

			alSourceQueueBuffers(sourceID, 1, &buffer);
//            check();

			alCheckError();
		}

		tpLogNotify("%s",__FUNCTION__);

		return active;
	}

	bool
	stream(ALuint buffer,tpSoundStream& stream)
	{
		tpArray<tpChar> streambuffer;
		tpUInt format; tpUInt freq;

		if (stream.read(streambuffer,format,freq))
		{
			this->write(buffer,streambuffer,format,freq);
			return true;
		}

		return false;
	}


	bool
	play(tpSoundStream& stream)
	{

		if (playing()) return true;

		if (!this->stream(buffers[0],stream)) return false;

		if (!this->stream(buffers[1],stream)) return false;

		alSourceQueueBuffers(sourceID, 2,&buffers[0]);
		alSourcePlay(sourceID);

		alCheckError();

		return true;
	}
};

void tpSoundTest(const char* what) {


	tpLog::get().setLevel(tpLog::kLogAll);

	tpRefPtr<tpSoundOpenAL> sound = new tpSoundOpenAL();
	tpRefPtr<tpSoundStream> stream = new tpSoundStreamOgg();

	stream->open(what);

//	tpArray<tpChar> buffer;
//	tpUInt f,r;
//	while (stream->read(buffer,f,r)) {}





#if 1

	tpRefPtr<tpSoundContext_OAL> context = new tpSoundContext_OAL;
	context->create();
	context->makeCurrent();

	sound->create();

	 tpLogNotify("Tick!");

	 if(sound->play(*stream.get())) {
		while(sound->update(*stream.get())) {
		}
	}
#endif

	tpLogNotify("Finished!");


	getchar();


}
