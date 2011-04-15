#include "tpGLRenderSurfaceSDL.h"

#include "tpLog.h"


tpGLRenderSurfaceSDL::tpGLRenderSurfaceSDL(tpRenderSurfaceTraits* traits) : tpRenderSurface(traits)
{
	
	if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 ) 
	{
		tpLogError("%s Unable to initialize SDL: %s\n", __FUNCTION__, SDL_GetError());
	}
	
	
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	
	// Starting with SDL 1.2.10, passing in 0 will use the system's current resolution.
    unsigned int windowWidth = 0;
    unsigned int windowHeight = 0;
	
    // Passing in 0 for bitdepth also uses the system's current bitdepth. This works before 1.2.10 too.
    unsigned int bitDepth = 0;
	
    // If not linked to SDL 1.2.10+, then we must use hardcoded values
    const SDL_version* linked_version = SDL_Linked_Version();
    if(linked_version->major == 1 && linked_version->minor == 2)
    {
        if(linked_version->patch < 10)
        {
            windowWidth = 1280;
            windowHeight = 1024;
        }
    }
	
	tpLogMessage("%s using SDL %d.%d.%d",__FUNCTION__,linked_version->major,linked_version->minor,linked_version->patch);
	
	
	m_surface = SDL_SetVideoMode( windowWidth, windowHeight, bitDepth, SDL_OPENGL | SDL_FULLSCREEN ); 
	
	
}



tpVoid tpGLRenderSurfaceSDL::frame()
{
	
	SDL_Event event;
	
	while ( SDL_PollEvent(&event) )
	{
		// pass the SDL event into the viewers event queue
		//convertEvent(event, *(gw->getEventQueue()));
		
		switch (event.type) {
				
			case SDL_VIDEORESIZE:
				//SDL_SetVideoMode(event.resize.w, event.resize.h, bitDepth, SDL_OPENGL | SDL_RESIZABLE);
				///gw->resized(0, 0, event.resize.w, event.resize.h );
				break;
				
			case SDL_KEYUP:
				
				if (event.key.keysym.sym==SDLK_ESCAPE) setDone(true);
				if (event.key.keysym.sym=='f') 
				{
					SDL_WM_ToggleFullScreen(m_surface);
					//gw->resized(0, 0, screen->w, screen->h );
				}
				
				break;
				
			case SDL_QUIT:
				setDone(true);
		}
	}
}



tpBool tpGLRenderSurfaceSDL::makeCurrent() 
{
	//SDL_GL_Lock();
	return true;
}

tpBool tpGLRenderSurfaceSDL::swapBuffers()
{
	
	SDL_GL_SwapBuffers();
	
	return true;
}

tpString tpGLRenderSurfaceSDL::getString(tpUInt glenum)
{
	return tpString("");
}

tpBool tpGLRenderSurfaceSDL::show(tpBool doShow)
{
	return true;
}


tpGLRenderSurfaceSDL::~tpGLRenderSurfaceSDL()
{
}

class tpRenderSurfaceFactorySDL : public tpRenderSurfaceFactory {
	
public:
	
	tpRenderSurface* create(tpRenderSurfaceTraits* traits)
	{
		return new tpGLRenderSurfaceSDL(traits);
	}
};



tpRenderSurfaceFactoryLoader<tpRenderSurfaceFactorySDL> gs_sdl_surface;

