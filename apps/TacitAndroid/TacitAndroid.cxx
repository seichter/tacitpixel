
#include <tp/window.h>

#include <android_native_app_glue.h>


struct TacitAndroidApp {
    struct android_app* app;
	
	
};

static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct TacitAndroidApp* engine = (struct TacitAndroidApp*)app->userData;
}


static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        // engine->animating = 1;
        // engine->state.x = AMotionEvent_getX(event, 0);
        // engine->state.y = AMotionEvent_getY(event, 0);
        return 1;
    }
    return 0;
}


void android_main(struct android_app* state) {

    struct TacitAndroidApp TacitAndroidApp;

    // Make sure glue isn't stripped.
    app_dummy();

    memset(&TacitAndroidApp, 0, sizeof(TacitAndroidApp));
    state->userData = &TacitAndroidApp;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
	
	
	
    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
		bool animating = true;
        while ((ident=ALooper_pollAll(animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }
			
            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                // engine_term_display(&engine);
                return;
            }
			
		}
	}
	
}
