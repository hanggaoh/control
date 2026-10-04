add_library(unique_handle INTERFACE)
target_include_directories(
    unique_handle
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(unique_handle INTERFACE cxx_std_20)

if(BUILD_TESTING)
    add_executable(unique_handle_tests tests/unique_handle_tests.cpp)
    target_link_libraries(unique_handle_tests PRIVATE unique_handle)
    target_compile_features(unique_handle_tests PRIVATE cxx_std_20)

    if(MSVC)
        target_compile_options(unique_handle_tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(unique_handle_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_test(NAME exercise_01_unique_handle COMMAND unique_handle_tests)
endif()
