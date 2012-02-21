project(libogg)

include(CheckTypeSize)
include(CheckIncludeFiles)

CHECK_INCLUDE_FILES("stdint.h" INCLUDE_INTTYPES_H)


check_type_size("int"			SIZEOF_INT)
check_type_size("unsigned int"		SIZEOF_UINT)
check_type_size("long"			SIZEOF_LONG)
check_type_size("unsigned long"		SIZEOF_ULONG)
check_type_size("long int"		SIZEOF_LONG_INT)
check_type_size("long long"		SIZEOF_LONG_LONG)

message(STATUS "Size of 'int' ${SIZEOF_INT}")

if(${SIZEOF_INT} EQUAL 2)
	set(SIZE16 "int")
	set(USIZE16 "unsigned int")
	set(SIZE32 "long")
	set(USIZE32 "unsigned long")
	set(SIZE64 "long long")
endif()

if(${SIZEOF_INT} EQUAL 4)
	set(SIZE16 "short")
	set(USIZE16 "unsigned short")
	set(SIZE32 "int")
	set(USIZE32 "unsigned long")
	set(SIZE64 "long long")
endif()


#typedef @SIZE16@ ogg_int16_t;
#typedef @USIZE16@ ogg_uint16_t;
#typedef @SIZE32@ ogg_int32_t;
#typedef @USIZE32@ ogg_uint32_t;
#typedef @SIZE64@ ogg_int64_t;

 

configure_file(include/ogg/config_types.h.in
	${CMAKE_BINARY_DIR}/include/ogg/config_types.h
	@ONLY
	)

include_directories(${CMAKE_BINARY_DIR}/include/ogg include)

add_library(ogg STATIC
	src/bitwise.c
        src/framing.c
    )

if(CMAKE_COMPILER_IS_GNUCC)
    set_target_properties(
        ogg
        PROPERTIES
        COMPILE_FLAGS -fPIC
        )
endif()

set_target_properties(ogg
	PROPERTIES
	PROJECT_LABEL "Dependency libogg"
	)

