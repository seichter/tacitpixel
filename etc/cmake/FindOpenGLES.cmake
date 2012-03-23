set(OPENGLES_FOUND FALSE)
set(OPENGLES1_FOUND FALSE)
set(OPENGLES2_FOUND FALSE)
set(EGL_FOUND FALSE)

set(OPENGLES_MODULE_DEBUG OFF)


if(IOS)
endif()

#
# EGL
# 
find_library(EGL_egl_LIBRARY
  NAMES EGL
  PATHS 
	/opt/graphics/OpenGL/lib
	/usr/openwin/lib
	/usr/shlib /usr/X11R6/lib
	/usr/lib
	)


	
#
# OpenGL ES 1.x
# 
find_path(OPENGLES1_INCLUDE_DIR GLES/gl.h
	PATHS
		/usr/openwin/share/include
		/opt/graphics/OpenGL/include
		/usr/X11R6/include
		/usr/include
	)


find_library(OPENGLES1_gl_LIBRARY
  NAMES GLES_CM
  PATHS 
	/opt/graphics/OpenGL/lib
	/usr/openwin/lib
	/usr/shlib /usr/X11R6/lib
	/usr/lib
	)
#
# OpenGL ES 2.x
# 
find_path(OPENGLES2_INCLUDE_DIR GLES2/gl.h
	PATHS
		/usr/openwin/share/include
		/opt/graphics/OpenGL/include
		/usr/X11R6/include
		/usr/include
	)


find_library(OPENGLES2_gl_LIBRARY
  NAMES GLESv2
  PATHS 
	/opt/graphics/OpenGL/lib
	/usr/openwin/lib
	/usr/shlib /usr/X11R6/lib
	/usr/lib
	)

if (OPENGLES1_gl_LIBRARY)
	set(OPENGLES1_FOUND ON)
	set(OPENGLES1_LIBRARIES ${OPENGLES1_gl_LIBRARY} ${EGL_egl_LIBRARY})
endif()

if (OPENGLES2_gl_LIBRARY)
	set(OPENGLES1_FOUND ON)
	set(OPENGLES2_LIBRARIES ${OPENGLES2_gl_LIBRARY} ${EGL_egl_LIBRARY})
endif()

if ( OPENGLES2_gl_LIBRARY OR OPENGLES1_gl_LIBRARY )
	set(OPENGLES_FOUND ON)
	set(OPENGLES_LIBRARIES ${OPENGLES1_gl_LIBRARY} ${OPENGLES2_gl_LIBRARY} ${EGL_egl_LIBRARY})
endif()

if(OPENGLES_MODULE_DEBUG)
	message(STATUS "EGL: ${EGL_egl_LIBRARY}")
	message(STATUS "OpenGL ES 1: ${OPENGLES1_gl_LIBRARY}")
	message(STATUS "OpenGL ES 2: ${OPENGLES2_gl_LIBRARY}")
endif()

mark_as_advanced(
	OPENGLES1_INCLUDE_DIR
	OPENGLES1_gl_LIBRARY
	OPENGLES2_INCLUDE_DIR
	OPENGLES2_gl_LIBRARY
	EGL_egl_LIBRARY
)