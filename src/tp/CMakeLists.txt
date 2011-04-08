# thirdparty stuff

add_subdirectory(thirdparty)

set(tp_include_path ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

include_directories(${tp_include_path} thirdparty ${CMAKE_BINARY_DIR}/include)

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
	${tp_include_path}/tp/globals.h
	${tp_include_path}/tp/types.h
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
	string.cxx
	system.cxx
	thread.cxx
	timer.cxx
	version.cxx
	)

add_library(tacitpixel SHARED
	${tp_srcs}
	${tp_hdrs}
	)

if(APPLE)
	target_link_libraries(tacitpixel
		"-framework CoreFoundation"
		)
endif()

target_link_libraries(tacitpixel
	pthread dl
	)
	
	
add_subdirectory(_testing)
