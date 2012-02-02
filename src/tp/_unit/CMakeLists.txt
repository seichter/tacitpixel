# add GTest
add_subdirectory(gtest)

# includes
include_directories(
	gtest/include 
	../../include 
	${CMAKE_BINARY_DIR}/include
	)

add_executable(tptest_000 tptest_000.cxx)
target_link_libraries(tptest_000 gtest tacit)