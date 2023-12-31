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
#include "boringlang/core/ClassFile.hpp"

#include <iostream>

#include "boringlang/core/Method.hpp"
#include "boringlang/core/util/BinaryStreamUtil.hpp"


using namespace BoringLang;

// Class Attribute

AttributeType Attribute::getType() const {
    return NONE;
}

uint32_t Attribute::getSize() const {
    return 8;
}


// Class ConstantValueAttribute

AttributeType ConstantValueAttribute::getType() const {
    return CONSTANT_VALUE;
}

uint32_t ConstantValueAttribute::getSize() const {
    return 12;
}

uint32_t ConstantValueAttribute::getValueIndex() const {
    return _valueIndex;
}

void ConstantValueAttribute::setValueIndex(uint32_t value_index) {
    _valueIndex = value_index;
}


// Class CodeAttribute

AttributeType CodeAttribute::getType() const {
    return CODE;
}

uint32_t CodeAttribute::getSize() const {
    return 18;
}

uint16_t CodeAttribute::getMaxStack() const {
    return _maxStack;
}

void CodeAttribute::setMaxStack(uint16_t max_stack) {
    _maxStack = max_stack;
}

uint32_t CodeAttribute::getCodeStart() const {
    return _codeStart;
}

void CodeAttribute::setCodeStart(uint32_t code_start) {
    _codeStart = code_start;
}

uint32_t CodeAttribute::getCodeLength() const {
    return _codeLength;
}

void CodeAttribute::setCodeLength(uint32_t code_length) {
    _codeLength = code_length;
}


// Class Attributed

Attributed::~Attributed() {
    for(auto* attr : _attributes) {
        if(attr != nullptr) {
            switch(attr->getType()) {
                case CONSTANT_VALUE:
                    delete dynamic_cast<ConstantValueAttribute*>(attr);
                    break;
                case CODE:
                    delete dynamic_cast<CodeAttribute*>(attr);
                    break;
                default:
                    delete attr;
            }
        }
    }
    _attributes.clear();
};

void Attributed::destroy() {
    this->deleteAttributes();
}

void Attributed::input(std::istream& stream) {
    this->destroy();

    const uint32_t attrsSize = BinaryStreamUtil::read32BitsNumberStream(stream);
    uint32_t attr = 0;
    while(attr < attrsSize) {
        if(attr + 8 > attrsSize)
            throw ClassLoadingError("Attributes sizes do not match.");

        const auto type = static_cast<AttributeType>(BinaryStreamUtil::read32BitsNumberStream(stream));
        const uint32_t size = BinaryStreamUtil::read32BitsNumberStream(stream);

        if(attr + size > attrsSize)
            throw ClassLoadingError("Attributes sizes do not match.");

        Attribute* vctAttr;
        switch(type) {
            case CONSTANT_VALUE: {
                vctAttr = new ConstantValueAttribute();
                dynamic_cast<ConstantValueAttribute*>(vctAttr)->setValueIndex(
                    BinaryStreamUtil::read32BitsNumberStream(stream));
                break;
            }
            case CODE: {
                vctAttr = new CodeAttribute();
                dynamic_cast<CodeAttribute*>(vctAttr)->setCodeStart(BinaryStreamUtil::read32BitsNumberStream(stream));
                dynamic_cast<CodeAttribute*>(vctAttr)->setCodeLength(BinaryStreamUtil::read32BitsNumberStream(stream));
                dynamic_cast<CodeAttribute*>(vctAttr)->setMaxStack(BinaryStreamUtil::read16BitsNumberStream(stream));
                break;
            }
            default: {
                vctAttr = new Attribute();
            }
        }

        _attributes.push_back(vctAttr);
        attr += size;
    }
}

uint32_t computeAttributesSize(std::vector<Attribute*>& attrs) {
    uint32_t total = 0;

    for(auto* attr : attrs) {
        total += attr->getSize();
    }

    return total;
}

void Attributed::output(std::ostream& stream) {
    BinaryStreamUtil::write32BitsNumberStream(stream, computeAttributesSize(_attributes));

    for(auto* attr : _attributes) {
        BinaryStreamUtil::write32BitsNumberStream(stream, attr->getType());
        BinaryStreamUtil::write32BitsNumberStream(stream, attr->getSize());

        switch(attr->getType()) {
            case CONSTANT_VALUE: {
                const auto* castAttr = dynamic_cast<ConstantValueAttribute*>(attr);
                BinaryStreamUtil::write32BitsNumberStream(stream, castAttr->getValueIndex());
                break;
            }
            case CODE: {
                const auto* castAttr = dynamic_cast<CodeAttribute*>(attr);
                BinaryStreamUtil::write32BitsNumberStream(stream, castAttr->getCodeStart());
                BinaryStreamUtil::write32BitsNumberStream(stream, castAttr->getCodeLength());
                BinaryStreamUtil::write16BitsNumberStream(stream, castAttr->getMaxStack());
                break;
            }
            default: {
            }
        }
    }
}

uint32_t Attributed::getNumberOfAttributes() const {
    return _attributes.size();
}

Attribute* Attributed::getAttribute(uint32_t attributeNumber) const {
    if(attributeNumber >= _attributes.size())
        throw std::out_of_range("This attribute does not exist.");
    return _attributes[attributeNumber];
}

void Attributed::addAttribute(Attribute* attribute) {
    _attributes.push_back(attribute);
}

void Attributed::deleteAttribute(uint32_t attributeNumber) {
    if(attributeNumber >= _attributes.size())
        throw std::out_of_range("This attribute does not exist.");
    _attributes.erase(_attributes.begin() + attributeNumber);
}

void Attributed::deleteAttributes() {
    for(auto* attr : _attributes) {
        if(attr != nullptr) {
            switch(attr->getType()) {
                case CONSTANT_VALUE:
                    std::cout << "- Destroyed ConstantValueAttribute" << std::endl;
                    delete dynamic_cast<ConstantValueAttribute*>(attr);
                    break;
                case CODE:
                    std::cout << "- Destroyed CodeAttribute" << std::endl;
                    delete dynamic_cast<CodeAttribute*>(attr);
                    break;
                default:
                    std::cout << "- Destroyed Default" << std::endl;
                    delete attr;
            }
        }
    }
    _attributes.clear();
}

ClassFile* Attributed::getClassFile() const {
    return _classFile;
}

void Attributed::setClassFile(ClassFile* classFile) {
    _classFile = classFile;
}


//Class ClassFormat

void ClassFormat::input(std::istream& stream) {
    _flags = BinaryStreamUtil::read16BitsNumberStream(stream);
    _name = BinaryStreamUtil::read32BitsNumberStream(stream);
    _namespace = BinaryStreamUtil::read32BitsNumberStream(stream);
    _superclass = BinaryStreamUtil::read32BitsNumberStream(stream);
    _indexable = BinaryStreamUtil::read8BitsNumberStream(stream);
    _primitive = BinaryStreamUtil::read8BitsNumberStream(stream);
    _indexedSlotSize = BinaryStreamUtil::read8BitsNumberStream(stream);
    _type = static_cast<PrimitiveType>(BinaryStreamUtil::read8BitsNumberStream(stream));

    this->Attributed::input(stream);
}

void ClassFormat::output(std::ostream& stream) {
    BinaryStreamUtil::write16BitsNumberStream(stream, _flags);
    BinaryStreamUtil::write32BitsNumberStream(stream, _name);
    BinaryStreamUtil::write32BitsNumberStream(stream, _namespace);
    BinaryStreamUtil::write32BitsNumberStream(stream, _superclass);
    BinaryStreamUtil::write8BitsNumberStream(stream, _indexable);
    BinaryStreamUtil::write8BitsNumberStream(stream, _primitive);
    BinaryStreamUtil::write8BitsNumberStream(stream, _indexedSlotSize);
    BinaryStreamUtil::write8BitsNumberStream(stream, _type);

    this->Attributed::output(stream);
}

uint16_t ClassFormat::getFlags() const {
    return _flags;
}

void ClassFormat::setFlags(uint16_t flags) {
    _flags = flags;
}

const BvSlot* ClassFormat::getName() const {
    return _classFile->getLiteral(getNameIndex());
}

uint32_t ClassFormat::getNameIndex() const {
    return _name;
}

void ClassFormat::setNameIndex(uint32_t index) {
    _name = index;
}

const BvSlot* ClassFormat::getNamespace() const {
    return _classFile->getLiteral(getNamespaceIndex());
}

uint32_t ClassFormat::getNamespaceIndex() const {
    return _namespace;
}

void ClassFormat::setNamespaceIndex(uint32_t index) {
    _namespace = index;
}

const BvSlot* ClassFormat::getSuperclass() const {
    return _classFile->getLiteral(getSuperclassIndex());
}

uint32_t ClassFormat::getSuperclassIndex() const {
    return _superclass;
}

void ClassFormat::setSuperclassIndex(uint32_t index) {
    _superclass = index;
}

bool ClassFormat::isIndexable() const {
    return _indexable == 1;
}

void ClassFormat::setIndexable(bool indexable) {
    _indexable = indexable ? 1 : 0;
}

bool ClassFormat::isPrimitive() const {
    return _primitive == 1;
}

void ClassFormat::setPrimitive(bool primitive) {
    _primitive = primitive ? 1 : 0;
}

uint8_t ClassFormat::getIndexedSlotSize() const {
    return _indexedSlotSize;
}

void ClassFormat::setIndexedSlotSize(uint8_t slotSize) {
    _indexedSlotSize = slotSize;
}

PrimitiveType ClassFormat::getPrimitiveType() const {
    return _type;
}

void ClassFormat::setPrimitiveType(PrimitiveType primitiveType) {
    _type = primitiveType;
}


// Class MethodFormat

void MethodFormat::destroy() {
    Attributed::destroy();
    this->destroyArgumentTypes();
}

void MethodFormat::input(std::istream& stream) {
    this->destroy();

    _flags = BinaryStreamUtil::read16BitsNumberStream(stream);
    _name = BinaryStreamUtil::read32BitsNumberStream(stream);
    _returnType = BinaryStreamUtil::read32BitsNumberStream(stream);

    const uint16_t numberOfArguments = BinaryStreamUtil::read16BitsNumberStream(stream);
    for(uint16_t arg = 0; arg < numberOfArguments; arg++)
        _argumentTypes.push_back(BinaryStreamUtil::read32BitsNumberStream(stream));

    this->Attributed::input(stream);
}

void MethodFormat::output(std::ostream& stream) {
    BinaryStreamUtil::write16BitsNumberStream(stream, _flags);
    BinaryStreamUtil::write32BitsNumberStream(stream, _name);
    BinaryStreamUtil::write32BitsNumberStream(stream, _returnType);

    const uint16_t numberOfArguments = _argumentTypes.size();
    BinaryStreamUtil::write16BitsNumberStream(stream, numberOfArguments);
    for(uint16_t arg = 0; arg < numberOfArguments; arg++)
        BinaryStreamUtil::write32BitsNumberStream(stream, _argumentTypes[arg]);

    this->Attributed::output(stream);
}

uint16_t MethodFormat::getFlags() const {
    return _flags;
}

void MethodFormat::setFlags(const uint16_t flags) {
    _flags = flags;
}

const BvSlot* MethodFormat::getName() const {
    return _classFile->getLiteral(getNameIndex());
}

uint32_t MethodFormat::getNameIndex() const {
    return _name;
}

void MethodFormat::setNameIndex(const uint32_t index) {
    _name = index;
}

const BvSlot* MethodFormat::getReturnType() const {
    return _classFile->getLiteral(getReturnTypeIndex());
}

uint32_t MethodFormat::getReturnTypeIndex() const {
    return _returnType;
}

void MethodFormat::setReturnTypeIndex(const uint32_t index) {
    _returnType = index;
}

uint16_t MethodFormat::getNumberOfArguments() const {
    return _argumentTypes.size();
}

const BvSlot* MethodFormat::getArgumentType(uint16_t argumentNumber) const {
    return _classFile->getLiteral(getArgumentTypeIndex(argumentNumber));
}

uint32_t MethodFormat::getArgumentTypeIndex(uint16_t argumentNumber) const {
    if(argumentNumber >= _argumentTypes.size())
        throw std::out_of_range("This argument does not exist.");
    return _argumentTypes[argumentNumber];
}

void MethodFormat::addArgumentTypeIndex(uint32_t index) {
    _argumentTypes.push_back(index);
}

void MethodFormat::deleteArgumentType(uint16_t argumentNumber) {
    if(argumentNumber >= _argumentTypes.size())
        throw std::out_of_range("This argument does not exist.");
    _argumentTypes.erase(_argumentTypes.begin() + argumentNumber);
}

void MethodFormat::destroyArgumentTypes() {
    _attributes.clear();
}


// Class VariableFormat


void VariableFormat::input(std::istream& stream) {
    _flags = BinaryStreamUtil::read16BitsNumberStream(stream);
    _name = BinaryStreamUtil::read32BitsNumberStream(stream);
    _type = BinaryStreamUtil::read32BitsNumberStream(stream);

    this->Attributed::input(stream);
}

void VariableFormat::output(std::ostream& stream) {
    BinaryStreamUtil::write16BitsNumberStream(stream, _flags);
    BinaryStreamUtil::write32BitsNumberStream(stream, _name);
    BinaryStreamUtil::write32BitsNumberStream(stream, _type);

    this->Attributed::output(stream);
}

uint16_t VariableFormat::getFlags() const {
    return _flags;
}

void VariableFormat::setFlags(const uint16_t flags) {
    _flags = flags;
}

const BvSlot* VariableFormat::getName() const {
    return _classFile->getLiteral(getNameIndex());
}

uint32_t VariableFormat::getNameIndex() const {
    return _name;
}

void VariableFormat::setNameIndex(uint32_t index) {
    _name = index;
}

const BvSlot* VariableFormat::getType() const {
    return _classFile->getLiteral(getTypeIndex());
}

uint32_t VariableFormat::getTypeIndex() const {
    return _type;
}

void VariableFormat::setTypeIndex(uint32_t index) {
    _type = index;
}


// Class ClassFile

ClassFile::ClassFile() {
    _classFormat.setClassFile(this);
}

ClassFile::~ClassFile() {
    std::cout << "ClassFile destructor" << std::endl;
    this->deleteLiterals();
    this->deleteMethodFormats();
    this->deleteVariableFormats();
    this->deleteBytecodes();
}

void ClassFile::destroy() {
    this->deleteLiterals();
    this->deleteMethodFormats();
    this->deleteVariableFormats();
    this->deleteBytecodes();
}

void ClassFile::inputLiterals(std::istream& stream) {
    BvSlot* currentLiteralsSlot = _literals;
    while((currentLiteralsSlot - _literals) < _literalsSize) {
        *currentLiteralsSlot = BinaryStreamUtil::read64BitsNumberStream(stream);
        BvSlot* data = currentLiteralsSlot + 1;
        ObjectHeader literalHeader(currentLiteralsSlot);
        if((currentLiteralsSlot - _literals) + literalHeader.getSlotSizeWithHeader() > _literalsSize)
            throw ClassLoadingError("Literals size and headers do not match.");
        if(!literalHeader.isBytes()) {
            *data = BinaryStreamUtil::read64BitsNumberStream(stream);
        } else {
            BinaryStreamUtil::readStream(stream, data, literalHeader.getSlotSize() * sizeof(BvSlot));
        }
        currentLiteralsSlot += literalHeader.getSlotSizeWithHeader();
    }
}

void ClassFile::input(std::istream& stream) {
    this->destroy();

    try {
        _magicNumber = BinaryStreamUtil::read32BitsNumberStream(stream);
        _version[0] = BinaryStreamUtil::read16BitsNumberStream(stream);
        _version[1] = BinaryStreamUtil::read16BitsNumberStream(stream);
        _literalsSize = BinaryStreamUtil::read32BitsNumberStream(stream);
        BinaryStreamUtil::read32BitsNumberStream(stream);

        _literals = new BvSlot[_literalsSize]();
        this->inputLiterals(stream);

        _classFormat.input(stream);

        const uint16_t numberOfMethods = BinaryStreamUtil::read16BitsNumberStream(stream);
        for(uint16_t methodIndex = 0; methodIndex < numberOfMethods; methodIndex++) {
            auto* method = new MethodFormat();
            method->setClassFile(this);
            method->input(stream);
            _methodFormats.push_back(method);
        }

        const uint16_t numberOfVariables = BinaryStreamUtil::read16BitsNumberStream(stream);
        for(uint16_t variableIndex = 0; variableIndex < numberOfVariables; variableIndex++) {
            auto* variable = new VariableFormat();
            variable->setClassFile(this);
            variable->input(stream);
            _variableFormats.push_back(variable);
        }

        _bytecodeSize = BinaryStreamUtil::read32BitsNumberStream(stream);
        _bytecodes = new BvBytecode[_bytecodeSize]();
        BinaryStreamUtil::readStream(stream, _bytecodes, sizeof(BvBytecode) * _bytecodeSize);
    } catch([[maybe_unused]] EndOfStreamError const& ex) {
        throw ClassLoadingError("Reached end-of-stream before class was fully loaded.");
    } catch([[maybe_unused]] StreamError const& ex) {
        throw ClassLoadingError("Reading error was encountered while loading class.");
    }
}

void ClassFile::outputLiterals(std::ostream& stream) {
    const BvSlot* currentLiteralsSlot = _literals;
    while((currentLiteralsSlot - _literals) < _literalsSize) {
        BinaryStreamUtil::write64BitsNumberStream(stream, *currentLiteralsSlot);
        const BvSlot* data = currentLiteralsSlot + 1;
        ObjectHeader literalHeader(currentLiteralsSlot);
        if((currentLiteralsSlot - _literals) + literalHeader.getSlotSizeWithHeader() > _literalsSize)
            throw ClassLoadingError("Literals size and headers do not match.");
        if(!literalHeader.isBytes()) {
            BinaryStreamUtil::write64BitsNumberStream(stream, *data);
        } else {
            BinaryStreamUtil::writeStream(stream, data, literalHeader.getSlotSize() * sizeof(BvSlot));
        }
        currentLiteralsSlot += literalHeader.getSlotSizeWithHeader();
    }
}

void ClassFile::output(std::ostream& stream) {
    try {
        BinaryStreamUtil::write32BitsNumberStream(stream, _magicNumber);
        BinaryStreamUtil::write16BitsNumberStream(stream, _version[0]);
        BinaryStreamUtil::write16BitsNumberStream(stream, _version[1]);
        BinaryStreamUtil::write32BitsNumberStream(stream, _literalsSize);
        BinaryStreamUtil::write32BitsNumberStream(stream, 0);

        this->outputLiterals(stream);

        _classFormat.output(stream);

        const uint16_t numberOfMethods = _methodFormats.size();
        BinaryStreamUtil::write16BitsNumberStream(stream, numberOfMethods);
        for(uint16_t methodIndex = 0; methodIndex < numberOfMethods; methodIndex++) {
            _methodFormats[methodIndex]->output(stream);
        }

        const uint16_t numberOfVariables = _variableFormats.size();
        BinaryStreamUtil::write16BitsNumberStream(stream, numberOfVariables);
        for(uint16_t variableIndex = 0; variableIndex < numberOfVariables; variableIndex++) {
            _variableFormats[variableIndex]->output(stream);
        }

        BinaryStreamUtil::write32BitsNumberStream(stream, _bytecodeSize);
        BinaryStreamUtil::writeStream(stream, _bytecodes, sizeof(BvBytecode) * _bytecodeSize);
    } catch([[maybe_unused]] EndOfStreamError const& ex) {
        throw ClassLoadingError("Reached end-of-stream before class was fully loaded.");
    } catch([[maybe_unused]] StreamError const& ex) {
        throw ClassLoadingError("Reading error was encountered while loading class.");
    }
}

uint32_t ClassFile::getMagicNumber() const {
    return _magicNumber;
}

void ClassFile::setMagicNumber(uint32_t magicNumber) {
    _magicNumber = magicNumber;
}

const uint16_t* ClassFile::getVersion() const {
    return _version;
}

void ClassFile::setVersion(const uint16_t* version) {
    memcpy(_version, version, 2 * sizeof(uint16_t));
}

uint32_t ClassFile::getLiteralsSize() const {
    return _literalsSize;
}

const BvSlot* ClassFile::getLiterals() const {
    return _literals;
}

void ClassFile::setLiterals(uint32_t size, const BvSlot* literals) {
    this->deleteLiterals();
    _literals = new BvSlot[size]();
    _literalsSize = size;
    memcpy(_literals, literals, sizeof(BvSlot) * size);
}

const BvSlot* ClassFile::getLiteral(uint32_t index) const {
    BvSlot* slot = _literals;
    for(int i = 0; i < index; i++) {
        slot = ObjectHeader::nextObject(slot);
    }
    return slot;
}

void ClassFile::deleteLiterals() {
    delete[] _literals;
    _literals = nullptr;
    _literalsSize = 0;
}

ClassFormat* ClassFile::getClassFormat() {
    return &_classFormat;
}

uint16_t ClassFile::getNumberOfMethods() const {
    return _methodFormats.size();
}

MethodFormat* ClassFile::getMethodFormat(uint16_t methodNumber) const {
    if(methodNumber >= _methodFormats.size())
        throw std::out_of_range("This method does not exist.");
    return _methodFormats[methodNumber];
}

void ClassFile::addMethodFormat(MethodFormat* method) {
    method->setClassFile(this);
    _methodFormats.push_back(method);
}

void ClassFile::deleteMethodFormat(uint16_t methodNumber) {
    if(methodNumber >= _methodFormats.size())
        throw std::out_of_range("This method does not exist.");
    _methodFormats.erase(_methodFormats.begin() + methodNumber);
}

void ClassFile::deleteMethodFormats() {
    for(const MethodFormat* mFormat : _methodFormats) {
        delete mFormat;
    }
    _methodFormats.clear();
}

uint16_t ClassFile::getNumberOfVariables() const {
    return _variableFormats.size();
}

VariableFormat* ClassFile::getVariableFormat(uint16_t variableNumber) const {
    if(variableNumber >= _variableFormats.size())
        throw std::out_of_range("This variable does not exist.");
    return _variableFormats[variableNumber];
}

void ClassFile::addVariableFormat(VariableFormat* variable) {
    variable->setClassFile(this);
    _variableFormats.push_back(variable);
}

void ClassFile::deleteVariableFormat(uint16_t variableNumber) {
    if(variableNumber >= _variableFormats.size())
        throw std::out_of_range("This variable does not exist.");
    _variableFormats.erase(_variableFormats.begin() + variableNumber);
}

void ClassFile::deleteVariableFormats() {
    for(VariableFormat* vFormat : _variableFormats) {
        delete vFormat;
    }
    _variableFormats.clear();
}

uint32_t ClassFile::getBytecodesSize() const {
    return _bytecodeSize;
}

const BvBytecode* ClassFile::getBytecodes() const {
    return _bytecodes;
}

void ClassFile::setBytecodes(uint32_t size, const BvBytecode* bytecodes) {
    this->deleteBytecodes();
    _bytecodes = new BvBytecode[size]();
    _bytecodeSize = size;
    memcpy(_bytecodes, bytecodes, sizeof(BvBytecode) * size);
}

void ClassFile::deleteBytecodes() {
    delete[] _bytecodes;
    _bytecodes = nullptr;
    _bytecodeSize = 0;
}
