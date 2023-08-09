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

FetchContent_Declare(
        catch
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0)
FetchContent_MakeAvailable(catch)

add_executable(${${PROJECT_NAME}_CORE_TEST}
        test.cpp
        core/namespaceTests.cpp
        core/headerTests.cpp
)
target_link_libraries(${${PROJECT_NAME}_CORE_TEST} PUBLIC ${${PROJECT_NAME}_CORE_LIB} Catch2::Catch2WithMain)
add_test(NAME ${${PROJECT_NAME}_CORE_TEST} COMMAND ${${PROJECT_NAME}_CORE_TEST})
set_tests_properties(${${PROJECT_NAME}_CORE_TEST} PROPERTIES ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${${PROJECT_NAME}_OUTPUT}")

#add_executable(${${PROJECT_NAME}_VM_TEST}
#        test.cpp)
#target_link_libraries(${${PROJECT_NAME}_VM_TEST} PUBLIC ${${PROJECT_NAME}_VM_LIB} Catch2::Catch2WithMain)
#add_test(NAME ${${PROJECT_NAME}_VM_TEST} COMMAND ${${PROJECT_NAME}_VM_TEST})
#set_tests_properties(${${PROJECT_NAME}_VM_TEST} PROPERTIES ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${${PROJECT_NAME}_OUTPUT}")

#add_executable(${${PROJECT_NAME}_COMPILER_TEST}
#        test.cpp)
#target_link_libraries(${${PROJECT_NAME}_COMPILER_TEST} PUBLIC ${${PROJECT_NAME}_COMPILER_LIB} Catch2::Catch2WithMain)
#add_test(NAME ${${PROJECT_NAME}_COMPILER_TEST} COMMAND BoringLang.Compiler.Tests)
#set_tests_properties(${${PROJECT_NAME}_COMPILER_TEST} PROPERTIES ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${${PROJECT_NAME}_OUTPUT}")