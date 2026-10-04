add_library(interview_concurrency INTERFACE)
target_include_directories(
    interview_concurrency
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_features(interview_concurrency INTERFACE cxx_std_20)
