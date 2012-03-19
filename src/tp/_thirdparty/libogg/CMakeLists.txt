project(libogg)

include(CheckTypeSize)
include(CheckIncludeFiles)

check_include_files("stdint.h" HAVE_STDINT_H)
 
configure_file(include/ogg/config_types.h.in
	${CMAKE_BINARY_DIR}/include/ogg/config_types.h
	)

include_directories(${CMAKE_BINARY_DIR}/include include)

add_library(ogg STATIC
	src/bitwise.c
        src/framing.c
    )

set_target_properties(ogg
	PROPERTIES
	PROJECT_LABEL "Dependency libogg"
	)

