set(lib_name minizip)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../zlib
	${CMAKE_BINARY_DIR}/include
	)

set(mz_srcs
#	minizip.c
	ioapi.c
	mztools.c
	mztools.h
	unzip.c
	zip.c
	ioapi.h
#	minizip.pc.in
#	miniunz.c
	unzip.h
	zip.h
	crypt.h
	)

if(WIN32)
	list(APPEND mz_srcs
		iowin32.h
		iowin32.c
	)
endif()
	
add_library(minizip
	STATIC
	${mz_srcs}
	)
	
	
