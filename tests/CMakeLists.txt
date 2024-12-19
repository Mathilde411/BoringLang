
# Tests need to be added as executables first
add_executable(BoringLangTests
        memory/slotOperation.cpp
)
target_compile_features(BoringLangTests PRIVATE cxx_std_23)
target_link_libraries(BoringLangTests PRIVATE BoringLang Catch2::Catch2WithMain)
catch_discover_tests(BoringLangTests)
