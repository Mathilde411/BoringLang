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
    _format = VOID;
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
    _format = VOID;
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

bool ObjectHeader::isBytes() const {
    return _primitive && ((_format & BYTES) != 0);
}

bool ObjectHeader::isString() const {
    return _primitive && ((_format & STRING) != 0);
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

int64_t PrimitivesUtil::getUnslotedInt(const BvSlot* slot) {
    ObjectHeader header(slot);
    if (header.getFormat() != INT)
        throw PrimitiveTypeError("This object is not an unsloted integer.");
    return getInt(slot + 1);
}

int64_t PrimitivesUtil::getInt(const BvSlot* slot) {
    return *((int64_t*) slot);
}

void PrimitivesUtil::putUnslotedInt(BvSlot* slot, int64_t number) {
    ObjectHeader header(INT, 8);
    header.exportHeader(slot);
    *((int64_t*)(slot + 1)) = number;
}

double PrimitivesUtil::getUnslotedFloat(const BvSlot* slot) {
    ObjectHeader header(slot);
    if (header.getFormat() != FLOAT)
        throw PrimitiveTypeError("This object is not an unsloted floating-point number.");
    return getFloat(slot + 1);
}

double PrimitivesUtil::getFloat(const BvSlot* slot) {
    return *((double*) slot);
}

void PrimitivesUtil::putUnslotedFloat(BvSlot* slot, double number) {
    ObjectHeader header(FLOAT, 8);
    header.exportHeader(slot);
    *((double*)(slot + 1)) = number;
}

char PrimitivesUtil::getUnslotedChar(const BvSlot* slot) {
    ObjectHeader header(slot);
    if (header.getFormat() != CHAR)
        throw PrimitiveTypeError("This object is not an unsloted character.");
    return getChar(slot + 1);
}

char PrimitivesUtil::getChar(const BvSlot* slot) {
    return (char) (*((uint64_t*) slot) & 0xFF);
}

void PrimitivesUtil::putUnslotedChar(BvSlot* slot, char character) {
    ObjectHeader header(CHAR, 8);
    header.exportHeader(slot);
    *((uint64_t*)(slot + 1)) = (unsigned char)(character);
}

bool PrimitivesUtil::getUnslotedBool(const BvSlot* slot) {
    ObjectHeader header(slot);
    if (header.getFormat() != BOOL)
        throw PrimitiveTypeError("This object is not an unsloted boolean.");
    return getBool(slot + 1);
}

bool PrimitivesUtil::getBool(const BvSlot* slot) {
    return *((int64_t*) slot) != 0;
}

void PrimitivesUtil::putUnslotedBool(BvSlot* slot, bool boolean) {
    ObjectHeader header(BOOL, 8);
    header.exportHeader(slot);
    *((uint64_t*)(slot + 1)) = boolean ? 1:0;
}

uint64_t PrimitivesUtil::getUnslotedBytes(const BvSlot* slot, uint8_t** bytes) {
    ObjectHeader header(slot);
    if (!header.isBytes())
        throw PrimitiveTypeError("This object is not a byte array.");
    *bytes = (uint8_t*) (slot + 1);
    return header.getSize();
}

uint64_t PrimitivesUtil::copyUnslotedBytes(const BvSlot* slot, uint8_t** bytes) {
    ObjectHeader header(slot);
    if (!header.isBytes())
        throw PrimitiveTypeError("This object is not a byte array.");
    uint64_t size = header.getSize();
    *bytes = (uint8_t*) malloc(size);
    memcpy(*bytes, (uint8_t*) (slot + 1), size);
    return size;
}

void PrimitivesUtil::putUnslotedBytes(BvSlot* slot, uint8_t* bytes, uint64_t size) {
    ObjectHeader header(BYTES, size);
    header.exportHeader(slot);
    memcpy((uint8_t*) (slot + 1), bytes, size);
}

void PrimitivesUtil::copyUnslotedString(const BvSlot* slot, std::string& string) {
    ObjectHeader header(slot);
    if (!header.isBytes())
        throw PrimitiveTypeError("This object is not a string.");
    string.assign((char*) (slot+1), header.getSize());
}

void PrimitivesUtil::putUnslotedString(BvSlot* slot, std::string& string) {
    putUnslotedString(slot, STRING, string);
}

void PrimitivesUtil::putUnslotedString(BvSlot* slot, PrimitiveType format, std::string& string) {
    ObjectHeader header(format, string.size());
    header.exportHeader(slot);
    memcpy((uint8_t*) (slot + 1), string.c_str(), string.size());
}
