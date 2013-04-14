if(NOT ANDROID AND NOT IOS)
	add_subdirectory(show)
	add_subdirectory(glut)
endif()

if(IOS)
	add_subdirectory(iosviewer)
endif()

if(ANDROID)
	add_subdirectory(TacitAndroid)
endif()
