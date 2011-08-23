# build doxygen docs
find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(TP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
	configure_file(tacit.dox.in ${CMAKE_BINARY_DIR}/etc/tacit.dox
		IMMEDIATE @ONLY
		)
	 add_custom_target("docs" 
		${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/etc/tacit.dox
		)
endif()