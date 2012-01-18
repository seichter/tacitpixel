set(module_name tacit_glsurface)

set(srcs)
set(ext_libs)

if(APPLE)
	list(APPEND srcs
		rendersurface_cocoa.h
		rendersurface_cocoa.mm
	)

	find_package(OpenGL REQUIRED)

	set(ext_libs "-framework Cocoa" ${OPENGL_LIBRARIES})

endif()


find_package(X11)

option(TP_USE_X11 FALSE "Option to use X11")
if(X11_FOUND AND TP_USE_X11)
	set(TPGL_USE_X11 TRUE)
	list(APPEND srcs
		rendersurface_x11.cxx
		rendersurface_x11.h
		)
	include_directories(${X11_INCLUDE_DIR})
endif()

if(WIN32)
	list(APPEND srcs
		rendersurface_w32.cpp
		rendersurface_w32.h
		)
endif()

#add_library(${module_name} MODULE ${srcs})
#
#set_target_properties(${module_name}
#	PROPERTIES
#	PREFIX ""
#	PROJECT_LABEL "Plugin OpenGL RenderSurface"
#)
#
#target_link_libraries(${module_name}
#	tacit ${ext_libs}
#	)

