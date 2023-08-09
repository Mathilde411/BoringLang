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

#ifndef BORINGLANG_TYPESUTIL_HPP
#define BORINGLANG_TYPESUTIL_HPP

#include <cstdint>

namespace BoringLang {

    typedef uint64_t BvUInt;
    typedef int64_t BvInt;
    typedef double BvFloat;
    typedef uint64_t BvSlot;
    typedef uint8_t BvBytecode;

    enum PrimitiveType : uint8_t {
        //SLOTED
        NO_TYPE = 0,
        INT = 1,
        FLOAT = 2,
        CHAR = 3,
        BOOL = 4,
        CLASS_HANDLE = 5,
        METHOD_HANDLE = 6,
        VARIABLE_HANDLE = 7,

        //UNSLOTED
        BYTES = 0 + (1 << 6),
        STRING = 1 + (1 << 6),
        NAMESPACE = 2 + (1 << 6),
        CLASS_PATH = 3 + (1 << 6),
        CLASS_NAME = 4 + (1 << 6),
        METHOD_NAME = 5 + (1 << 6),
        VARIABLE_NAME = 6 + (1 << 6),
        NAMESPACE_HANDLE = 7 + (1 << 6)
    };

    class ObjectHeader {
    protected:
        bool _primitive;
        bool _doubleHeader;
        PrimitiveType _format;
        uint64_t _size;
        uint32_t _hash;
        uint32_t _classIndex;
        uint8_t _flags;

        void loadPrimitiveHeader(const BvSlot* slot);
        void loadSizeExtension(const BvSlot* slot);
        void loadObjectHeader(const BvSlot* slot, bool loadSize);
    public:
        explicit ObjectHeader(BvSlot* header);
        ObjectHeader(PrimitiveType format, uint64_t byteSize);
        ObjectHeader(uint64_t byteSize, uint32_t hash, uint32_t classIndex, uint8_t flags);

        [[nodiscard]]
        bool isPrimitive() const;
        [[nodiscard]]
        bool isSloted() const;
        [[nodiscard]]
        bool isDoubleHeader() const;
        [[nodiscard]]
        PrimitiveType getFormat() const;
        [[nodiscard]]
        uint64_t getSize() const;
        [[nodiscard]]
        uint64_t getSlotSize() const;
        [[nodiscard]]
        uint32_t getHash() const;
        [[nodiscard]]
        uint32_t getClassIndex() const;
        [[nodiscard]]
        uint8_t getFlags() const;
        [[nodiscard]]
        uint64_t getSlotSizeWithHeader() const;
        void exportHeader(BvSlot* slot);
        [[nodiscard]]
        static BvSlot* nextObject(BvSlot* slot);
    };
}

#include "../Class.hpp"
#endif //BORINGLANG_TYPESUTIL_HPP
