
set(tacit_libs tacit)

add_executable(test_000 test_000.cxx)
add_executable(test_001 test_001.cxx)
add_executable(test_002 test_002.cxx)
add_executable(test_003 test_003.cxx)
add_executable(test_004 test_004.cxx)
add_executable(test_005 test_005.cxx)
add_executable(test_006 test_006.cxx)

target_link_libraries(test_000 ${tp_name})
target_link_libraries(test_001 ${tp_name})
target_link_libraries(test_002 ${tp_name})
target_link_libraries(test_003 ${tp_name})
target_link_libraries(test_004 ${tp_name})
target_link_libraries(test_005 ${tp_name})
target_link_libraries(test_006 ${tp_name})