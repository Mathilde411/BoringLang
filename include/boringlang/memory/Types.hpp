#ifndef BORINGLANG_TYPES_HPP
#define BORINGLANG_TYPES_HPP
#include <cstdint>


namespace BoringLang::Memory {
    typedef uint64_t Slot;
    typedef double Float;
    typedef int64_t Integer;
    typedef uint64_t Pointer;
    typedef char32_t Character;

    enum PrimitiveType : uint64_t {
        POINTER = 0b00, // HAS TO STAY 0 FOR EVERYTHING TO WORK !
        INTEGER = 0b01,
        FLOAT = 0b10,
        CHARACTER = 0b11
    };
}

#endif //BORINGLANG_TYPES_HPP
