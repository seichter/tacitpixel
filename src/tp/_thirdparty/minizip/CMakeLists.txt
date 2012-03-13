set(lib_name minizip)

include_directories(..)

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
	list(ADD mz_srcs
		iowin32.h
		iowin32.c
	)
endif()
	
add_library(minizip
	STATIC
	${mz_srcs}
	)
	
	
