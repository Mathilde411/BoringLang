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

namespace BoringLang {

    class ClassLoadingError : public std::runtime_error {
    public:
        explicit ClassLoadingError(std::string const& msg):runtime_error(msg.c_str()){}
    };

    struct ClassFormat {
        uint32_t _name; // Index of type CLASS_NAME_TYPE in literals
        uint32_t _namespace; // Index of type NAMESPACE_PATH_TYPE in literals
        uint32_t _superclass; // Index of type CLASS_PATH_TYPE in literals
        uint8_t _indexable;
        uint8_t _primitive;
        uint8_t _indexedSlotSize;
        PrimitiveType _type;
    };

    struct MethodFormat {
        uint32_t _name; // Index of type METHOD_NAME_TYPE in literals
        uint32_t _returnType; // Index of type CLASS_PATH_TYPE in literals
        uint16_t _numberOfArguments;
        uint32_t* _argumentTypes; // Indexes of type CLASS_PATH_TYPE in literals
    };

    struct VariableFormat {
        uint32_t _name; // Index of type VARIABLE_NAME_TYPE in literals
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
        void destroyMethodArgumentTypes(MethodFormat* method) const;
        void destroyMethodFormats() const;
        void destroyVariableFormats() const;
        void destroyBytecodes() const;
        void importLiterals(std::istream& stream, ClassHeader& header);
        void importHeader(std::istream& stream, ClassHeader& header);
        void importClassFormat(std::istream& stream, ClassFormat& format);
        void importMethodFormat(std::istream& stream, MethodFormat& format);
        void importVariableFormat(std::istream& stream, VariableFormat& format);
        void exportLiterals(std::ostream& stream, ClassHeader& header);
        void exportHeader(std::ostream& stream, ClassHeader& header);
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

        [[nodiscard]]
        BvSlot* getClassName() const;
        [[nodiscard]]
        uint32_t getClassNameIndex() const;
        void setClassNameIndex(uint32_t index);

        [[nodiscard]]
        BvSlot* getNamespace() const;
        [[nodiscard]]
        uint32_t getNamespaceIndex() const;
        void setNamespaceIndex(uint32_t index);

        [[nodiscard]]
        BvSlot* getSuperclass() const;
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
        uint16_t getNumberOfMethods() const;
        void setNumberOfMethods(uint16_t number);

        [[nodiscard]]
        BvSlot* getMethodName(uint16_t methodNumber) const;
        [[nodiscard]]
        uint32_t getMethodNameIndex(uint16_t methodNumber) const;
        void setMethodNameIndex(uint16_t methodNumber, uint32_t index);

        [[nodiscard]]
        BvSlot* getMethodReturnType(uint16_t methodNumber) const;
        [[nodiscard]]
        uint32_t getMethodReturnTypeIndex(uint16_t methodNumber) const;
        void setMethodReturnTypeIndex(uint16_t methodNumber, uint32_t index);

        [[nodiscard]]
        uint16_t getMethodNumberOfArguments(uint16_t methodNumber) const;
        void setMethodNumberOfArguments(uint16_t methodNumber, uint16_t number);

        [[nodiscard]]
        BvSlot* getMethodArgumentType(uint16_t methodNumber, uint16_t argumentNumber) const;
        [[nodiscard]]
        uint32_t getMethodArgumentTypeIndex(uint16_t methodNumber, uint16_t argumentNumber) const;
        void setMethodArgumentTypeIndex(uint16_t methodNumber, uint16_t argumentNumber, uint32_t index);

        [[nodiscard]]
        uint16_t getNumberOfVariables() const;
        void setNumberOfVariables(uint16_t number);

        [[nodiscard]]
        BvSlot* getVariableName(uint16_t variableNumber) const;
        [[nodiscard]]
        uint32_t getVariableNameIndex(uint16_t variableNumber) const;
        void setVariableNameIndex(uint16_t variableNumber, uint32_t index);

        [[nodiscard]]
        BvSlot* getVariableType(uint16_t variableNumber) const;
        [[nodiscard]]
        uint32_t getVariableTypeIndex(uint16_t variableNumber) const;
        void setVariableTypeIndex(uint16_t variableNumber, uint32_t index);

        [[nodiscard]]
        uint32_t getBytecodesSize() const;
        void setBytecodesSize(uint32_t size);
        [[nodiscard]]
        BvBytecode* getBytecodes() const;
        void setBytecodes(uint32_t size, BvBytecode* bytecodes);
    };

} // BoringLang

#endif //BORINGLANG_CLASSFILE_HPP
