cmake_minimum_required(VERSION 3.20)

project(catapult_cpp_interview_prep LANGUAGES CXX)

include(CTest)

add_subdirectory(foundations)
add_subdirectory(algorithms)
add_subdirectory(control_systems)
add_subdirectory(networking)
add_subdirectory(boost_asio)
add_subdirectory(mfc_legacy)
add_subdirectory(database)
add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_subdirectory(exercises/01_unique_handle)
