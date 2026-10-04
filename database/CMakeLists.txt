if(BUILD_TESTING)
    add_executable(transaction_guard_exercise_tests
        exercises/transaction_guard/tests/transaction_guard_tests.cpp
    )
    target_include_directories(transaction_guard_exercise_tests PRIVATE
        exercises/transaction_guard/include
    )
    target_compile_features(transaction_guard_exercise_tests PRIVATE cxx_std_20)

    if(MSVC)
        target_compile_options(transaction_guard_exercise_tests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(transaction_guard_exercise_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_test(NAME database_transaction_guard_exercise COMMAND transaction_guard_exercise_tests)
endif()
