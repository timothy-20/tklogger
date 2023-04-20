FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(logger_test logger_test.cpp)
target_link_libraries(logger_test gtest gtest_main tklogger)