
find_package(GLUT)

if(GLUT_FOUND)

	include_directories(
		${CMAKE_SOURCE_DIR}/include
		${GLUT_INCLUDE_DIRS}
		)

	set(srcs
		glutviewer.cxx
		)

	add_executable(glutviewer
		MACOSX_BUNDLE
		${srcs}
		)


	target_link_libraries(glutviewer
		tacit
		${GLUT_LIBRARIES}
		)
		
endif()


