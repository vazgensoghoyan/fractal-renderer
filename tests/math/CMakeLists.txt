function(add_my_test TEST_NAME TEST_SRC)
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} PRIVATE iheay_lib gtest gtest_main pthread)
    target_include_directories(${TEST_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/tests/googletest/googletest/include
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

add_my_test(test_vec3 test_vec3.cpp)
add_my_test(test_complex test_complex.cpp)
add_my_test(test_quaternion test_quaternion.cpp)
