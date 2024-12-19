set(HEADER_LIST
        ../include/boringlang/memory/Types.hpp
        ../include/boringlang/memory/ObjectHeader.hpp
        ../include/boringlang/memory/SlotOperation.hpp
        ../include/boringlang/structure/Class.hpp
        ../include/boringlang/structure/Namespaceable.hpp
        ../include/boringlang/structure/Namespace.hpp
)

add_library(BoringLang
        ${HEADER_LIST}
        memory/ObjectHeader.cpp
        structure/Class.cpp
        structure/Namespace.cpp
        structure/Namespaceable.cpp
)
target_include_directories(BoringLang PUBLIC ../include)
#target_link_libraries(BoringLang PRIVATE Boost::boost)
target_compile_features(BoringLang PUBLIC cxx_std_23)
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST})

