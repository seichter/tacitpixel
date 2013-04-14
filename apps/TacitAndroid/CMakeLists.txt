
include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

add_library(tacit-jni
	MODULE
	TacitAndroid.cxx
	)
	
target_link_libraries(tacit-jni tacit log)