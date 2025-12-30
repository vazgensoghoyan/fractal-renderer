# Тест для vec3
add_executable(test_vec3 test_vec3.cpp)
target_link_libraries(test_vec3 PRIVATE iheay_lib gtest gtest_main pthread)
target_include_directories(test_vec3 PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/googletest/googletest/include
)
add_test(NAME TestVec3 COMMAND test_vec3)

# Тест для комплексных чисел
add_executable(test_complex test_complex.cpp)
target_link_libraries(test_complex PRIVATE iheay_lib gtest gtest_main pthread)
target_include_directories(test_complex PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/googletest/googletest/include
)
add_test(NAME TestComplex COMMAND test_complex)

# Тест для кватернионов
add_executable(test_quaternion test_quaternion.cpp)
target_link_libraries(test_quaternion PRIVATE iheay_lib gtest gtest_main pthread)
target_include_directories(test_quaternion PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/googletest/googletest/include
)
add_test(NAME TestQuaternion COMMAND test_quaternion)
