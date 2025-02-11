# cmake requirements
cmake_minimum_required(VERSION 3.14)

project(
    TSP
    VERSION 0.1
    DESCRIPTION "Solving travelling salesman problem"
    LANGUAGES CXX
)

# checks if we are in the main CMakeLists
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # ensures -std=c++xx instead of -std=g++xx for protability purposes
    set(CMAKE_CXX_EXTENSIONS OFF)

    # ensures C++ 17 is the standard
    set (CMAKE_CXX_STANDARD 17)

    # exports compile commands
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # displays the project source directory
    # message(STATUS "Project Source Directory: ${PROJECT_SOURCE_DIR}")

    # for the IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # compiler flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "TEST")
        add_compile_options(-Wall -Wextra -pedantic -g -fsanitize=address,undefined)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Test")
        add_compile_options(-Wall -Wextra -pedantic -O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-Wall -Wextra -pedantic -O3)
    endif()

    # linker flags
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "TEST")
        add_link_options(-g -fsanitize=address,undefined)
    endif()

    # uses CTest
    # NOTE: this needs to be done in the main CMakeLists
    include(CTest)

    # compile definition
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "TEST")
        add_compile_definitions(DEBUG)
    endif()


    # Docs only available in the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found")
    endif()
endif()

# FetchContent helps grabbing repos during the configure step
# NOTE: available after CMake 3.11
include(FetchContent)

FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 0041a40c1350ba702d475b9c4ad62da77caea164
)
FetchContent_MakeAvailable(fmt)

# all required packages
find_package(Boost REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED) # boost::program_options

# the compiled library code
add_subdirectory(src)

# the executable code
add_subdirectory(apps)

# checks if we are in the main CMakeLists
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING AND (CMAKE_BUILD_TYPE STREQUAL "Test" OR CMAKE_BUILD_TYPE STREQUAL "TEST"))
    add_subdirectory(tests)
endif()
