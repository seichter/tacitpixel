set(module_name tacit_png)

include_directories(
	${CMAKE_SOURCE_DIR}/src/tp/_thirdparty/zlib
	${CMAKE_SOURCE_DIR}/src/tp/_thirdparty/libpng
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/src/_ext
	${CMAKE_CURRENT_SOURCE_DIR}
	)
	
	
if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(srcs
	png_handler.cxx
	)
	

add_library(${module_name} MODULE
	${srcs}
	)
	

set_target_properties(${module_name} 
	PROPERTIES
	OUTPUT_NAME ${module_name}
	)
	
	
target_link_libraries(${module_name}
	tacit png z
	)
	
if(APPLE)
	target_link_libraries(${module_name}
		"-framework Cocoa" 
	)
endif()

set_target_properties(${module_name}
	PROPERTIES
	PREFIX ""
	PROJECT_LABEL "Plugin PNG"
)
	
if(CMAKE_CROSSCOMPILING AND MSVC)
	
	target_link_libraries(${module_name}
		libwcex
		)
			
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
