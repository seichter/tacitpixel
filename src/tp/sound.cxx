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
	return "N/A";
}

#define alCheckError() \
    if (int err = alGetError()) tpLogNotify("OpenAL error %d (0x%x) '%s' %d",err,err,alGetErrorName(err),__LINE__)
	if (int err = alGetError()) tpLogNotify("OpenAL error %d (0x%x) '%s'",err,err,alGetErrorName(err))


class tpSoundStream : public tpReferenced {
public:
    enum {
        kFormatMono8,
        kFormatMono16,
        kFormatMono32,
        kFormatStereo8,
        kFormatStereo16,
        kFormatStereo32
    };

    virtual bool open(const tpString& name) = 0;

    virtual bool read(tpArray<tpChar> &buffer, tpUInt &format, tpUInt &freq) = 0;
class tpSound : public tpNode {
protected:
};


class tpSoundStreamOgg : public tpSoundStream {
struct tpSoundOpenALContext {

};

class tpSoundOpenAL : public tpSound {

	tpVec3f mVelocity;
	tpMat44f mPosition;

public:

    bool open(const tpString& name);
    void close();

    bool read(tpArray<tpChar> &buffer, tpUInt &format, tpUInt &freq);

//    bool playback();
//    bool playing();
//    bool update();

	tpSoundOpenAL() {

    tpFile mFile;
    OggVorbis_File mStream;

    vorbis_info*    vorbisInfo;    // some formatting data
    vorbis_comment* vorbisComment; // user comments

};


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
                tpLogError("Error reading file");
                return false;
            } else {
                break;
            }
        }
    }
    if (size == 0) return false;

    buffer.resize(size);

    return true;
}

class tpSoundContext_OAL : public tpReferenced {

    ALCcontext* mContext;
public:

    tpSoundContext_OAL()
        : tpReferenced()
        , mContext(0)
    {}

    void create()
    {
        const ALCchar *default_device;
        ALCdevice *device;
        default_device = alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);
		const ALCchar *defaultDevice;
		ALCdevice *device;
		defaultDevice = alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);

        if ((device = alcOpenDevice(default_device)) == NULL) {
            tpLogNotify("failed to open sound device");
        }
		if ((device = alcOpenDevice(defaultDevice)) == NULL) {
			tpLogNotify("failed to open sound device");
		}

        mContext = alcCreateContext(device, NULL);
		ALCcontext *context = alcCreateContext(device, NULL);
		alcMakeContextCurrent(context);
		alcProcessContext(context);

        makeCurrent();

        tpLogNotify("OpenAL on device '%s'\n\t%s\n\t%s\n\t%s\n\t%s",
                    default_device,
                    alGetString(AL_VENDOR),
                    alGetString(AL_VERSION),
                    alGetString(AL_RENDERER),
                    alGetString(AL_EXTENSIONS));
    }
		tpLogNotify("OpenAL\n\t%s\n\t%s\n\t%s\n\t%s",
					alGetString(AL_VENDOR),
					alGetString(AL_VERSION),
					alGetString(AL_RENDERER),
					alGetString(AL_EXTENSIONS));

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

		float listenerPos[]={0.0,0.0,4.0};
		float listenerVel[]={0.0,0.0,0.0};
		float listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};
		

//		alListenerfv(AL_POSITION,listenerPos);
//	    alListenerfv(AL_VELOCITY,listenerVel);
//	    alListenerfv(AL_ORIENTATION,listenerOri);
	

		ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
		ALfloat source0Vel[]={ 0.0, 0.0, 0.0};
	
	



		ALuint  bufferID;
		ALuint  sourceID;
		// tpUInt  environment[NUM_ENVIRONMENTS];

		ALsizei size,freq;
		ALenum  format;
		ALvoid  *data;
		

		alGetError(); // clear any error messages

	   	// Generate buffers, or else no sound will happen!
        alGenBuffers(2,&buffers[0]);
        alGenSources(1,&sourceID);
		// Generate buffers, or else no sound will happen!
		alGenBuffers(1,&bufferID);

        alCheckError();
		alCheckError();

        alSourcef(sourceID, AL_PITCH, 1.0f);
        alSourcef(sourceID, AL_GAIN, 1.0f);
        alSourcefv(sourceID, AL_POSITION, source0Pos);
        alSourcefv(sourceID, AL_VELOCITY, source0Vel);
		alBufferData(bufferID,format,data,size,freq);

		alSourcef(sourceID, AL_PITCH, 1.0f);
		alSourcef(sourceID, AL_GAIN, 1.0f);
		alSourcefv(sourceID, AL_POSITION, source0Pos);
		alSourcefv(sourceID, AL_VELOCITY, source0Vel);
		alSourcei(sourceID, AL_BUFFER,bufferID);
		alSourcei(sourceID, AL_LOOPING, AL_TRUE);

        alCheckError();
//        alSourcei(sourceID, AL_BUFFER,bufferID);
//        alSourcei(sourceID, AL_LOOPING, AL_TRUE);


	}
	
    void write(ALuint bufferID, const tpArray<tpChar>& buffer,tpUInt format, tpUInt rateFreq)
    {
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

	void run() {

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


            tpArray<tpChar> streambuffer;
            tpUInt format; tpUInt freq;

            active = this->stream(buffer,stream);
//            active = stream(buffer);

            alSourceQueueBuffers(sourceID, 1, &buffer);
//            check();

            alCheckError();
        }



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

        return true;
    }


    bool play(tpSoundStream& stream)
    {

        if (playing()) return true;

        if (!this->stream(buffers[0],stream)) return false;

        if (!this->stream(buffers[1],stream)) return false;

        alSourceQueueBuffers(sourceID, 2,&buffers[0]);
        alSourcePlay(sourceID);

        return true;
    }



};

void tpSoundTest(const char* what) {
void tpSoundTest() {

	tpRefPtr<tpSound> sound = new tpSoundOpenAL();

    tpLog::get().setLevel(tpLog::kLogAll);

    tpRefPtr<tpSoundOpenAL> sound = new tpSoundOpenAL();
    tpRefPtr<tpSoundStream> stream = new tpSoundStreamOgg();

    stream->open(what);

    tpRefPtr<tpSoundContext_OAL> context = new tpSoundContext_OAL;
    context->create();
    context->makeCurrent();

    sound->create();

     tpLogNotify("Tick!");

     if(!sound->play(*stream.get())) {
         tpLogError("Already here off");
     }

    while(sound->update(*stream.get())) {
        if(!sound->playing()) {
            if(!sound->play(*stream.get())) {

            }
        }
    }

    tpLogNotify("Finished!");


    getchar();


}
