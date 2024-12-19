#include "boringlang/memory/ObjectHeader.hpp"


namespace BoringLang::Memory {
    ObjectHeader::ObjectHeader(SlotOrHeader* slot) : _header(slot) {
    }

    bool ObjectHeader::arrayed() const {
        return _header->header.flags.arrayed;
    }

    void ObjectHeader::setArrayed(const bool value) {
        _header->header.flags.arrayed = value;
    }

    SlotSize ObjectHeader::arraySlotSize() const {
        return _header->header.flags.arraySlotSize;
    }

    void ObjectHeader::setArraySlotSize(const SlotSize value) {
        _header->header.flags.arraySlotSize = value;
    }

    SpecialSection ObjectHeader::specialSection() const {
        return _header->header.flags.specialSection;
    }

    void ObjectHeader::setSpecialSection(const SpecialSection value) {
        _header->header.flags.specialSection = value;
    }

    bool ObjectHeader::null() const {
        return _header->header.flags.null;
    }

    void ObjectHeader::setNull(const bool value) {
        _header->header.flags.null = value;
    }

    uint64_t ObjectHeader::length() const {
        if (_header->header.length != 0xFFFF)
            return _header->header.length;
        return (_header + 1)->slot;
    }

    uint64_t ObjectHeader::lengthWithHeader() const {
        if (_header->header.length != 0xFFFF)
            return _header->header.length + 8;
        return (_header + 1)->slot + 16;
    }

    uint64_t ObjectHeader::slotLength() const {
        const uint64_t length = this->length();
        return length / 8 + (length % 8 ? 1 : 0);
    }

    uint64_t ObjectHeader::slotLengthWithHeader() const {
        const uint64_t length = this->lengthWithHeader();
        return length / 8 + (length % 8 ? 1 : 0);
    }

    Slot* ObjectHeader::slotStart() const {
        return &(_header + (_header->header.length == 0xFFFF ? 2 : 1))->slot;
    }

    void ObjectHeader::setLength(const uint64_t value) {
        if (value >= 0xFFFF) {
            _header->header.length = 0xFFFF;
            (_header + 1)->slot = value;
        } else
            _header->header.length = value;
    }

    uint32_t ObjectHeader::classPointer() const {
        return _header->header.classPointer;
    }

    void ObjectHeader::setClassPointer(uint32_t value) {
    }
}
