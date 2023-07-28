add_executable(BoringVM vm/main.cpp)
target_link_libraries(BoringVM PRIVATE BoringLang.VM)

add_executable(BoringCompiler compiler/main.cpp)
target_link_libraries(BoringCompiler PRIVATE BoringLang.Compiler)