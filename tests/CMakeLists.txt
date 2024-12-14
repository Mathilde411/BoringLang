FetchContent_Declare(
        catch
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1)
FetchContent_MakeAvailable(catch)

# Tests need to be added as executables first
add_executable(BoringLangTests
        notest.cpp
)
target_compile_features(BoringLangTests PRIVATE cxx_std_23)
target_link_libraries(BoringLangTests PRIVATE BoringLang Catch2::Catch2)
add_test(NAME BoringLangTests COMMAND BoringLangTests)
