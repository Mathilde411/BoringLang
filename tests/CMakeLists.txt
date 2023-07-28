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
#     along with this program.  If not, see <https:#www.gnu.org/licenses/>.

FetchContent_Declare(
        catch
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0)
FetchContent_MakeAvailable(catch)

add_executable(BoringLang.Core.Tests
        test.cpp
        core/namespaceTests.cpp)
target_link_libraries(BoringLang.Core.Tests PUBLIC BoringLang.Core Catch2::Catch2WithMain)
add_test(NAME "BoringLang Core Tests" COMMAND BoringLang.Core.Tests)

#add_executable(BoringLang.VM.Tests
#        test.cpp)
#target_link_libraries(BoringLang.VM.Tests PUBLIC BoringLang.VM Catch2::Catch2WithMain)
#add_test(NAME "BoringLang VM Tests" COMMAND BoringLang.VM.Tests)

#add_executable(BoringLang.Compiler.Tests
#        test.cpp)
#target_link_libraries(BoringLang.Compiler.Tests PUBLIC BoringLang.Compiler Catch2::Catch2WithMain)
#add_test(NAME "BoringLang Compiler Tests" COMMAND BoringLang.Compiler.Tests)