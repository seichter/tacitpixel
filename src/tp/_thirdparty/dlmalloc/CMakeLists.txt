set(lib_name dlmalloc)
	
set(dlmalloc_srcs
	malloc.c
	malloc_.h
	)

add_library(dlmalloc STATIC ${dlmalloc_srcs})

set_target_properties(
	dlmalloc
	PROPERTIES
	COMPILE_DEFINITIONS USE_DL_PREFIX
	)

set_target_properties(dlmalloc
	PROPERTIES
	PROJECT_LABEL "Dependency dlmalloc"
	)
