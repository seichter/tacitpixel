
include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include)


set(srcs
	show.cxx
	)

add_executable(tacitshow
	MACOSX_BUNDLE
	${srcs})


target_link_libraries(tacitshow
	tacit
	)


