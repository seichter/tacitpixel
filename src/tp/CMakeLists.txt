
set(tp_include_path ${CMAKE_CURRENT_SOURCE_DIR}/../../include)


include_directories(${tp_include_path})

set(tp_hdrs
	${tp_include_path}/tp/allocator.h
	${tp_include_path}/tp/array.h
	${tp_include_path}/tp/globals.h
	${tp_include_path}/tp/types.h
	)

set(tp_srcs
	allocator.cxx
	hash.cxx
	referenced.cxx
	string.cxx
	)

add_library(tacitpixel
	${tp_srcs}
	${tp_hdrs}
	)
	
	
add_subdirectory(_testing)