set(module_name tacit_obj)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}
	)
	
	
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(srcs
	glm.c
	glm.h
	glm_util.h
	main.cpp
	)
	

add_library(${module_name} MODULE
	${srcs}
	)
	

set_target_properties(${module_name} 
	PROPERTIES
	OUTPUT_NAME ${module_name}
	)
	
	
target_link_libraries(${module_name}
	tacit
	)
	
if(APPLE)
	target_link_libraries(${module_name}
		"-framework Cocoa" 
	)
endif()

set_target_properties(${module_name}
	PROPERTIES
	PREFIX ""
	PROJECT_LABEL "Plugin OBJ"
)
	
if(CMAKE_CROSSCOMPILING AND MSVC)
	set_target_properties(${module_name}
		PROPERTIES
		LINK_FLAGS /nodefaultlib:secchk.lib
	)
endif()

if(BUILD_INSTALLTARGET)
	set(MODULE_PATH lib)
	if (WIN32)
		set(MODULE_PATH bin)
	endif()
	
	install(TARGETS ${module_name}
		RUNTIME DESTINATION bin COMPONENT core
		LIBRARY DESTINATION ${MODULE_PATH} COMPONENT core
		ARCHIVE DESTINATION lib COMPONENT dev
		)		
endif()
