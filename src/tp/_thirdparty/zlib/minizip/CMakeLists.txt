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
	iowin32.h
	iowin32.c
	unzip.h
	zip.h
	crypt.h
	)
	
add_library(minizip
	STATIC
	${mz_srcs}
	)
	
	