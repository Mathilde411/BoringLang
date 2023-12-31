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
#include <cstdint>
#include <vector>

#include "Constants.hpp"

namespace BoringLang {

    class ClassLoadingError final : public std::runtime_error {
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
        virtual ~Attribute() = default;
        [[nodiscard]]
        virtual AttributeType getType() const;
        [[nodiscard]]
        virtual uint32_t getSize() const;
    };

    class ConstantValueAttribute final : public Attribute {
    private:
        uint32_t _valueIndex = BORINGLANG_NO_INDEX;
    public:
        [[nodiscard]]
        AttributeType getType() const override;

        [[nodiscard]]
        uint32_t getSize() const override;

        [[nodiscard]]
        uint32_t getValueIndex() const;

        void setValueIndex(uint32_t value_index);
    };

    class CodeAttribute final : public Attribute {
        uint32_t _codeStart = 0;
        uint32_t _codeLength = 0;
        uint16_t _maxStack = 0;
    public:
        [[nodiscard]]
        AttributeType getType() const override;

        [[nodiscard]]
        uint32_t getSize() const override;

        [[nodiscard]] uint16_t getMaxStack() const;

        void setMaxStack(uint16_t max_stack);

        [[nodiscard]] uint32_t getCodeStart() const;

        void setCodeStart(uint32_t code_start);

        [[nodiscard]] uint32_t getCodeLength() const;

        void setCodeLength(uint32_t code_length);
    };

    class Streamed {
    public:
        virtual ~Streamed() = default;
        virtual void destroy() = 0;

        virtual void input(std::istream& stream) = 0;
        virtual void output(std::ostream& stream) = 0;
    };

    class ClassFile;
    class Attributed : public Streamed {
    protected:
        ClassFile* _classFile = nullptr;
        std::vector<Attribute*> _attributes;

    public:
        ~Attributed() override;
        void destroy() override;

        void input(std::istream& stream) override;
        void output(std::ostream& stream) override;

        [[nodiscard]]
        uint32_t getNumberOfAttributes() const;
        [[nodiscard]]
        Attribute* getAttribute(uint32_t attributeNumber) const;
        void addAttribute(Attribute* attribute);
        void deleteAttribute(uint32_t attributeNumber);
        void deleteAttributes();
        [[nodiscard]]
        ClassFile* getClassFile() const;
        void setClassFile(ClassFile* classFile);
    };

    class ClassFormat : public Attributed {
    protected:
        uint16_t _flags = 0;
        uint32_t _name = BORINGLANG_NO_INDEX; // Index of type CLASS_NAME_TYPE in literals
        uint32_t _namespace = BORINGLANG_NO_INDEX; // Index of type NAMESPACE_PATH_TYPE in literals
        uint32_t _superclass = BORINGLANG_NO_INDEX; // Index of type CLASS_PATH_TYPE in literals
        uint8_t _indexable = 0;
        uint8_t _primitive = 0;
        uint8_t _indexedSlotSize = sizeof(BvSlot);
        PrimitiveType _type = VOID_TYPE;



    public:

        void input(std::istream& stream) override;
        void output(std::ostream& stream) override;

        [[nodiscard]]
        uint16_t getFlags() const;
        void setFlags(uint16_t flags);

        [[nodiscard]]
        const BvSlot* getName() const;
        [[nodiscard]]
        uint32_t getNameIndex() const;
        void setNameIndex(uint32_t index);

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

    };

    class MethodFormat : public Attributed {
    protected:
        uint16_t _flags = 0;
        uint32_t _name = BORINGLANG_NO_INDEX; // Index of type METHOD_NAME_TYPE in literals
        uint32_t _returnType = BORINGLANG_NO_INDEX; // Index of type CLASS_PATH_TYPE in literals

        std::vector<uint32_t> _argumentTypes; // Indexes of type CLASS_PATH_TYPE in literals


    public:
        void destroy() override;

        void input(std::istream& stream) override;
        void output(std::ostream& stream) override;

        [[nodiscard]]
        uint16_t getFlags() const;
        void setFlags(uint16_t flags);

        [[nodiscard]]
        const BvSlot* getName() const;
        [[nodiscard]]
        uint32_t getNameIndex() const;
        void setNameIndex(uint32_t index);

        [[nodiscard]]
        const BvSlot* getReturnType() const;
        [[nodiscard]]
        uint32_t getReturnTypeIndex() const;
        void setReturnTypeIndex(uint32_t index);

        [[nodiscard]]
        uint16_t getNumberOfArguments() const;
        [[nodiscard]]
        const BvSlot* getArgumentType(uint16_t argumentNumber) const;
        [[nodiscard]]
        uint32_t getArgumentTypeIndex(uint16_t argumentNumber) const;
        void addArgumentTypeIndex(uint32_t index);
        void deleteArgumentType(uint16_t argumentNumber);
        void destroyArgumentTypes();
    };

    class VariableFormat  : public Attributed {
    protected:
        uint16_t _flags = 0;
        uint32_t _name = BORINGLANG_NO_INDEX; // Index of type VARIABLE_NAME_TYPE in literals
        uint32_t _type = BORINGLANG_NO_INDEX; // Index of type CLASS_PATH_TYPE in literals

    public:
        void input(std::istream& stream) override;
        void output(std::ostream& stream) override;

        [[nodiscard]]
        uint16_t getFlags() const;
        void setFlags(uint16_t flags);

        [[nodiscard]]
        const BvSlot* getName() const;
        [[nodiscard]]
        uint32_t getNameIndex() const;
        void setNameIndex(uint32_t index);

        [[nodiscard]]
        const BvSlot* getType() const;
        [[nodiscard]]
        uint32_t getTypeIndex() const;
        void setTypeIndex(uint32_t index);
    };

    class ClassFile : public Streamed {

    protected:
        uint32_t _magicNumber = BORINGLANG_MAGIC_NUMBER;
        uint16_t _version[2] = {BORINGLANG_MAJOR_VERSION, BORINGLANG_MINOR_VERSION};
        uint32_t _literalsSize = 0;
        BvSlot* _literals = nullptr;

        ClassFormat _classFormat;

        std::vector<MethodFormat*> _methodFormats;

        std::vector<VariableFormat*> _variableFormats;

        uint32_t _bytecodeSize = 0;
        BvBytecode* _bytecodes = nullptr;

        void inputLiterals(std::istream& stream);
        void outputLiterals(std::ostream& stream);

    public:
        ClassFile();
        ~ClassFile() override;
        void destroy() override;

        void input(std::istream& stream) override;
        void output(std::ostream& stream) override;

        [[nodiscard]]
        uint32_t getMagicNumber() const;
        void setMagicNumber(uint32_t magicNumber);

        [[nodiscard]]
        const uint16_t* getVersion() const;
        void setVersion(const uint16_t* version);

        [[nodiscard]]
        uint32_t getLiteralsSize() const;
        [[nodiscard]]
        const BvSlot* getLiterals() const;
        void setLiterals(uint32_t size, const BvSlot* literals);
        [[nodiscard]]
        const BvSlot* getLiteral(uint32_t index) const;
        void deleteLiterals();

        [[nodiscard]]
        ClassFormat* getClassFormat();

        [[nodiscard]]
        uint16_t getNumberOfMethods() const;
        [[nodiscard]]
        MethodFormat* getMethodFormat(uint16_t methodNumber) const;
        void addMethodFormat(MethodFormat* method);
        void deleteMethodFormat(uint16_t methodNumber);
        void deleteMethodFormats();

        [[nodiscard]]
        uint16_t getNumberOfVariables() const;
        [[nodiscard]]
        VariableFormat* getVariableFormat(uint16_t variableNumber) const;
        void addVariableFormat(VariableFormat* variable);
        void deleteVariableFormat(uint16_t variableNumber);
        void deleteVariableFormats();

        [[nodiscard]]
        uint32_t getBytecodesSize() const;
        [[nodiscard]]
        const BvBytecode* getBytecodes() const;
        void setBytecodes(uint32_t size, const BvBytecode* bytecodes);
        void deleteBytecodes();
    };



} // BoringLang

#endif //BORINGLANG_CLASSFILE_HPP
