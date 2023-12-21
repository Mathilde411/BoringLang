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

#ifndef BORINGLANG_CLASSFILE_HPP
#define BORINGLANG_CLASSFILE_HPP

#include "boringlang/core/util/TypesUtil.hpp"
#include <cstdlib>
#include <cstdint>
#include <vector>

namespace BoringLang {

    class ClassLoadingError : public std::runtime_error {
    public:
        explicit ClassLoadingError(std::string const& msg):runtime_error(msg.c_str()){}
    };

    enum Flags : uint16_t {
        PUBLIC = 0x0001,
        PRIVATE = 0x0002,
        PROTECTED = 0x0004,
        STATIC = 0x0008,
        FINAL = 0x0010,
        ABSTRACT = 0x0020
    };

    enum AttributeType : uint32_t {
        NONE,
        CONSTANT_VALUE,
        CODE
    };

    class Attribute {
    public:
        virtual AttributeType getType() {
            return NONE;
        }

        virtual uint32_t getSize() {
            return 8;
        }
    };

    class ConstantValueAttribute : public Attribute {
    private:
        uint32_t _valueIndex = 0xFFFFFFFF;
    public:
        AttributeType getType() override {
            return CONSTANT_VALUE;
        }

        uint32_t getSize() override {
            return 12;
        }

        [[nodiscard]] uint32_t getValueIndex() const {
            return _valueIndex;
        }

        void setValueIndex(uint32_t value_index) {
            _valueIndex = value_index;
        }
    };

    class CodeAttribute : public Attribute {
        uint32_t _codeStart;
        uint32_t _codeLength;
        uint16_t _maxStack;
    public:
        AttributeType getType() override {
            return CODE;
        }

        uint32_t getSize() override {
            return 18;
        }

        [[nodiscard]] uint16_t getMaxStack() const {
            return _maxStack;
        }

        void setMaxStack(uint16_t max_stack) {
            _maxStack = max_stack;
        }

        [[nodiscard]] uint32_t getCodeStart() const {
            return _codeStart;
        }

        void setCodeStart(uint32_t code_start) {
            _codeStart = code_start;
        }

        [[nodiscard]] uint32_t getCodeLength() const {
            return _codeLength;
        }

        void setCodeLength(uint32_t code_length) {
            _codeLength = code_length;
        }
    };

    struct Format {
        uint16_t _flags;
        uint32_t _name; // Index of type CLASS_NAME_TYPE METHOD_NAME_TYPE or VARIABLE_NAME_TYPE in literals
        std::vector<Attribute*> _attributes;
    };

    struct ClassFormat : Format {
        uint32_t _namespace; // Index of type NAMESPACE_PATH_TYPE in literals
        uint32_t _superclass; // Index of type CLASS_PATH_TYPE in literals
        uint8_t _indexable;
        uint8_t _primitive;
        uint8_t _indexedSlotSize;
        PrimitiveType _type;
    };

    struct MethodFormat : Format {
        uint32_t _returnType; // Index of type CLASS_PATH_TYPE in literals
        uint16_t _numberOfArguments;
        uint32_t* _argumentTypes; // Indexes of type CLASS_PATH_TYPE in literals
    };

    struct VariableFormat  : Format {
        uint32_t _type; // Index of type CLASS_PATH_TYPE in literals
    };

    struct ClassHeader {
        uint32_t _magicNumber;
        uint16_t _version[2];
        uint32_t _literalsSize;
        BvSlot* _literals;

        ClassFormat _format;

        uint16_t _numberOfMethods;
        MethodFormat* _methodFormats;

        uint16_t _numberOfVariables;
        VariableFormat* _variableFormats;

        uint32_t _bytecodeSize;
    };

    class ClassFile {

    protected:
        ClassHeader _header{};
        BvBytecode* _bytecodes;
        void destroyLiterals() const;
        void destroyFormatAttributes(Format* format) const;
        void destroyMethodArgumentTypes(MethodFormat* method) const;
        void destroyMethodFormats() const;
        void destroyVariableFormats() const;
        void destroyBytecodes() const;
        void importLiterals(std::istream& stream, ClassHeader& header);
        void importHeader(std::istream& stream, ClassHeader& header);
        void importAttributes(std::istream& stream, Format& format);
        void importClassFormat(std::istream& stream, ClassFormat& format);
        void importMethodFormat(std::istream& stream, MethodFormat& format);
        void importVariableFormat(std::istream& stream, VariableFormat& format);
        void exportLiterals(std::ostream& stream, ClassHeader& header);
        void exportHeader(std::ostream& stream, ClassHeader& header);
        void exportAttributes(std::ostream& stream, Format& format);
        void exportClassFormat(std::ostream& stream, ClassFormat& format);
        void exportMethodFormat(std::ostream& stream, MethodFormat& format);
        void exportVariableFormat(std::ostream& stream, VariableFormat& format);
        [[nodiscard]]
        MethodFormat* getMethodFormat(uint16_t methodNumber) const;
        [[nodiscard]]
        uint32_t* getMethodArgument(MethodFormat* method, uint16_t argumentNumber) const;
        [[nodiscard]]
        VariableFormat* getVariableFormat(uint16_t variableNumber) const;

    public:
        ~ClassFile();
        ClassFile();
        void destroy() const;
        void importClass(std::istream& stream);
        void exportClass(std::ostream& stream);

        [[nodiscard]]
        uint32_t getMagicNumber();
        void setMagicNumber(uint32_t magicNumber);

        [[nodiscard]]
        uint16_t* getVersion();
        void setVersion(uint16_t* magicNumber);

        [[nodiscard]]
        uint32_t getLiteralsSize() const;
        void setLiteralsSize(uint32_t size);
        [[nodiscard]]
        BvSlot* getLiterals() const;
        void setLiterals(uint32_t size, BvSlot* literals);
        [[nodiscard]] const BvSlot* getLiteral(uint32_t index) const;

        [[nodiscard]]
        uint16_t getClassFlags() const;
        void setClassFlags(uint16_t flags);

        [[nodiscard]]
        const BvSlot* getClassName() const;
        [[nodiscard]]
        uint32_t getClassNameIndex() const;
        void setClassNameIndex(uint32_t index);

        [[nodiscard]]
        const BvSlot* getNamespace() const;
        [[nodiscard]]
        uint32_t getNamespaceIndex() const;
        void setNamespaceIndex(uint32_t index);

        [[nodiscard]]
        const BvSlot* getSuperclass() const;
        [[nodiscard]]
        uint32_t getSuperclassIndex() const;
        void setSuperclassIndex(uint32_t index);

        [[nodiscard]]
        bool isIndexable() const;
        void setIndexable(bool indexable);
        [[nodiscard]]
        bool isPrimitive() const;
        void setPrimitive(bool primitive);
        [[nodiscard]]
        uint8_t getIndexedSlotSize() const;
        void setIndexedSlotSize(uint8_t slotSize);
        [[nodiscard]]
        PrimitiveType getPrimitiveType() const;
        void setPrimitiveType(PrimitiveType primitiveType);
        [[nodiscard]]
        std::vector<Attribute*>& getClassAttributes();

        [[nodiscard]]
        uint16_t getNumberOfMethods() const;
        void setNumberOfMethods(uint16_t number);

        [[nodiscard]]
        uint16_t getMethodFlags(uint16_t methodNumber) const;
        void setMethodFlags(uint16_t methodNumber, uint16_t flags);

        [[nodiscard]]
        const BvSlot* getMethodName(uint16_t methodNumber) const;
        [[nodiscard]]
        uint32_t getMethodNameIndex(uint16_t methodNumber) const;
        void setMethodNameIndex(uint16_t methodNumber, uint32_t index);

        [[nodiscard]]
        const BvSlot* getMethodReturnType(uint16_t methodNumber) const;
        [[nodiscard]]
        uint32_t getMethodReturnTypeIndex(uint16_t methodNumber) const;
        void setMethodReturnTypeIndex(uint16_t methodNumber, uint32_t index);

        [[nodiscard]]
        uint16_t getMethodNumberOfArguments(uint16_t methodNumber) const;
        void setMethodNumberOfArguments(uint16_t methodNumber, uint16_t number);

        [[nodiscard]]
        const BvSlot* getMethodArgumentType(uint16_t methodNumber, uint16_t argumentNumber) const;
        [[nodiscard]]
        uint32_t getMethodArgumentTypeIndex(uint16_t methodNumber, uint16_t argumentNumber) const;
        void setMethodArgumentTypeIndex(uint16_t methodNumber, uint16_t argumentNumber, uint32_t index);

        [[nodiscard]]
        std::vector<Attribute*>& getMethodAttributes(uint16_t methodNumber);

        [[nodiscard]]
        uint16_t getNumberOfVariables() const;
        void setNumberOfVariables(uint16_t number);

        [[nodiscard]]
        uint16_t getVariableFlags(uint16_t variableNumber) const;
        void setVariableFlags(uint16_t variableNumber, uint16_t flags);

        [[nodiscard]]
        const BvSlot* getVariableName(uint16_t variableNumber) const;
        [[nodiscard]]
        uint32_t getVariableNameIndex(uint16_t variableNumber) const;
        void setVariableNameIndex(uint16_t variableNumber, uint32_t index);

        [[nodiscard]]
        const BvSlot* getVariableType(uint16_t variableNumber) const;
        [[nodiscard]]
        uint32_t getVariableTypeIndex(uint16_t variableNumber) const;
        void setVariableTypeIndex(uint16_t variableNumber, uint32_t index);

        [[nodiscard]]
        std::vector<Attribute*>& getVariableAttributes(uint16_t variableNumber);

        [[nodiscard]]
        uint32_t getBytecodesSize() const;
        void setBytecodesSize(uint32_t size);
        [[nodiscard]]
        BvBytecode* getBytecodes() const;
        void setBytecodes(uint32_t size, BvBytecode* bytecodes);
    };

} // BoringLang

#endif //BORINGLANG_CLASSFILE_HPP
