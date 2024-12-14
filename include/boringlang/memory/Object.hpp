//
// Created by odoo on 13/12/24.
//

#ifndef BORINGLANG_OBJECT_HPP
#define BORINGLANG_OBJECT_HPP
#include <cstdint>


namespace BoringLang::Memory {

    typedef uint64_t Slot;

    enum PrimitiveType : uint8_t {
        POINTER = 0,
        INTEGER = 1,
        FLOAT = 2,
        CHARACTER = 3,

        FREE = 0b11110,
        BLOCKED = 0b11111
    };

    enum SlotSize : uint8_t {
        BITS_64 = 0,
        BITS_32 = 1,
        BITS_16 = 2,
        BITS_8 = 3,
    };

    class Layout {
    private:
        bool _primitive;
        SlotSize _slotSize;
        PrimitiveType _primitiveType;

    public:
        Layout(bool primitive, SlotSize slotSize, PrimitiveType primitiveType);

        [[nodiscard]] bool isPrimitive() const;

        [[nodiscard]] SlotSize getSlotSize() const;

        [[nodiscard]] PrimitiveType getPrimitiveType() const;

        [[nodiscard]] uint8_t build() const;

        static Layout load(uint8_t layout);
    };

    class Object {
    private:
        Layout _layout;
        uint8_t _flags;
        uint64_t _length;
        uint32_t _classPointer;

    public:
        Object(bool primitive, SlotSize slotSize, PrimitiveType primitiveType, uint8_t flags, uint64_t length,
               uint32_t classPointer);

        [[nodiscard]] bool isPrimitive() const;
        [[nodiscard]] SlotSize getSlotSize() const;
        [[nodiscard]] PrimitiveType getPrimitiveType() const;
        [[nodiscard]] uint64_t getLength() const;
        [[nodiscard]] uint32_t getClassPointer() const;

        [[nodiscard]] Slot buildHeader() const;
    };
}

#endif //BORINGLANG_OBJECT_HPP
