#ifndef BORINGLANG_OBJECTHEADER_HPP
#define BORINGLANG_OBJECTHEADER_HPP

#include <cstdint>
#include "../structure/Class.hpp"


namespace BoringLang::Memory {

    typedef uint64_t Slot;

    enum SlotSize : uint8_t {
        BITS_64 = 0b00,
        BITS_32 = 0b01,
        BITS_16 = 0b10,
        BITS_8 = 0b11,
    };

    enum SpecialSection : uint8_t {
        NONE = 0b00,
        FREE = 0b01,
        BLOCKED = 0b10
    };

    struct Flags{
        bool arrayed : 1;
        SlotSize arraySlotSize : 2;
        SpecialSection specialSection : 2;
        bool null : 1;
        uint16_t _reserved : 10;
    };
    static_assert(sizeof(Flags) == 2);

    struct Header {
        Flags flags;
        uint16_t length;
        uint32_t classPointer;
    };
    static_assert(sizeof(Header) == 8);

    union SlotOrHeader {
        Slot slot;
        Header header;
    };
    static_assert(sizeof(SlotOrHeader) == 8);

    class ObjectHeader {
    private:
        Header _header;
        uint64_t _length;

        [[nodiscard]] Slot buildHeader() const;

    public:
        ObjectHeader(bool primitive, SlotSize slotSize, PrimitiveType primitiveType, uint8_t flags, uint64_t length,
                     uint32_t classPointer);

        explicit ObjectHeader(const SlotOrHeader* slot);



        void write(Slot* slot) const;
    };
}

#endif //BORINGLANG_OBJECTHEADER_HPP
