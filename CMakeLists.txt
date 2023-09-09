cmake_minimum_required(VERSION 3.15)
project(SupremeFTP)

add_subdirectory(include)
add_subdirectory(src)

option(BUILD_TEST "whether or not to build test" ON)


if(BUILD_TEST)
    message(STATUS "building the tests ...")
    add_subdirectory(test)
endif()


