
file(GLOB CORE_HEADER_LIST CONFIGURE_DEPENDS "${BoringLang_SOURCE_DIR}/include/core/boringlang/core/*.hpp")
add_library(BoringLang.Core
        SHARED
        ${CORE_HEADER_LIST}
        core/Class.cpp
        core/Namespace.cpp
        core/Namespaceable.cpp
        core/NamespacePath.cpp
        core/Method.cpp )
target_link_libraries(BoringLang.Core Boost::algorithm)
target_include_directories(BoringLang.Core PUBLIC ../include/core)
target_compile_features(BoringLang.Core PUBLIC cxx_std_11)


file(GLOB VM_HEADER_LIST CONFIGURE_DEPENDS "${BoringLang_SOURCE_DIR}/include/vm/boringlang/vm/*.hpp")
add_library(BoringLang.VM
        SHARED
        ${VM_HEADER_LIST}
        vm/VM.cpp
        vm/interpreter/Interpreter.cpp
        vm/memory/MemoryManager.cpp )
target_link_libraries(BoringLang.VM PUBLIC BoringLang.Core)
target_include_directories(BoringLang.VM  PUBLIC ../include/vm)
target_compile_features(BoringLang.VM PUBLIC cxx_std_11)

file(GLOB COMPILER_HEADER_LIST CONFIGURE_DEPENDS "${BoringLang_SOURCE_DIR}/include/compiler/boringlang/compiler/*.hpp")
add_library(BoringLang.Compiler
        SHARED
        ${COMPILER_HEADER_LIST}
        compiler/Compiler.cpp )
target_link_libraries(BoringLang.Compiler PUBLIC BoringLang.Core)
target_include_directories(BoringLang.Compiler  PUBLIC ../include/compiler)
target_compile_features(BoringLang.Compiler PUBLIC cxx_std_11)
