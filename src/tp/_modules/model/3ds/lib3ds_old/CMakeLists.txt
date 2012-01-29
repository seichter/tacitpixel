set(LIB_NAME lib3ds) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

set(SRCS
	atmosphere.c
	background.c
	camera.c
	chunk.c
	ease.c
	file.c
	float.c
	io.c
	light.c
	material.c
	matrix.c
	mesh.c
	node.c
	quat.c
	shadow.c
	tcb.c
	tracks.c
	vector.c
	viewport.c)

if(NOT WIN32)
	add_definitions(-fPIC)
endif(NOT WIN32)

		
add_library(${LIB_NAME} STATIC ${SRCS})

set_target_properties(${LIB_NAME}
	PROPERTIES
	PREFIX ""
	PROJECT_LABEL "Dependency lib3ds"
	)

