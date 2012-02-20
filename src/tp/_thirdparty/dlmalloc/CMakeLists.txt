set(lib_name dlmalloc)


	
set(dlmalloc_srcs
	malloc.c
	malloc_.h
	)


set_source_files_properties(
	${dlmalloc_srcs}
	PROPERTIES
#	LANGUAGE C
	COMPILE_FLAGS "-DUSE_DL_PREFIX"
	)
	
	
add_library(dlmalloc STATIC ${dlmalloc_srcs})

set_target_properties(dlmalloc
	PROPERTIES
	PROJECT_LABEL "Dependency dlmalloc"
	)
