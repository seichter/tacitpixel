
add_definitions(-DNO_ERRNO_H)

set(zlib_srcs
	adler32.c
	compress.c
	crc32.c
	deflate.c
	gzio.c
	infback.c
	inffast.c
	inflate.c
	inftrees.c
	trees.c
	uncompr.c
	zutil.c
	crc32.h
	deflate.h
	inffast.h
	inffixed.h
	inflate.h
	inftrees.h
	trees.h
	zconf.h
	zlib.h
	zutil.h
	)

add_library(z STATIC ${zlib_srcs})

set_target_properties(z
	PROPERTIES
	PROJECT_LABEL "Dependency zlib"
)
