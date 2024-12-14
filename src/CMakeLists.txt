set(HEADER_LIST
    ../include/boringlang/memory/Object.hpp
)

add_library(BoringLang
        Object.cpp
        ${HEADER_LIST}
)
target_include_directories(BoringLang PUBLIC ../include)
#target_link_libraries(BoringLang PRIVATE Boost::boost)
target_compile_features(BoringLang PUBLIC cxx_std_23)
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})
