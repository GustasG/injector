cmake_minimum_required(VERSION 3.15)

project(injector
    VERSION 1.0.0
    DESCRIPTION "Dependency injection for C++"
    HOMEPAGE_URL "https://github.com/GustasG/injector"
    LANGUAGES CXX
)

option(INJECTOR_ENABLE_TESTING "Enable testing" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

add_library(${PROJECT_NAME} STATIC
    include/injector/detail/argument_resolver.hpp
    include/injector/detail/factory.hpp
    include/injector/detail/provider.hpp
    include/injector/detail/storage.hpp

    include/injector/injector.hpp
    include/injector/type_id.hpp    src/type_id.cpp
    include/injector/traits.hpp
    include/injector/errors.hpp     src/errors.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

if (INJECTOR_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()