cmake_minimum_required(VERSION 2.8)

project(tacitpixel)

set(TP_VERSION_MAJOR 3)
set(TP_VERSION_MINOR 2)
set(TP_VERSION_BUILD 160911)

set(LIB_OUTPUT_PATH lib)
if (WIN32)
	set(LIB_OUTPUT_PATH bin)
endif()

# some general setup
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH "Executables go here")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Libraries go here")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${LIB_OUTPUT_PATH} CACHE PATH "Standalone DLLs go here")

# prevent direct installation into the host machine
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/staging CACHE PATH "Installation directory" FORCE)

if(NOT MINGW AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()


# shut up the compiler
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# traverse down
add_subdirectory(src)
add_subdirectory(etc)
add_subdirectory(apps)


