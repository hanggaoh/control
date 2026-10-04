if(BUILD_TESTING)
    function(add_control_exercise target source include_dir test_name)
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

    add_control_exercise(
        pid_controller_exercise_tests
        exercises/pid_controller/tests/pid_controller_tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/exercises/pid_controller/include
        control_pid_controller_exercise
    )
    add_control_exercise(
        low_pass_filter_exercise_tests
        exercises/low_pass_filter/tests/low_pass_filter_tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/exercises/low_pass_filter/include
        control_low_pass_filter_exercise
    )
    add_control_exercise(
        hysteresis_exercise_tests
        exercises/hysteresis/tests/hysteresis_tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/exercises/hysteresis/include
        control_hysteresis_exercise
    )
endif()
