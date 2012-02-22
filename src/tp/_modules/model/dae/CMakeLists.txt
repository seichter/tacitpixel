set(module_name tacit_dae)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}
	)

set(srcs
	nodehandler_dae.cxx
	)

#set_source_files_properties(
#	${srcs}
#	PROPERTIES
#	COMPILE_FLAGS "-DLIB3DS_STATIC"
#	)



add_library(${module_name} MODULE
	${srcs}
	)



target_link_libraries(${module_name}
	tacit
	)

set_target_properties(${module_name}
	PROPERTIES
	PREFIX ""
	OUTPUT_NAME ${module_name}
	PROJECT_LABEL "Plugin Collada"
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
