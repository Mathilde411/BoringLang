#ifndef BORINGLANG_OBJECTHEADER_HPP
#define BORINGLANG_OBJECTHEADER_HPP

#include <cstdint>
#include "../structure/Class.hpp"
#include "Types.hpp"


namespace BoringLang::Memory {

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

    struct Flags {
        bool arrayed: 1;
        SlotSize arraySlotSize: 2;
        SpecialSection specialSection: 2;
        bool null: 1;
        uint8_t _reserved1: 2;
        uint8_t _reserved2;
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
        SlotOrHeader* _header;

    public:
        explicit ObjectHeader(SlotOrHeader* slot);

        [[nodiscard]] bool arrayed() const;

        void setArrayed(bool value);

        [[nodiscard]] SlotSize arraySlotSize() const;

        void setArraySlotSize(SlotSize value);

        [[nodiscard]] SpecialSection specialSection() const;

        void setSpecialSection(SpecialSection value);

        [[nodiscard]] bool null() const;

        void setNull(bool value);

        [[nodiscard]] uint64_t length() const;

        [[nodiscard]] uint64_t lengthWithHeader() const;

        [[nodiscard]] uint64_t slotLength() const;

        [[nodiscard]] uint64_t slotLengthWithHeader() const;

        [[nodiscard]] Slot* slotStart() const;

        void setLength(uint64_t value);

        [[nodiscard]] uint32_t classPointer() const;

        void setClassPointer(uint32_t value);
    };
}

#endif //BORINGLANG_OBJECTHEADER_HPP
