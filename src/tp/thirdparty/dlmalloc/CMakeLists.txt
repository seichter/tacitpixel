set(lib_name dlmalloc)

set(dlmalloc_srcs
	malloc.c
	malloc.h
	)
	
add_library(dlmalloc STATIC ${dlmalloc_srcs})