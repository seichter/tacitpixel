set(lib_name dlmalloc)

set(dlmalloc_srcs
	malloc.c
	malloc.h
	)

add_definitions(-DUSE_DL_PREFIX)

add_library(dlmalloc STATIC ${dlmalloc_srcs})