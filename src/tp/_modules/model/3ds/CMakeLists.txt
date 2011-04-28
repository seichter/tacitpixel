add_subdirectory(lib3ds)

set(module_name tacit_3ds)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/lib3ds
	)

set(srcs
	main.cpp
	)
	

add_library(${module_name} MODULE
	${srcs}
	)
	
	
target_link_libraries(${module_name}
	tacit lib3ds
	)
	
if(APPLE)
	target_link_libraries(${module_name}
		"-framework Cocoa" 
	)
endif()

set_target_properties(${module_name}
	PROPERTIES
	PREFIX ""
	OUTPUT_NAME ${module_name}
	PROJECT_LABEL "Plugin 3DS"
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
