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

file(GLOB CORE_HEADER_LIST CONFIGURE_DEPENDS "${BoringLang_SOURCE_DIR}/include/core/boringlang/core/*.hpp")
add_library(BoringLang.Core
        SHARED
        ${CORE_HEADER_LIST}
        core/Class.cpp
        core/Namespace.cpp
        core/Namespaceable.cpp
        core/NamespacePath.cpp
        core/Method.cpp )
target_include_directories(BoringLang.Core PUBLIC ../include/core)


file(GLOB VM_HEADER_LIST CONFIGURE_DEPENDS "${BoringLang_SOURCE_DIR}/include/vm/boringlang/vm/*.hpp")
add_library(BoringLang.VM
        SHARED
        ${VM_HEADER_LIST}
        vm/VM.cpp
        vm/interpreter/Interpreter.cpp
        vm/memory/MemoryManager.cpp )
target_link_libraries(BoringLang.VM PUBLIC BoringLang.Core)
target_include_directories(BoringLang.VM  PUBLIC ../include/vm)

file(GLOB COMPILER_HEADER_LIST CONFIGURE_DEPENDS "${BoringLang_SOURCE_DIR}/include/compiler/boringlang/compiler/*.hpp")
add_library(BoringLang.Compiler
        SHARED
        ${COMPILER_HEADER_LIST}
        compiler/Compiler.cpp )
target_link_libraries(BoringLang.Compiler PUBLIC BoringLang.Core)
target_include_directories(BoringLang.Compiler  PUBLIC ../include/compiler)
