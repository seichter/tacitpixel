# thirdparty stuff

add_subdirectory(_thirdparty)

set(tp_include_path ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

include_directories(${tp_include_path} _thirdparty ${CMAKE_BINARY_DIR}/include)

include(CheckIncludeFiles)

CHECK_INCLUDE_FILES(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES(malloc.h HAVE_MALLOC_H)
CHECK_INCLUDE_FILES("sys/param.h;sys/mount.h" HAVE_SYS_MOUNT_H)
CHECK_INCLUDE_FILES("pthread.h" HAVE_PTHREAD_H)
CHECK_INCLUDE_FILES("dlfcn.h" HAVE_DLFCN_H)
CHECK_INCLUDE_FILES("time.h" HAVE_TIME_H)

set(CMAKE_SKIP_BUILD_RPATH FALSE)

configure_file(config.h.in
	${CMAKE_BINARY_DIR}/include/tp/config.h
	@ONLY
	)

set(tp_hdrs
	${tp_include_path}/tp/allocator.h
	${tp_include_path}/tp/array.h
	${tp_include_path}/tp/chunk.h
	${tp_include_path}/tp/field.h
	${tp_include_path}/tp/globals.h
	${tp_include_path}/tp/hash.h
	${tp_include_path}/tp/image.h
	${tp_include_path}/tp/log.h
	${tp_include_path}/tp/list.h	
	${tp_include_path}/tp/library.h
	${tp_include_path}/tp/map.h
	${tp_include_path}/tp/math.h
	${tp_include_path}/tp/matrix.h
	${tp_include_path}/tp/mat.h
	${tp_include_path}/tp/mutex.h
	${tp_include_path}/tp/quat.h	
	${tp_include_path}/tp/referenced.h
	${tp_include_path}/tp/rs232.h	
	${tp_include_path}/tp/socket.h	
	${tp_include_path}/tp/stack.h
	${tp_include_path}/tp/string.h
	${tp_include_path}/tp/system.h
	${tp_include_path}/tp/thread.h
	${tp_include_path}/tp/timer.h
	${tp_include_path}/tp/types.h
	${tp_include_path}/tp/version.h
	)

	
set(tp_srcs
	allocator.cxx
	chunk.cxx
	hash.cxx
	image.cxx
	log.cxx
	library.cxx
	mutex.cxx
	referenced.cxx
	rs232.cxx
	socket.cxx
	string.cxx
	system.cxx
	thread.cxx
	timer.cxx
	version.cxx
	)


#
# Scenegraph
#
set(tp_hdrs_sg
	${tp_include_path}/tp/node.h
	${tp_include_path}/tp/object.h
	${tp_include_path}/tp/transform.h
	${tp_include_path}/tp/traverser.h
	)
	
set(tp_srcs_sg
	node.cxx
	object.cxx
	transform.cxx
	traverser.cxx
	)

add_library(tacitpixel SHARED
	${tp_srcs}
	${tp_hdrs}
	${tp_hdrs_sg}
	${tp_srcs_sg}
	)
	
set_target_properties(tacitpixel
	PROPERTIES
	COMPILE_DEFINITIONS TP_BUILD_LIBRARY=1
	)

target_link_libraries(tacitpixel dlmalloc)

if(APPLE)
	target_link_libraries(tacitpixel
		"-framework CoreFoundation"
		)
endif()

if (UNIX)
	target_link_libraries(tacitpixel
		pthread dl
		)
endif()


if (WIN32)
	target_link_libraries(tacitpixel
		wsock32
		)
endif()


add_subdirectory(_testing)
