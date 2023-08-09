//     BoringLang Project
//     Copyright (C) 2023  Mathilde Pascal
//
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Lesser General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU Lesser General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "boringlang/core/util/TypesUtil.hpp"

using namespace BoringLang;

ObjectHeader::ObjectHeader(BvSlot* header) {
    BvSlot value = *header;
    if((value >> 63) == 0) {
        this->loadPrimitiveHeader(header);
    } else {
        if((value >> 56) == 0xFF) {
            this->loadSizeExtension(header);
            this->loadObjectHeader(header + 1, false);
        } else {
            this->loadObjectHeader(header, true);
        }
    }
}

ObjectHeader::ObjectHeader(PrimitiveType format, uint64_t byteSize) {
    _primitive = true;
    _doubleHeader = false;
    _format = format;
    _size = byteSize;
    _hash = 0;
    _classIndex = 0;
    _flags = 0;
}

ObjectHeader::ObjectHeader(uint64_t byteSize, uint32_t hash, uint32_t classIndex, uint8_t flags) {
    _primitive = false;
    _doubleHeader = (byteSize >= 0x7F);
    _format = NO_TYPE;
    _size = byteSize;
    _hash = hash;
    _classIndex = classIndex;
    _flags = flags;
}

void ObjectHeader::loadPrimitiveHeader(const BvSlot* slot) {
    BvSlot value = *slot;
    _primitive = true;
    _doubleHeader = false;
    _format = (PrimitiveType) (value >> 56);
    _size = value & 0xFFFFFFFFFFFFFFUL;
    _hash = 0;
    _classIndex = 0;
    _flags = 0;
}

void ObjectHeader::loadSizeExtension(const BvSlot* slot) {
    BvSlot value = *slot;
    _primitive = false;
    _doubleHeader = true;
    _size = value & 0xFFFFFFFFFFFFFF;
}

void ObjectHeader::loadObjectHeader(const BvSlot* slot, bool loadSize) {
    BvSlot value = *slot;
    _primitive = false;
    _format = NO_TYPE;
    if(loadSize) {
        _size = (value >> 56) & 0x7F;
        _doubleHeader = false;
    }
    _hash = (value >> 32) & 0xFFFFFF;
    _classIndex = value & 0xFFFFFF;
    _flags = (value >> 24) & 0xFF;
}

bool ObjectHeader::isPrimitive() const {
    return _primitive;
}

bool ObjectHeader::isSloted() const {
    return _primitive && ((_format & (1<<6)) != 0);
}

bool ObjectHeader::isDoubleHeader() const {
    return _doubleHeader;
}

PrimitiveType ObjectHeader::getFormat() const {
    return _format;
}

uint64_t ObjectHeader::getSize() const {
    return _size;
}

uint64_t ObjectHeader::getSlotSize() const {
    return (_size / 8) + ((_size % 8) > 0 ? 1 : 0);
}

uint32_t ObjectHeader::getHash() const {
    return _hash;
}

uint32_t ObjectHeader::getClassIndex() const {
    return _classIndex;
}

uint8_t ObjectHeader::getFlags() const {
    return _flags;
}

uint64_t ObjectHeader::getSlotSizeWithHeader() const {
    return (_doubleHeader ? 2 : 1) + this->getSlotSize();
}

void ObjectHeader::exportHeader(BvSlot* slot) {
    if(_primitive) {
        *slot = (((uint64_t)_format) << 56) | (_size & 0xFFFFFFFFFFFFFF);
    } else {
        uint64_t miniSize;
        if(_doubleHeader) {
            *slot = (0xFFULL << 56) | (_size & 0xFFFFFFFFFFFFFF);
            miniSize = 0x7FUL;
            slot++;
        } else {
            miniSize = _size;
        }
        *slot = (1ULL << 63) |
                (miniSize << 56) |
                (((uint64_t) _hash) << 32) |
                (((uint64_t) _flags) << 24) |
                ((uint64_t) _classIndex);
    }
}

BvSlot* ObjectHeader::nextObject(BvSlot* slot) {
    ObjectHeader header(slot);
    return slot + header.getSlotSizeWithHeader();
}
