if(BUILD_TESTING)
    function(add_algorithm_exercise target source include_dir test_name)
        add_executable(${target} ${source})
        target_include_directories(${target} PRIVATE ${include_dir})
        target_compile_features(${target} PRIVATE cxx_std_20)

        if(MSVC)
            target_compile_options(${target} PRIVATE /W4 /permissive-)
        else()
            target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        endif()

        add_test(NAME ${test_name} COMMAND ${target})
    endfunction()

    add_algorithm_exercise(
        stl_safety_exercise_tests
        exercises/stl_safety/tests/stl_safety_tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/exercises/stl_safety/include
        algorithm_stl_safety_exercise
    )
    add_algorithm_exercise(
        binary_search_exercise_tests
        exercises/binary_search/tests/binary_search_tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/exercises/binary_search/include
        algorithm_binary_search_exercise
    )
    add_algorithm_exercise(
        top_k_exercise_tests
        exercises/top_k/tests/top_k_tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/exercises/top_k/include
        algorithm_top_k_exercise
    )
endif()
