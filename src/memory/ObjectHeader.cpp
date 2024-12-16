#include "boringlang/memory/ObjectHeader.hpp"


namespace BoringLang::Memory {

    ObjectHeader::ObjectHeader(const bool primitive, const SlotSize slotSize, const PrimitiveType primitiveType,
                   const uint8_t flags, const uint64_t length,
                   const uint32_t classPointer) : _layout(primitive, slotSize, primitiveType), _flags(flags),
                                                  _length(length),
                                                  _classPointer(classPointer) {
    }

    ObjectHeader::ObjectHeader(const SlotOrHeader* slot) :
            _length(slot->header.length), _header(slot->header){
        if(_length == 0xFFFF)
            _length = (slot + 1)->slot;
    }

    bool ObjectHeader::isPrimitive() const {
        return _layout.isPrimitive();
    }

    SlotSize ObjectHeader::getSlotSize() const {
        return _layout.getSlotSize();
    }

    PrimitiveType ObjectHeader::getPrimitiveType() const {
        return _layout.getPrimitiveType();
    }

    uint64_t ObjectHeader::getLength() const {
        return _length;
    }

    uint64_t ObjectHeader::getSlotLength() const {
        uint8_t ratio;
        switch(_layout.getSlotSize()) {
            case BITS_64:
                ratio = 1;
                break;
            case BITS_32:
                ratio = 2;
                break;
            case BITS_16:
                ratio = 4;
                break;
            case BITS_8:
                ratio = 8;
                break;
        }

        return (_length / ratio) + (_length % ratio > 0);
    }

    uint32_t ObjectHeader::getClassPointer() const {
        return _classPointer;
    }

    Slot ObjectHeader::buildHeader() const {
        return (static_cast<uint64_t>(_layout.build()) << 56) |
               (static_cast<uint64_t>(_flags) << 48) |
               (_layout.isPrimitive() ?
                    _length & 0xFFFFFFFFFFFF :
                    (_length >= 0xFFFF ? _length : 0xFFFF) << 32 | _classPointer
               );
    }

    void ObjectHeader::write(Slot* slot) const {
        *slot = this->buildHeader();
        if (!_layout.isPrimitive() && _length == 0xFFFF)
            *(slot + 1) = _length;
    }

    uint8_t ObjectHeader::getHeaderSize() const {
        return _layout.isPrimitive() || _length != 0xFFFF ? 1 : 2;
    }


}
