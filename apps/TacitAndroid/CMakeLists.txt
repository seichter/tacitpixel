
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(tacit-jni
	MODULE
	TacitAndroid.cxx
	)
	
target_link_libraries(tacit-jni tacit log)