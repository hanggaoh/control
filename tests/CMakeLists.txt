find_package(Threads REQUIRED)

add_executable(bounded_queue_tests bounded_queue_tests.cpp)
target_link_libraries(
    bounded_queue_tests
    PRIVATE
        interview_concurrency
        Threads::Threads
)

if(MSVC)
    target_compile_options(bounded_queue_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(bounded_queue_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME bounded_queue COMMAND bounded_queue_tests)
