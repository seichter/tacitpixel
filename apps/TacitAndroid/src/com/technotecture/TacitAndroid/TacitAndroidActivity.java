package com.technotecture.TacitAndroid;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

class Native {

	/**
	 * Internal handle for the native parts of SURF-ES.
	 */
	protected long handle;
	
	private native void create();
	private native void update();
	private native void destroy();
	
	
	Native() {
		this.create();
	}
	
	public void finalize()
	{
		this.destroy();
	}
	
	public void render() 
	{
		this.update();
	}
	

	static {
		try {
			System.loadLibrary("tacit");
			System.loadLibrary("tacit-jni");
		} catch (UnsatisfiedLinkError e) {
			Log.e("TacitAndroid", e.getMessage());
		}
	}
}


class DemoGLSurfaceView extends GLSurfaceView {
    public DemoGLSurfaceView(Context context) {
        super(context);
        mRenderer = new DemoRenderer();
        setRenderer(mRenderer);
    }

    public boolean onTouchEvent(final MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
//            nativePause();
        }
        return true;
    }

    DemoRenderer mRenderer;

    private static native void nativePause();
}

class DemoRenderer implements GLSurfaceView.Renderer {
	
	Native glnative;
	
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        glnative = new Native();
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
        //gl.glViewport(0, 0, w, h);
        //nativeResize(w, h);
    }

    public void onDrawFrame(GL10 gl) {
        //
    	glnative.render();
    }
}

public class TacitAndroidActivity extends Activity {

	protected DemoGLSurfaceView mGLView;
	
	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        mGLView = new DemoGLSurfaceView(this);
        setContentView(mGLView);

    }
}