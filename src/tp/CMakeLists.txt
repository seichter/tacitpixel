# thirdparty stuff

add_subdirectory(_thirdparty)

set(tp_name tacit)

set(tp_include_path ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

include_directories(${tp_include_path}
	_thirdparty
	_thirdparty/libogg/include
	_thirdparty/libvorbis/include
	${CMAKE_BINARY_DIR}/include)

find_package(X11)
find_package(OpenGL)
find_package(OpenAL)
#find_package(Cocoa)

include(CheckIncludeFiles)

CHECK_INCLUDE_FILES("stdint.h" HAVE_STDINT_H)
CHECK_INCLUDE_FILES("malloc.h" HAVE_MALLOC_H)
CHECK_INCLUDE_FILES("sys/param.h;sys/mount.h" HAVE_SYS_MOUNT_H)
CHECK_INCLUDE_FILES("pthread.h" HAVE_PTHREAD_H)
CHECK_INCLUDE_FILES("dlfcn.h" HAVE_DLFCN_H)
CHECK_INCLUDE_FILES("time.h" HAVE_TIME_H)
CHECK_INCLUDE_FILES("sys/time.h" HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILES("stdio.h" HAVE_STDIO_H)

include(CheckSymbolExists)
check_symbol_exists(clock_gettime sys/time.h HAVE_CLOCK_GETTIME)
check_symbol_exists(gettimeofday sys/time.h HAVE_GETTIMEOFDAY)


set(CMAKE_SKIP_BUILD_RPATH FALSE)


set(tp_impl_srcs)
set(tp_impl_libs)


message(STATUS "Build ${TP_VERSION_BUILD}")


set(tp_hdrs
	${tp_include_path}/tp/allocator.h
	${tp_include_path}/tp/array.h
	${tp_include_path}/tp/arguments.h
	${tp_include_path}/tp/chunk.h
	${tp_include_path}/tp/event.h
	${tp_include_path}/tp/field.h
	${tp_include_path}/tp/file.h
	${tp_include_path}/tp/functoid.h
	${tp_include_path}/tp/globals.h
	${tp_include_path}/tp/egl.h
	${tp_include_path}/tp/font.h
	${tp_include_path}/tp/gl.h
	${tp_include_path}/tp/hash.h
	${tp_include_path}/tp/image.h
	${tp_include_path}/tp/imagehandler.h
	${tp_include_path}/tp/imageop.h
	${tp_include_path}/tp/io.h
	${tp_include_path}/tp/log.h
	${tp_include_path}/tp/logutils.h
	${tp_include_path}/tp/list.h
	${tp_include_path}/tp/library.h
	${tp_include_path}/tp/map.h
	${tp_include_path}/tp/math.h
	${tp_include_path}/tp/matrix.h
	${tp_include_path}/tp/mat.h
	${tp_include_path}/tp/matop.h
	${tp_include_path}/tp/module.h
	${tp_include_path}/tp/mutex.h
#	${tp_include_path}/tp/opengl.h
	${tp_include_path}/tp/pixelformat.h
	${tp_include_path}/tp/pair.h
	${tp_include_path}/tp/quat.h
	${tp_include_path}/tp/referenced.h
	${tp_include_path}/tp/refptr.h
	${tp_include_path}/tp/rs232.h
	${tp_include_path}/tp/rtl.h
	${tp_include_path}/tp/rtti.h
	${tp_include_path}/tp/socket.h
	${tp_include_path}/tp/sort.h
	${tp_include_path}/tp/sound.h
	${tp_include_path}/tp/scopeptr.h
	${tp_include_path}/tp/stack.h
	${tp_include_path}/tp/stream.h
	${tp_include_path}/tp/string.h
	${tp_include_path}/tp/stringtokenizer.h
	${tp_include_path}/tp/stringformater.h
	${tp_include_path}/tp/stringutils.h
	${tp_include_path}/tp/system.h
	${tp_include_path}/tp/thread.h
	${tp_include_path}/tp/timer.h
	${tp_include_path}/tp/types.h
	${tp_include_path}/tp/vec.h
	${tp_include_path}/tp/version.h
	${tp_include_path}/tp/viewer.h

	)

if(OPENGL_FOUND)
	set(TP_USE_OPENGL ON)
	list(APPEND tp_impl_srcs
		_impl/renderer_gl1.cxx
		_impl/renderer_gl1.h
		)
	include_directories(${OPENGL_INCLUDE_DIRS})
	list(APPEND tp_impl_libs
		${OPENGL_LIBRARIES}
		)
endif()

if(OPENAL_FOUND)
	set(TP_USE_OPENAL ON)
#	list(APPEND tp_impl_srcs
#		_impl/renderer_gl1.cxx
#		_impl/renderer_gl1.h
#		)
	include_directories(${OPENAL_INCLUDE_DIR})
	list(APPEND tp_impl_libs
		${OPENAL_LIBRARY}
		)
endif()


if (APPLE)

	option(TP_USE_COCOA "Use Cocoa as a rendersurface backend" OFF)

	if (TP_USE_COCOA)
		list(APPEND tp_impl_srcs
			_impl/rendersurface_cocoa.mm
			_impl/rendersurface_cocoa.h
			_impl/rendercontext_cocoa.mm
			_impl/rendercontext_cocoa.h
			)

		list(APPEND tp_impl_libs
			"-framework Cocoa"
			"-framework OpenGL"
			)
	endif()
endif()

if(X11_FOUND)

	option(TP_USE_X11 "Use X11 as a rendersurface backend" OFF)

	if(TP_USE_X11)

		list(APPEND tp_impl_srcs
			_impl/rendersurface_x11.cxx
			_impl/rendersurface_x11.h
			_impl/renderbuffer_x11.cxx
			_impl/renderbuffer_x11.h
			)

		list(APPEND tp_impl_libs
			${X11_LIBRARIES}
			)

		# only now we add GLX
		if(OPENGL_FOUND)
			list(APPEND tp_impl_srcs
				_impl/rendercontext_glx.cxx
				_impl/rendercontext_glx.h
				)
		endif()

	endif()

endif()




set(tp_srcs
	arguments.cxx
	allocator.cxx
	chunk.cxx
	egl.cxx
	event.cxx
	file.cxx
	filename.cxx
	functoid.cxx
	font.cxx
#	gl.cxx
	hash.cxx
	image.cxx
	imagehandler.cxx
	imageop.cxx
	log.cxx
	library.cxx
	module.cxx
	mutex.cxx
#	opengl.cxx
	referenced.cxx
	rs232.cxx
	rtl.cxx
	socket.cxx
	string.cxx
	stringtokenizer.cxx
	system.cxx
	thread.cxx
	timer.cxx
	version.cxx
	viewer.cxx
	)

#
# Image handlers
#
list(APPEND tp_srcs
#	_modules/image/bmp/bmp_handler.cxx
#	_modules/image/jpg/jpg_handler.cxx
#	_modules/image/ppm/ppm_handler.cxx
	)


#
# Scenegraph
#
set(tp_hdrs_sg
	${tp_include_path}/tp/camera.h
	${tp_include_path}/tp/light.h
	${tp_include_path}/tp/material.h
	${tp_include_path}/tp/node.h
	${tp_include_path}/tp/nodeops.h
	${tp_include_path}/tp/object.h
	${tp_include_path}/tp/primitive.h
	${tp_include_path}/tp/renderable.h
	${tp_include_path}/tp/renderer.h
	${tp_include_path}/tp/rendercontext.h
	${tp_include_path}/tp/rendersurface.h
	${tp_include_path}/tp/texture.h
	${tp_include_path}/tp/transform.h
	${tp_include_path}/tp/traverser.h
	)

set(tp_srcs_sg
	camera.cxx
	light.cxx
	material.cxx
	node.cxx
	nodeops.cxx
	object.cxx
	primitive.cxx
	renderable.cxx
	renderer.cxx
	rendercontext.cxx
	rendersurface.cxx
	sound.cxx
	texture.cxx
	transform.cxx
	traverser.cxx
	)


configure_file(config.h.in
	${CMAKE_BINARY_DIR}/include/tp/config.h
	@ONLY IMMEDIATE
	)


add_library(${tp_name} SHARED
	${tp_srcs}
	${tp_hdrs}
	${tp_hdrs_sg}
	${tp_srcs_sg}
	${tp_impl_srcs}
	)


target_link_libraries(${tp_name} ${tp_impl_libs})

set_target_properties(${tp_name}
	PROPERTIES
	COMPILE_DEFINITIONS TP_BUILD_LIBRARY=1
	)

target_link_libraries(${tp_name} jpeg png z ogg vorbis)

if(APPLE)
	target_link_libraries(${tp_name}
		"-framework CoreFoundation"
		)
endif()


if (HAVE_DLFCN_H)
	target_link_libraries(${tp_name} dl)
endif()

if (HAVE_CLOCK_GETTIME)
	target_link_libraries(${tp_name} rt)
endif()

if(HAVE_PTHREAD_H AND NOT ANDROID)
	target_link_libraries(${tp_name}
		pthread)
endif()


if (WIN32)
	target_link_libraries(${tp_name}
		wsock32
		)
endif()

add_subdirectory(_modules)
add_subdirectory(_testing)

option(TP_BUILD_UNITTESTS "Build unit tests" OFF)
if(TP_BUILD_UNITTESTS)
	add_subdirectory(_unit)
endif()

