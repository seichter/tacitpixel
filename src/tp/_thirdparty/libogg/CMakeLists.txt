project(libogg)

include(CheckTypeSize)
include(CheckIncludeFiles)

check_include_files("inttypes.h" HAVE_INTTYPES_H)
check_include_files("stdint.h" HAVE_STDINT_H)
check_include_files("sys/types.h" HAVE_SYS_TYPES_H)

configure_file(include/ogg/config_types.h.in
	${CMAKE_BINARY_DIR}/include/ogg/config_types.h
	)

include_directories(${CMAKE_BINARY_DIR}/include include)

add_library(ogg STATIC
	src/bitwise.c
	src/framing.c
	${CMAKE_BINARY_DIR}/include/ogg/config_types.h
	)

set_target_properties(ogg
	PROPERTIES
	PROJECT_LABEL "Dependency libogg"
	)

