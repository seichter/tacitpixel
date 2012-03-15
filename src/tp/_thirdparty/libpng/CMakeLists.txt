include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../zlib
	${CMAKE_BINARY_DIR}/include
	)

set(png_srcs
	png.c
	pngerror.c
	pnggccrd.c
	pngget.c
	pngmem.c
	pngpread.c
	pngread.c
	pngrio.c
	pngrtran.c
	pngrutil.c
	pngset.c
	#pngtest.c
	pngtrans.c
	pngvcrd.c
	pngwio.c
	pngwrite.c
	pngwtran.c
	pngwutil.c
	png.h
	pngconf.h
	)

add_library(png STATIC ${png_srcs})

set_target_properties(png
	PROPERTIES
	PROJECT_LABEL "Dependency libpng"
	)
