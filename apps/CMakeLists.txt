add_executable(BoringVM vm/main.cpp)
target_compile_features(BoringVM PUBLIC cxx_std_11)
target_link_libraries(BoringVM PRIVATE BoringLang.VM)

add_executable(BoringCompiler compiler/main.cpp)
target_compile_features(BoringCompiler PUBLIC cxx_std_11)
target_link_libraries(BoringCompiler PRIVATE BoringLang.Compiler)