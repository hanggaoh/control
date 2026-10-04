cmake_minimum_required(VERSION 3.20)

project(catapult_cpp_interview_prep LANGUAGES CXX)

include(CTest)

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_subdirectory(exercises/01_unique_handle)
