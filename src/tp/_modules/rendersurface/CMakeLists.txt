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

add_library(${module_name} MODULE ${srcs})

set_target_properties(${module_name}
	PROPERTIES
	PREFIX ""
	PROJECT_LABEL "Plugin OpenGL"
)

target_link_libraries(${module_name}
	tacit ${ext_libs}
	)

