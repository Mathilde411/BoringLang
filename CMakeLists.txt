# Works with 3.15 and tested through 3.31
cmake_minimum_required(VERSION 3.20...3.31)

# Project name and a few useful settings. Other commands can pick up the results
project(
        BoringLang
        VERSION 0.1
        DESCRIPTION "An example project with CMake"
        LANGUAGES CXX)

include(FetchContent)
# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD 23)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.

    FetchContent_Declare(
            catch
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.7.1)
    FetchContent_MakeAvailable(catch)

    include(CTest)
    include(Catch)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()
# The compiled library code is here
add_subdirectory(src)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR BORING_LANG_BUILD_TESTING)
        AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
