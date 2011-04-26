cmake_minimum_required(VERSION 2.8)

project(tacitpixel)

set(TP_VERSION_MAJOR 3)
set(TP_VERSION_MINOR 2)
set(TP_VERSION_BUILD 0)

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

# traverse down
add_subdirectory(src)

# build doxygen docs
find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(TP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
	set(DOXY_CONFIG ${CMAKE_BINARY_DIR}/etc/tacit.dox)
	configure_file(etc/tacit.dox.in
		${DOXY_CONFIG}
		IMMEDIATE
		@ONLY
		)
	 add_custom_target("docs" ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG})
endif()


