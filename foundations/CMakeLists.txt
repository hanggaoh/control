add_library(cpp_foundations INTERFACE)
target_include_directories(
    cpp_foundations
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(cpp_foundations INTERFACE cxx_std_20)

if(BUILD_TESTING)
    add_executable(cpp_foundations_tests tests/foundations_tests.cpp)
    target_link_libraries(cpp_foundations_tests PRIVATE cpp_foundations)

    if(MSVC)
        target_compile_options(cpp_foundations_tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(cpp_foundations_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_test(NAME cpp_foundations COMMAND cpp_foundations_tests)

    add_executable(copy_swap_exercise_tests
        exercises/copy_swap/tests/copy_swap_tests.cpp
    )
    target_include_directories(copy_swap_exercise_tests PRIVATE exercises/copy_swap/include)
    target_compile_features(copy_swap_exercise_tests PRIVATE cxx_std_20)

    add_executable(move_lifetime_exercise_tests
        exercises/move_lifetime/tests/move_lifetime_tests.cpp
    )
    target_include_directories(move_lifetime_exercise_tests PRIVATE exercises/move_lifetime/include)
    target_compile_features(move_lifetime_exercise_tests PRIVATE cxx_std_20)

    add_executable(unique_resource_exercise_tests
        exercises/unique_resource/tests/unique_resource_tests.cpp
    )
    target_include_directories(unique_resource_exercise_tests PRIVATE exercises/unique_resource/include)
    target_compile_features(unique_resource_exercise_tests PRIVATE cxx_std_20)

    add_executable(shared_ownership_exercise_tests
        exercises/shared_ownership/tests/shared_ownership_tests.cpp
    )
    target_include_directories(shared_ownership_exercise_tests PRIVATE exercises/shared_ownership/include)
    target_compile_features(shared_ownership_exercise_tests PRIVATE cxx_std_20)

    foreach(target copy_swap_exercise_tests move_lifetime_exercise_tests
                   unique_resource_exercise_tests shared_ownership_exercise_tests)
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4 /permissive-)
        else()
            target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        endif()
    endforeach()

    add_test(NAME foundation_copy_swap_exercise COMMAND copy_swap_exercise_tests)
    add_test(NAME foundation_move_lifetime_exercise COMMAND move_lifetime_exercise_tests)
    add_test(NAME foundation_unique_resource_exercise COMMAND unique_resource_exercise_tests)
    add_test(NAME foundation_shared_ownership_exercise COMMAND shared_ownership_exercise_tests)
endif()
