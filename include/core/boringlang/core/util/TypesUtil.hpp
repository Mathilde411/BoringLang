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
#include <string>
#include <stdexcept>

namespace BoringLang {

    typedef uint64_t BvUInt;
    typedef int64_t BvInt;
    typedef double BvFloat;
    typedef uint64_t BvSlot;
    typedef uint8_t BvBytecode;

    enum PrimitiveType : uint8_t {
        //Slot-sized
        VOID_TYPE = 0,
        INT_TYPE = 1,
        FLOAT_TYPE = 2,
        CHAR_TYPE = 3,
        BOOL_TYPE = 4,
        CLASS_HANDLE_TYPE = 5,
        METHOD_HANDLE_TYPE = 6,
        VARIABLE_HANDLE_TYPE = 7,
        NAMESPACE_HANDLE_TYPE = 8,

        EMPTY_SPACE = 62,
        UNAVAILABLE_SPACE = 63,

        //Bytes array type
        BYTES_TYPE = 0 | (1 << 6),
        STRING_TYPE = (1 << 5) | BYTES_TYPE,
        NAMESPACE_PATH_TYPE = 2 | STRING_TYPE,
        CLASS_PATH_TYPE = 3 | STRING_TYPE,
        CLASS_NAME_TYPE = 4 | STRING_TYPE,
        METHOD_NAME_TYPE = 5 | STRING_TYPE,
        VARIABLE_NAME_TYPE = 6 | STRING_TYPE,

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
        explicit ObjectHeader(const BvSlot* header);
        ObjectHeader(PrimitiveType format, uint64_t byteSize);
        ObjectHeader(uint64_t byteSize, uint32_t hash, uint32_t classIndex, uint8_t flags);

        [[nodiscard]]
        bool isPrimitive() const;
        [[nodiscard]]
        bool isEmptySpace() const;
        [[nodiscard]]
        bool isUnavailableSpace() const;
        [[nodiscard]]
        bool isBytes() const;
        [[nodiscard]]
        bool isString() const;
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

    class PrimitiveTypeError : public std::runtime_error {
    public:
        PrimitiveTypeError():runtime_error("Error when reading stream."){}
        explicit PrimitiveTypeError(std::string const& msg):runtime_error(msg.c_str()){}
    };

    class PrimitivesUtil {
    public:
        static int64_t getUnslotedInt(const BvSlot* slot);
        static int64_t getInt(const BvSlot* slot);
        static void putUnslotedInt(BvSlot* slot, int64_t number);
        static double getUnslotedFloat(const BvSlot* slot);
        static double getFloat(const BvSlot* slot);
        static void putUnslotedFloat(BvSlot* slot, double number);
        static char getUnslotedChar(const BvSlot* slot);
        static char getChar(const BvSlot* slot);
        static void putUnslotedChar(BvSlot* slot, char character);
        static bool getUnslotedBool(const BvSlot* slot);
        static bool getBool(const BvSlot* slot);
        static void putUnslotedBool(BvSlot* slot, bool boolean);
        static uint64_t getUnslotedBytes(const BvSlot* slot, uint8_t** bytes);
        static uint64_t copyUnslotedBytes(const BvSlot* slot, uint8_t** bytes);
        static void putUnslotedBytes(BvSlot* slot, uint8_t* bytes, uint64_t size);
        static void copyUnslotedString(const BvSlot* slot, std::string& string);
        static void putUnslotedString(BvSlot* slot, std::string& string);
        static void putUnslotedString(BvSlot* slot, PrimitiveType format, std::string& string);
    };
}

#endif //BORINGLANG_TYPESUTIL_HPP
