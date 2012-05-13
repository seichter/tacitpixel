add_subdirectory(show)
add_subdirectory(glut)

if(IOS)
	add_subdirectory(iosviewer)
endif()

if(ANDROID)
	add_subdirectory(TacitAndroid)
endif()
