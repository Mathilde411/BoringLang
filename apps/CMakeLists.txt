add_executable(BrOOinfuck main.cpp)

target_compile_features(BrOOinfuck_lib PUBLIC cxx_std_11)

target_link_libraries(BrOOinfuck_lib PRIVATE modern_library)