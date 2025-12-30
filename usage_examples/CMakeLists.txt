# usage_examples/CMakeLists.txt

file(GLOB EXAMPLE_SOURCES
    "*.cpp"
)

foreach(example_file ${EXAMPLE_SOURCES})
    get_filename_component(example_name ${example_file} NAME_WE)

    add_executable(${example_name} ${example_file})

    target_link_libraries(${example_name}
        PRIVATE iheay_lib
    )

    if(MSVC)
        target_compile_options(${example_name} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${example_name} PRIVATE -Wall -Wextra -Wpedantic -O2)
    endif()
endforeach()
