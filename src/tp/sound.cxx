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
	#endif
#endif


class tpSound : public tpNode {
protected:
};


class tpSoundOpenAL : public tpSound {
	
	tpVec3f mVelocity;
	tpMat44f mPosition;
	
public:
	
	tpSoundOpenAL() {
		
		#define NUM_BUFFERS 1
		#define NUM_SOURCES 1
//		#define NUM_ENVIRONMENTS 1
		
		float listenerPos[]={0.0,0.0,4.0};
		float listenerVel[]={0.0,0.0,0.0};
		float listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};
		
		alListenerfv(AL_POSITION,listenerPos);
	    alListenerfv(AL_VELOCITY,listenerVel);
	    alListenerfv(AL_ORIENTATION,listenerOri);
	
		ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
		ALfloat source0Vel[]={ 0.0, 0.0, 0.0};
	
	
		tpUInt  buffer[NUM_BUFFERS];
		tpUInt  source[NUM_SOURCES];
		// tpUInt  environment[NUM_ENVIRONMENTS];

		ALsizei size,freq;
		ALenum  format;
		ALvoid  *data;
		
		alGetError(); // clear any error messages

	   	// Generate buffers, or else no sound will happen!
    	alGenBuffers(NUM_BUFFERS, buffer);

    	if(alGetError() != AL_NO_ERROR) 
    	{
        	tpLogError("- Error creating buffers !!\n");
        	exit(1);
    	}
    	else
    	{
        	tpLogNotify("init() - No errors yet.");
    	}

    	alBufferData(buffer[0],format,data,size,freq);

		alSourcef(source[0], AL_PITCH, 1.0f);
		alSourcef(source[0], AL_GAIN, 1.0f);
		alSourcefv(source[0], AL_POSITION, source0Pos);
		alSourcefv(source[0], AL_VELOCITY, source0Vel);
		alSourcei(source[0], AL_BUFFER,buffer[0]);
		alSourcei(source[0], AL_LOOPING, AL_TRUE);


	}
	
	void run() {
	   	
	}
	
	
	
};