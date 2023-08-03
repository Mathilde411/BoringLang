#     BoringLang Project
#     Copyright (C) 2023  Mathilde Pascal
#
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Lesser General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU Lesser General Public License
#     along with this program.  If not, see <https://www.gnu.org/licenses/>.

# Works with 3.20 and tested through 3.26
cmake_minimum_required(VERSION 3.20...3.26)

# Project name and a few useful settings. Other commands can pick up the results
project(
        BoringLang
        VERSION 0.1
        DESCRIPTION "A dumb object-oriented language"
        LANGUAGES CXX)


set(${PROJECT_NAME}_OUTPUT          "${CMAKE_BINARY_DIR}/output/${PROJECT_NAME}")

IF (WIN32)
    set(${PROJECT_NAME}_LIB_INSTALL     "")
    set(${PROJECT_NAME}_EXEC_INSTALL    "")
ELSE()
    set(${PROJECT_NAME}_LIB_INSTALL     "lib")
    set(${PROJECT_NAME}_EXEC_INSTALL    "bin")
ENDIF()
set(${PROJECT_NAME}_LICENSE_INSTALL "legal")

set(${PROJECT_NAME}_CORE_LIB        "${PROJECT_NAME}Core")
set(${PROJECT_NAME}_VM_LIB          "${PROJECT_NAME}VM")
set(${PROJECT_NAME}_COMPILER_LIB    "${PROJECT_NAME}Compiler")
set(${PROJECT_NAME}_VM_EXEC         "boringcompiler")
set(${PROJECT_NAME}_COMPILER_EXEC   "boringvm")
set(${PROJECT_NAME}_CORE_TEST       "${${PROJECT_NAME}_CORE_LIB}Test")
set(${PROJECT_NAME}_VM_TEST         "${${PROJECT_NAME}_VM_LIB}Test")
set(${PROJECT_NAME}_COMPILER_TEST   "${${PROJECT_NAME}_COMPILER_LIB}Test")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${${PROJECT_NAME}_OUTPUT})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_OUTPUT})
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)


    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

# FetchContent added in CMake 3.11, downloads during the configure step
# FetchContent_MakeAvailable was added in CMake 3.14; simpler usage
include(FetchContent)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)

install(FILES COPYING.md COPYING.LESSER.md
        DESTINATION ${${PROJECT_NAME}_LICENSE_INSTALL})

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
        AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

