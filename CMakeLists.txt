cmake_minimum_required(VERSION 3.15)
project(rt-scheduler VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find pthreads
find_package(Threads REQUIRED)

# --- Main library (shared between app and tests) ---
add_library(scheduler_lib
    src/Scheduler.cpp
    src/Event.cpp
)
target_include_directories(scheduler_lib PUBLIC include)
target_link_libraries(scheduler_lib Threads::Threads)

# --- Main executable ---
add_executable(rt_scheduler src/main.cpp)
target_link_libraries(rt_scheduler scheduler_lib)

# --- Google Test (fetched automatically) ---
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(run_tests tests/test_scheduler.cpp)
target_link_libraries(run_tests scheduler_lib GTest::gtest_main)
target_include_directories(run_tests PRIVATE include)

include(GoogleTestHelpers OPTIONAL)
add_test(NAME SchedulerTests COMMAND run_tests)
