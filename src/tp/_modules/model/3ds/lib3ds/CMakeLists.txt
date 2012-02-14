set(LIB_NAME lib3ds)

set(SRCS
	lib3ds.h
	lib3ds_atmosphere.c
	lib3ds_background.c
	lib3ds_camera.c
	lib3ds_chunk.c
	lib3ds_chunktable.c
	lib3ds_file.c
	lib3ds_impl.h
	lib3ds_io.c
	lib3ds_light.c
	lib3ds_material.c
	lib3ds_math.c
	lib3ds_matrix.c
	lib3ds_mesh.c
	lib3ds_node.c
	lib3ds_quat.c
	lib3ds_shadow.c
	lib3ds_track.c
	lib3ds_util.c
	lib3ds_vector.c
	lib3ds_viewport.c
	)


set_source_files_properties(
	${SRCS}
	PROPERTIES
	COMPILE_FLAGS "-DLIB3DS_STATIC"
	)

if(NOT WIN32)
	add_definitions(-fPIC)
endif(NOT WIN32)


add_library(${LIB_NAME} STATIC ${SRCS})

set_target_properties(${LIB_NAME}
	PROPERTIES
	PREFIX ""
	PROJECT_LABEL "Dependency lib3ds"
	)

