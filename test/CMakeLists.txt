cmake_minimum_required(VERSION 3.15)

project(injector-tests
    VERSION 1.0.0
    LANGUAGES CXX
)

FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(injector-tests
    injector_with_function.cpp
    injector_with_value.cpp)

target_link_libraries(injector-tests injector gtest gmock gtest_main)

gtest_discover_tests(injector-tests)