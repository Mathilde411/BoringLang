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

#include <cstring>
#include "boringlang/core/util/TypesUtil.hpp"

using namespace BoringLang;

ObjectHeader::ObjectHeader(const BvSlot* header) {
    BvSlot value = *header;
    if((value >> 63) == 0) {
        _primitive = true;
        _doubleHeader = false;
        _format = static_cast<PrimitiveType>(value >> 56);
        _size = value & 0xFFFFFFFFFFFFFFUL;
        _hash = 0;
        _classIndex = 0;
        _flags = 0;
    } else {
        if((value >> 56) == 0xFF) {
            _primitive = false;
            _doubleHeader = true;
            _size = value & 0xFFFFFFFFFFFFFF;
            value = *(header + 1);
            _primitive = false;
            _format = VOID_TYPE;
            _hash = (value >> 32) & 0xFFFFFF;
            _classIndex = value & 0xFFFFFF;
            _flags = (value >> 24) & 0xFF;
        } else {
            _primitive = false;
            _format = VOID_TYPE;
            _size = (value >> 56) & 0x7F;
            _doubleHeader = false;
            _hash = (value >> 32) & 0xFFFFFF;
            _classIndex = value & 0xFFFFFF;
            _flags = (value >> 24) & 0xFF;
        }
    }
}

ObjectHeader::ObjectHeader(const PrimitiveType format, const uint64_t byteSize) {
    _primitive = true;
    _doubleHeader = false;
    _format = format;
    _size = byteSize;
    _hash = 0;
    _classIndex = 0;
    _flags = 0;
}

ObjectHeader::ObjectHeader(const uint64_t byteSize, const uint32_t hash, const uint32_t classIndex,
                           const uint8_t flags) {
    _primitive = false;
    _doubleHeader = (byteSize >= 0x7F);
    _format = VOID_TYPE;
    _size = byteSize;
    _hash = hash;
    _classIndex = classIndex;
    _flags = flags;
}

bool ObjectHeader::isPrimitive() const {
    return _primitive;
}

bool ObjectHeader::isEmptySpace() const {
    return _primitive && (_format == EMPTY_SPACE);
}

bool ObjectHeader::isUnavailableSpace() const {
    return _primitive && (_format == UNAVAILABLE_SPACE);
}

bool ObjectHeader::isBytes() const {
    return _primitive && ((_format & BYTES_TYPE) != 0);
}

bool ObjectHeader::isString() const {
    return _primitive && ((_format & STRING_TYPE) != 0);
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
    return (_size / sizeof(BvSlot)) + ((_size % sizeof(BvSlot)) > 0 ? 1 : 0);
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

void ObjectHeader::exportHeader(BvSlot* slot) const {
    if(_primitive) {
        *slot = (static_cast<uint64_t>(_format) << 56) | (_size & 0xFFFFFFFFFFFFFF);
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
                (static_cast<uint64_t>(_hash) << 32) |
                (static_cast<uint64_t>(_flags) << 24) |
                static_cast<uint64_t>(_classIndex);
    }
}

BvSlot* ObjectHeader::nextObject(BvSlot* slot) {
    const ObjectHeader header(slot);
    return slot + header.getSlotSizeWithHeader();
}

BvInt PrimitivesUtil::getUnslotedInt(const BvSlot* slot) {
    const ObjectHeader header(slot);
    if(header.getFormat() != INT_TYPE)
        throw PrimitiveTypeError("This object is not an unsloted integer.");
    return getInt(slot + 1);
}

BvInt PrimitivesUtil::getInt(const BvSlot* slot) {
    return static_cast<BvInt>(*slot);
}

void PrimitivesUtil::putUnslotedInt(BvSlot* slot, const BvInt number) {
    const ObjectHeader header(INT_TYPE, 8);
    header.exportHeader(slot);
    putInt(slot + 1, number);
}

void PrimitivesUtil::putInt(BvSlot* slot, const BvInt number) {
    *reinterpret_cast<BvInt*>(slot) = number;
}

BvFloat PrimitivesUtil::getUnslotedFloat(const BvSlot* slot) {
    const ObjectHeader header(slot);
    if(header.getFormat() != FLOAT_TYPE)
        throw PrimitiveTypeError("This object is not an unsloted floating-point number.");
    return getFloat(slot + 1);
}

BvFloat PrimitivesUtil::getFloat(const BvSlot* slot) {
    return static_cast<BvFloat>(*slot);
}

void PrimitivesUtil::putUnslotedFloat(BvSlot* slot, const BvFloat number) {
    const ObjectHeader header(FLOAT_TYPE, 8);
    header.exportHeader(slot);
    putFloat(slot + 1, number);
}

void PrimitivesUtil::putFloat(BvSlot* slot, const BvFloat number) {
    *reinterpret_cast<BvFloat*>(slot) = number;
}

BvByte PrimitivesUtil::getUnslotedChar(const BvSlot* slot) {
    const ObjectHeader header(slot);
    if(header.getFormat() != CHAR_TYPE)
        throw PrimitiveTypeError("This object is not an unsloted character.");
    return getChar(slot + 1);
}

BvByte PrimitivesUtil::getChar(const BvSlot* slot) {
    return static_cast<BvByte>(*slot & 0xFF);
}

void PrimitivesUtil::putUnslotedChar(BvSlot* slot, const BvByte character) {
    const ObjectHeader header(CHAR_TYPE, 8);
    header.exportHeader(slot);
    putChar(slot + 1, character);
}

void PrimitivesUtil::putChar(BvSlot* slot, const BvByte character) {
    *slot = static_cast<BvSlot>(character);
}

BvBool PrimitivesUtil::getUnslotedBool(const BvSlot* slot) {
    const ObjectHeader header(slot);
    if(header.getFormat() != BOOL_TYPE)
        throw PrimitiveTypeError("This object is not an unsloted boolean.");
    return getBool(slot + 1);
}

BvBool PrimitivesUtil::getBool(const BvSlot* slot) {
    return static_cast<int64_t>(*slot) != 0;
}

void PrimitivesUtil::putUnslotedBool(BvSlot* slot, const BvBool boolean) {
    const ObjectHeader header(BOOL_TYPE, 8);
    header.exportHeader(slot);
    putBool(slot + 1, boolean);
}

void PrimitivesUtil::putBool(BvSlot* slot, const BvBool boolean) {
    *slot = boolean ? 1 : 0;
}

uint64_t PrimitivesUtil::getUnslotedBytes(BvSlot* slot, BvByte** bytes) {
    const ObjectHeader header(slot);
    if(!header.isBytes())
        throw PrimitiveTypeError("This object is not a byte array.");
    *bytes = reinterpret_cast<uint8_t*>(slot + 1);
    return header.getSize();
}

uint64_t PrimitivesUtil::copyUnslotedBytes(const BvSlot* slot, BvByte** bytes) {
    const ObjectHeader header(slot);
    if(!header.isBytes())
        throw PrimitiveTypeError("This object is not a byte array.");
    const uint64_t size = header.getSize();
    *bytes = new uint8_t[size];
    memcpy(*bytes, slot + 1, size);
    return size;
}

void PrimitivesUtil::putUnslotedBytes(BvSlot* slot, const BvByte* bytes, const uint64_t size) {
    const ObjectHeader header(BYTES_TYPE, size);
    header.exportHeader(slot);
    memcpy(slot + 1, bytes, size);
}

void PrimitivesUtil::copyUnslotedString(const BvSlot* slot, std::string& string) {
    const ObjectHeader header(slot);
    if(!header.isBytes())
        throw PrimitiveTypeError("This object is not a string.");
    string.assign(reinterpret_cast<const char*>(slot + 1), header.getSize());
}

void PrimitivesUtil::putUnslotedString(BvSlot* slot, const std::string& string) {
    putUnslotedString(slot, STRING_TYPE, string);
}

void PrimitivesUtil::putUnslotedString(BvSlot* slot, PrimitiveType format, const std::string& string) {
    const ObjectHeader header(format, string.size());
    header.exportHeader(slot);
    memcpy(slot + 1, string.c_str(), string.size());
}
