cmake_minimum_required(VERSION 3.16)

project(CPAlgoTemplates LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_library(catch_amalgamated STATIC third_party/catch/catch_amalgamated.cpp)
target_include_directories(catch_amalgamated PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE LOCAL_TESTS CONFIGURE_DEPENDS test/local/*.cpp)
foreach(test_src IN LISTS LOCAL_TESTS)
    get_filename_component(test_name ${test_src} NAME_WE)
    file(RELATIVE_PATH test_rel ${CMAKE_CURRENT_SOURCE_DIR}/test/local ${test_src})
    get_filename_component(test_dir ${test_rel} DIRECTORY)

    add_executable(${test_name} ${test_src})
    target_include_directories(${test_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${test_name} PRIVATE catch_amalgamated)

    add_test(NAME ${test_name} COMMAND ${test_name} --reporter compact)
    if(test_dir)
        string(REPLACE "/" ";" test_labels "${test_dir}")
        set_tests_properties(${test_name} PROPERTIES LABELS "${test_labels}")
    endif()
endforeach()
