#include <jni.h>

#include <tp/viewer.h>
#include <tp/logutils.h>

#include <android/log.h>
// #include <EGL/egl.h>

//NativeWindowType displayWindow;

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "tacit-jni", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "tacit-jni", __VA_ARGS__))


extern "C" {
	JNIEXPORT void JNICALL Java_com_technotecture_TacitAndroid_Native_create(JNIEnv* env, jobject thiz);
	JNIEXPORT void JNICALL Java_com_technotecture_TacitAndroid_Native_update(JNIEnv* env, jobject thiz);
	JNIEXPORT void JNICALL Java_com_technotecture_TacitAndroid_Native_destroy(JNIEnv* env, jobject thiz);
};


class tpWindowAndroid : public tpWindow {
public:

	TP_TYPE_DECLARE

	tpWindowAndroid( tpWindowTraits *traits ) {
		
	//	android_createDisplaySurface();
		
	}

	virtual bool show(bool doShow) { return true; }

	virtual void destroy() {}

	virtual void update() {}

	// virtual tpVec2i getSize() const {}
	// virtual void setSize(tpInt w, tpInt h) {}


	// virtual tpVec2i getClientAreaSize() const;
	// virtual void setClientAreaSize(tpUInt w, tpUInt h);


	// void setCaption(const tpString& caption);

	// tpRawPtr getWindow() { return window; }

	// void setContext(tpRenderContext *context);


protected:

	// NSWindow *window;
	// NSObject *delegate;

	virtual ~tpWindowAndroid();
	
	//android_createDisplaySurface();

};

struct AndroidViewer {
	
	tpRefPtr<tpViewer> viewer;
	
	AndroidViewer()
	{
		
		LOGI("Working?");
		
		viewer = new tpViewer();
		tpLogNotify("constructor called");
	}

};

JNIEXPORT void JNICALL
Java_com_technotecture_TacitAndroid_Native_create(JNIEnv* env, jobject thiz)
{
	AndroidViewer* av = new AndroidViewer;
	jclass cls = env->GetObjectClass(thiz);
	jfieldID fid = env->GetFieldID(cls, "handle", "J");
	env->SetLongField(thiz, fid, (long)av);
}

JNIEXPORT void JNICALL
Java_com_technotecture_TacitAndroid_Native_update(JNIEnv* env, jobject thiz)
{
	jclass cls = env->GetObjectClass(thiz);
	jfieldID fid = env->GetFieldID(cls, "handle", "J");
	AndroidViewer* av(reinterpret_cast<AndroidViewer*>(env->GetLongField(thiz, fid)));
}

JNIEXPORT void JNICALL
Java_com_technotecture_TacitAndroid_Native_destroy(JNIEnv* env, jobject thiz)
{
	jclass cls = env->GetObjectClass(thiz);
	jfieldID fid = env->GetFieldID(cls, "handle", "J");
	AndroidViewer* av(reinterpret_cast<AndroidViewer*>(env->GetLongField(thiz, fid)));
	
	if (av) delete av;
}

