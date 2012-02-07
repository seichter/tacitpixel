#include <tp/config.h>
#include <tp/log.h>
#include <tp/node.h>
#include <tp/scopeptr.h>
#include <tp/mat.h>

#if defined(TP_USE_OPENAL)
	#if defined(__APPLE__)
		#include <OpenAL/al.h>
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
    if (int err = alGetError()) tpLogNotify("OpenAL error %d (0x%x) '%s'",err,err,alGetErrorName(err))


class tpSound : public tpNode {
protected:
};


class tpSoundOpenAL : public tpSound {
	
	tpVec3f mVelocity;
	tpMat44f mPosition;
	
public:




    tpSoundOpenAL() {

        const ALCchar *default_device;
        ALCdevice *device;
        default_device = alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);

        if ((device = alcOpenDevice(default_device)) == NULL) {
            tpLogNotify("failed to open sound device");
        }

        ALCcontext *context = alcCreateContext(device, NULL);
       // alcMakeCurrentContext
        alcMakeContextCurrent(context);
        alcProcessContext(context);

        tpLogNotify("OpenAL\n\t%s\n\t%s\n\t%s\n\t%s",
                    alGetString(AL_VENDOR),
                    alGetString(AL_VERSION),
                    alGetString(AL_RENDERER),
                    alGetString(AL_EXTENSIONS));


				
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
        alGenBuffers(1,&bufferID);

        alCheckError();

        alBufferData(bufferID,format,data,size,freq);

        alSourcef(sourceID, AL_PITCH, 1.0f);
        alSourcef(sourceID, AL_GAIN, 1.0f);
        alSourcefv(sourceID, AL_POSITION, source0Pos);
        alSourcefv(sourceID, AL_VELOCITY, source0Vel);
        alSourcei(sourceID, AL_BUFFER,bufferID);
        alSourcei(sourceID, AL_LOOPING, AL_TRUE);


	}
	
	void run() {
	   	
	}
	
	
	
};

void tpSoundTest() {

    tpRefPtr<tpSound> sound = new tpSoundOpenAL();


}
