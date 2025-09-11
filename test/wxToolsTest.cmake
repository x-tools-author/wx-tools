if(NOT WXT_BUILD_TESTS)
  return()
endif()

enable_testing()
include_directories(${CMAKE_SOURCE_DIR}/src/Common)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
set(TEST_LIBS ${PROJECT_LIBS} GTest::gtest GTest::gtest_main)
message(STATUS "[wxTools] Test Libraries: ${TEST_LIBS}")

set(test_src_dir ${CMAKE_CURRENT_LIST_DIR})
file(GLOB_RECURSE common_files ${CMAKE_SOURCE_DIR}/src/Common/*.*)

# CRC Test
add_executable(test_crc ${test_src_dir}/test_crc.cpp ${common_files})
target_link_libraries(test_crc ${TEST_LIBS})
set_target_properties(test_crc PROPERTIES FOLDER "Tests")
add_test(NAME test_crc COMMAND test_crc)

add_executable(test_decode ${test_src_dir}/test_decode.cpp ${common_files})
target_link_libraries(test_decode ${TEST_LIBS})
set_target_properties(test_decode PROPERTIES FOLDER "Tests")
add_test(NAME test_decode COMMAND test_decode)

add_executable(test_encode ${test_src_dir}/test_encode.cpp ${common_files})
target_link_libraries(test_encode ${TEST_LIBS})
set_target_properties(test_encode PROPERTIES FOLDER "Tests")
add_test(NAME test_encode COMMAND test_encode)

add_executable(test_table ${test_src_dir}/test_table.cpp ${common_files})
target_link_libraries(test_table ${TEST_LIBS})
set_target_properties(test_table PROPERTIES FOLDER "Tests")
add_test(NAME test_table COMMAND test_table)
