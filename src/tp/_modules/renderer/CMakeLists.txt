set(module_name tacit_gl)


include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/include
	)
	
set(shader_srcs
	shaders/mesh.vert
	shaders/mesh.frag
	)
	
source_group("Shader" FILES ${shader_srcs})

set_source_files_properties(${shader_srcs}
	PROPERTIES
	HEADER_FILE_ONLY TRUE
	)

set(srcs
	tpGL.h
	tpGL.cpp
	tpGLRenderer.h
	tpGLRenderer.cpp
	tpGLShader.cpp
	tpGLShader.h
	)

	
list(APPEND srcs ${shader_srcs})
find_package(X11)
option(TP_USE_X11 FALSE "Option to use X11")


if(X11_FOUND AND TP_USE_X11)

	set(TPGL_USE_X11 TRUE)
	list(APPEND srcs
		${CMAKE_CURRENT_SOURCE_DIR}/tpGLRenderSurfaceX11.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/tpGLRenderSurfaceX11.h
		)
	include_directories(${X11_INCLUDE_DIR})
endif()	

#
# Render Surface Implementations
#
if(WIN32)
	list(APPEND srcs
		${CMAKE_CURRENT_SOURCE_DIR}/tpGLRenderSurfaceWin32.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/tpGLRenderSurfaceWin32.h
		)
endif()


if(APPLE)
	find_package(OpenGL)
	list(APPEND srcs
		${CMAKE_CURRENT_SOURCE_DIR}/tpGLRenderSurfaceCocoa.mm
		${CMAKE_CURRENT_SOURCE_DIR}/tpGLRenderSurfaceCocoa.h
		)
endif()



configure_file(tpGLConfig.h.in
	${CMAKE_BINARY_DIR}/include/tp/tpGLConfig.h
	@ONLY
	)


add_library(${module_name} MODULE
	${srcs}
	)

add_custom_command(TARGET ${module_name}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
	)

set_target_properties(${module_name} 
	PROPERTIES
	OUTPUT_NAME ${module_name}
	)
	
target_link_libraries(${module_name}
	tacit
	)
	
if(X11_FOUND AND TP_USE_X11)
	target_link_libraries(${module_name} ${X11_LIBRARIES})
endif()


if(APPLE)
	target_link_libraries(${module_name}
		"-framework Cocoa" 
		${OPENGL_LIBRARIES}
	)
endif()

if(SDL_FOUND)
	target_link_libraries(${module_name} ${SDL_LIBRARY})
endif()

set_target_properties(${module_name}
	PROPERTIES
	PREFIX ""
	PROJECT_LABEL "Plugin OpenGL"
)
	
if(CMAKE_CROSSCOMPILING AND MSVC)
	set_target_properties(${module_name}
		PROPERTIES
		LINK_FLAGS /nodefaultlib:secchk.lib
	)
endif()


if(BUILD_INSTALLTARGET)
	set(MODULE_PATH lib)
	if (WIN32)
		set(MODULE_PATH bin)
	endif()
	
	install(TARGETS ${module_name}
		RUNTIME DESTINATION bin COMPONENT core
		LIBRARY DESTINATION ${MODULE_PATH} COMPONENT core
		ARCHIVE DESTINATION lib COMPONENT dev
		)		
endif()
