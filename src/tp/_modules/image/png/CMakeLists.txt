set(module_name tacit_png)


find_package(PNG)

set(module_libs)

if(PNG_FOUND)
	# if we found a implementation use it
	option(TP_USE_SYSTEM_PNG OFF)
	if (TP_USE_SYSTEM_PNG)
		include_directories(${PNG_INCLUDE_DIRS})
		set(module_libs ${PNG_LIBRARIES})
	endif()
else()
	# otherwise skip it
	include_directories(
		${CMAKE_SOURCE_DIR}/src/tp/_thirdparty/zlib
		${CMAKE_SOURCE_DIR}/src/tp/_thirdparty/libpng
		)
	set(module_libs png zlib)
endif()

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/include
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
	tacit ${module_libs}
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
