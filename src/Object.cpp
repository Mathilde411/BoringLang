//
// Created by odoo on 13/12/24.
//

#include "boringlang/memory/Object.hpp"


namespace BoringLang::Memory {

    Layout::Layout(const bool primitive, const SlotSize slotSize,
                   const PrimitiveType primitiveType) : _primitive(primitive),
                                                        _slotSize(slotSize), _primitiveType(primitiveType) {
    }

    bool Layout::isPrimitive() const {
        return _primitive;
    }

    SlotSize Layout::getSlotSize() const {
        return _slotSize;
    }

    PrimitiveType Layout::getPrimitiveType() const {
        return _primitiveType;
    }

    uint8_t Layout::build() const {
        return (_primitive << 7) + (_slotSize << 5) + _primitiveType;
    }

    Layout Layout::load(const uint8_t layout) {
        return {
            static_cast<bool>(layout & 0b10000000),
            static_cast<SlotSize>(layout >> 5 & 0b11),
            static_cast<PrimitiveType>(layout & 0b111111)
        };
    }

    Object::Object(const bool primitive, const SlotSize slotSize, const PrimitiveType primitiveType,
                   const uint8_t flags, const uint64_t length,
                   const uint32_t classPointer) : _layout(primitive, slotSize, primitiveType), _flags(flags),
                                                  _length(length),
                                                  _classPointer(classPointer) {
    }

    bool Object::isPrimitive() const {
        return _layout.isPrimitive();
    }

    SlotSize Object::getSlotSize() const {
        return _layout.getSlotSize();
    }

    PrimitiveType Object::getPrimitiveType() const {
        return _layout.getPrimitiveType();
    }

    uint64_t Object::getLength() const {
        return _length;
    }

    uint32_t Object::getClassPointer() const {
        return _classPointer;
    }


}
