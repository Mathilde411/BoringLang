

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

#include "boringlang/core/ClassFile.hpp"
#include "boringlang/core/Constants.hpp"
#include "boringlang/core/util/BinaryStreamUtil.hpp"


using namespace BoringLang;

ClassFile::~ClassFile() {
    this->destroy();
}

ClassFile::ClassFile() {
    _header = {
            BORINGLANG_MAGIC_NUMBER,
            {BORINGLANG_MAJOR_VERSION, BORINGLANG_MINOR_VERSION},
            0,
            nullptr,
            {
                    0xFFFFFFFF,
                    0xFFFFFFFF,
                    0xFFFFFFFF,
                    false,
                    false,
                    8,
                    PrimitiveType::VOID
            },
            0,
            nullptr,
            0,
            nullptr,
            0
    };
    _bytecodes = nullptr;
}

void ClassFile::destroyLiterals() const {
    free(_header._literals);
}

void ClassFile::destroyMethodArgumentTypes(MethodFormat* method) const {
    free(method->_argumentTypes);
}

void ClassFile::destroyMethodFormats() const {
    if (_header._methodFormats != nullptr) {
        for (MethodFormat* mFormat = _header._methodFormats;
             mFormat < _header._methodFormats + _header._numberOfMethods;
             mFormat++) {
            this->destroyMethodArgumentTypes(mFormat);
        }
    }
    free(_header._methodFormats);
}

void ClassFile::destroyVariableFormats() const {
    free(_header._variableFormats);
}

void ClassFile::destroyBytecodes() const {
    free(_bytecodes);
}

void ClassFile::destroy() const {
    this->destroyLiterals();
    this->destroyMethodFormats();
    this->destroyVariableFormats();
    this->destroyBytecodes();
}

void ClassFile::importClass(std::istream& stream) {
    this->destroy();

    try {
        this->importHeader(stream, _header);

        _bytecodes = (BvBytecode*) calloc(_header._bytecodeSize, sizeof(BvBytecode));
        BinaryStreamUtil::readStream(stream, (char*) (_bytecodes), sizeof(BvBytecode) * _header._bytecodeSize);
    } catch (EndOfStreamError const& ex) {
        throw ClassLoadingError("Reached end-of-stream before class was fully loaded.");
    } catch (StreamError const& ex) {
        throw ClassLoadingError("Reading error was encountered while loading class.");
    }

}

void ClassFile::importLiterals(std::istream& stream, ClassHeader& header) {
    BvSlot* currentLiteralsSlot = header._literals;
    while((currentLiteralsSlot - header._literals) < header._literalsSize) {
        *currentLiteralsSlot = BinaryStreamUtil::read64BitsNumberStream(stream);
        BvSlot* data = currentLiteralsSlot + 1;
        ObjectHeader literalHeader(currentLiteralsSlot);
        if((currentLiteralsSlot - header._literals) + literalHeader.getSlotSizeWithHeader() > header._literalsSize)
            throw ClassLoadingError("Literals size and headers do not match.");
        if(!literalHeader.isBytes()) {
            *data = BinaryStreamUtil::read64BitsNumberStream(stream);
        } else {
            BinaryStreamUtil::readStream(stream, (char*)data, literalHeader.getSlotSize() * sizeof(BvSlot));
        }
        currentLiteralsSlot += literalHeader.getSlotSizeWithHeader();
    }
}

void ClassFile::importHeader(std::istream& stream, ClassHeader& header) {
    header._magicNumber = BinaryStreamUtil::read32BitsNumberStream(stream);
    header._version[0] = BinaryStreamUtil::read16BitsNumberStream(stream);
    header._version[1] = BinaryStreamUtil::read16BitsNumberStream(stream);
    header._literalsSize = BinaryStreamUtil::read32BitsNumberStream(stream);
    header._literals = (BvSlot*) calloc(header._literalsSize, sizeof(BvSlot));
    this->importLiterals(stream, header);

    this->importClassFormat(stream, header._format);

    header._numberOfMethods = BinaryStreamUtil::read16BitsNumberStream(stream);
    header._methodFormats = (MethodFormat*) calloc(header._numberOfMethods, sizeof(MethodFormat));
    for (MethodFormat* methodFormat = header._methodFormats;
         methodFormat < header._methodFormats + header._numberOfMethods; methodFormat++) {
        this->importMethodFormat(stream, *methodFormat);
    }

    header._numberOfVariables = BinaryStreamUtil::read16BitsNumberStream(stream);
    header._variableFormats = (VariableFormat*) calloc(header._numberOfVariables, sizeof(VariableFormat));
    for (VariableFormat* variableFormat = header._variableFormats;
         variableFormat < header._variableFormats + header._numberOfVariables; variableFormat++) {
        this->importVariableFormat(stream, *variableFormat);
    }

    header._bytecodeSize = BinaryStreamUtil::read32BitsNumberStream(stream);
}

void ClassFile::importClassFormat(std::istream& stream, ClassFormat& format) {
    format._name = BinaryStreamUtil::read32BitsNumberStream(stream);
    format._namespace = BinaryStreamUtil::read32BitsNumberStream(stream);
    format._superclass = BinaryStreamUtil::read32BitsNumberStream(stream);
    format._indexable = BinaryStreamUtil::read8BitsNumberStream(stream);
    format._primitive = BinaryStreamUtil::read8BitsNumberStream(stream);
    format._indexedSlotSize = BinaryStreamUtil::read8BitsNumberStream(stream);
    format._type = (PrimitiveType)BinaryStreamUtil::read8BitsNumberStream(stream);
}

void ClassFile::importMethodFormat(std::istream& stream, MethodFormat& format) {
    format._name = BinaryStreamUtil::read32BitsNumberStream(stream);
    format._returnType = BinaryStreamUtil::read32BitsNumberStream(stream);
    format._numberOfArguments = BinaryStreamUtil::read16BitsNumberStream(stream);

    format._argumentTypes = (uint32_t*) calloc(format._numberOfArguments, sizeof(uint32_t));
    for(uint32_t* arg = format._argumentTypes; arg < format._argumentTypes + format._numberOfArguments; arg++)
        *arg = BinaryStreamUtil::read32BitsNumberStream(stream);
}

void ClassFile::importVariableFormat(std::istream& stream, VariableFormat& format) {
    format._name = BinaryStreamUtil::read32BitsNumberStream(stream);
    format._type = BinaryStreamUtil::read32BitsNumberStream(stream);
}

void ClassFile::exportClass(std::ostream& stream) {
    try {
        this->exportHeader(stream, _header);

        BinaryStreamUtil::writeStream(stream, (char*) (_bytecodes), sizeof(BvBytecode) * _header._bytecodeSize);
    } catch (EndOfStreamError const& ex) {
        throw ClassLoadingError("Reached end-of-stream before class was fully loaded.");
    } catch (StreamError const& ex) {
        throw ClassLoadingError("Reading error was encountered while loading class.");
    }

}

void ClassFile::exportLiterals(std::ostream& stream, ClassHeader& header) {
    BvSlot* currentLiteralsSlot = header._literals;
    while((currentLiteralsSlot - header._literals) < header._literalsSize) {
        BinaryStreamUtil::write64BitsNumberStream(stream, *currentLiteralsSlot);
        BvSlot* data = currentLiteralsSlot + 1;
        ObjectHeader literalHeader(currentLiteralsSlot);
        if((currentLiteralsSlot - header._literals) + literalHeader.getSlotSizeWithHeader() > header._literalsSize)
            throw ClassLoadingError("Literals size and headers do not match.");
        if(!literalHeader.isBytes()) {
            BinaryStreamUtil::write64BitsNumberStream(stream, *data);
        } else {
            BinaryStreamUtil::writeStream(stream, (char*)data, literalHeader.getSlotSize() * sizeof(BvSlot));
        }
        currentLiteralsSlot += literalHeader.getSlotSizeWithHeader();
    }
}

void ClassFile::exportHeader(std::ostream& stream, ClassHeader& header) {
    BinaryStreamUtil::write32BitsNumberStream(stream, header._magicNumber);
    BinaryStreamUtil::write16BitsNumberStream(stream, header._version[0]);
    BinaryStreamUtil::write16BitsNumberStream(stream, header._version[1]);
    BinaryStreamUtil::write32BitsNumberStream(stream, header._literalsSize);

    this->exportLiterals(stream, header);

    this->exportClassFormat(stream, header._format);

    BinaryStreamUtil::write16BitsNumberStream(stream, header._numberOfMethods);
    for (MethodFormat* methodFormat = header._methodFormats;
         methodFormat < header._methodFormats + header._numberOfMethods; methodFormat++) {
        this->exportMethodFormat(stream, *methodFormat);
    }

    BinaryStreamUtil::write16BitsNumberStream(stream, header._numberOfVariables);
    for (VariableFormat* variableFormat = header._variableFormats;
         variableFormat < header._variableFormats + header._numberOfVariables; variableFormat++) {
        this->exportVariableFormat(stream, *variableFormat);
    }

    BinaryStreamUtil::write32BitsNumberStream(stream, header._bytecodeSize);
}

void ClassFile::exportClassFormat(std::ostream& stream, ClassFormat& format) {
    BinaryStreamUtil::write32BitsNumberStream(stream, format._name);
    BinaryStreamUtil::write32BitsNumberStream(stream, format._namespace);
    BinaryStreamUtil::write32BitsNumberStream(stream, format._superclass);
    BinaryStreamUtil::write8BitsNumberStream(stream, format._indexable);
    BinaryStreamUtil::write8BitsNumberStream(stream, format._primitive);
    BinaryStreamUtil::write8BitsNumberStream(stream, format._indexedSlotSize);
    BinaryStreamUtil::write8BitsNumberStream(stream, format._type);
}

void ClassFile::exportMethodFormat(std::ostream& stream, MethodFormat& format) {
    BinaryStreamUtil::write32BitsNumberStream(stream, format._name);
    BinaryStreamUtil::write32BitsNumberStream(stream, format._returnType);
    BinaryStreamUtil::write16BitsNumberStream(stream, format._numberOfArguments);

    for(uint32_t* arg = format._argumentTypes; arg < format._argumentTypes + format._numberOfArguments; arg++)
        BinaryStreamUtil::write32BitsNumberStream(stream, *arg);
}

void ClassFile::exportVariableFormat(std::ostream& stream, VariableFormat& format) {
    BinaryStreamUtil::write32BitsNumberStream(stream, format._name);
    BinaryStreamUtil::write32BitsNumberStream(stream, format._type);
}

uint32_t ClassFile::getLiteralsSize() const {
    return _header._literalsSize;
}

void ClassFile::setLiteralsSize(uint32_t size) {
    this->destroyLiterals();
    _header._literals = (BvSlot*) calloc(size, sizeof(BvSlot));
    _header._literalsSize = size;
}

BvSlot* ClassFile::getLiterals() const {
    return _header._literals;
}

void ClassFile::setLiterals(uint32_t size, BvSlot* literals) {
    this->destroyLiterals();
    _header._literals = literals;
    _header._literalsSize = size;
}

BvSlot* ClassFile::getClassName() const {
    return _header._literals + _header._format._name;
}

uint32_t ClassFile::getClassNameIndex() const {
    return _header._format._name;
}

void ClassFile::setClassNameIndex(uint32_t index) {
    _header._format._name = index;
}

BvSlot* ClassFile::getNamespace() const {
    return _header._literals + _header._format._namespace;
}

uint32_t ClassFile::getNamespaceIndex() const {
    return _header._format._namespace;
}

void ClassFile::setNamespaceIndex(uint32_t index) {
    _header._format._namespace = index;
}

BvSlot* ClassFile::getSuperclass() const {
    return _header._literals + _header._format._superclass;
}

uint32_t ClassFile::getSuperclassIndex() const {
    return _header._format._superclass;
}

void ClassFile::setSuperclassIndex(uint32_t index) {
    _header._format._superclass = index;
}

bool ClassFile::isIndexable() const {
    return _header._format._indexable == 1;
}

void ClassFile::setIndexable(bool indexable) {
    _header._format._indexable = indexable ? 1 : 0;
}

bool ClassFile::isPrimitive() const {
    return _header._format._primitive == 1;
}

void ClassFile::setPrimitive(bool primitive) {
    _header._format._primitive = primitive ? 1 : 0;
}

uint8_t ClassFile::getIndexedSlotSize() const {
    return _header._format._indexedSlotSize;
}

void ClassFile::setIndexedSlotSize(uint8_t slotSize) {
    _header._format._indexedSlotSize = slotSize;
}

PrimitiveType ClassFile::getPrimitiveType() const {
    return _header._format._type;
}

void ClassFile::setPrimitiveType(PrimitiveType primitiveType) {
    _header._format._type = primitiveType;
}

uint16_t ClassFile::getNumberOfMethods() const {
    return _header._numberOfMethods;
}

void ClassFile::setNumberOfMethods(uint16_t number) {
    this->destroyMethodFormats();
    _header._numberOfMethods = number;
    _header._methodFormats = (MethodFormat*)calloc(number, sizeof(MethodFormat));
}

MethodFormat* ClassFile::getMethodFormat(uint16_t methodNumber) const {
    if(methodNumber >= _header._numberOfMethods)
        throw std::out_of_range("This method does not exist.");
    return _header._methodFormats + methodNumber;
}

BvSlot* ClassFile::getMethodName(uint16_t methodNumber) const {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    return _header._literals + methodFormat->_name;
}

uint32_t ClassFile::getMethodNameIndex(uint16_t methodNumber) const {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    return methodFormat->_name;
}

void ClassFile::setMethodNameIndex(uint16_t methodNumber, uint32_t index) {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    methodFormat->_name = index;
}

BvSlot* ClassFile::getMethodReturnType(uint16_t methodNumber) const {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    return _header._literals + methodFormat->_returnType;
}

uint32_t ClassFile::getMethodReturnTypeIndex(uint16_t methodNumber) const {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    return methodFormat->_returnType;
}

void ClassFile::setMethodReturnTypeIndex(uint16_t methodNumber, uint32_t index) {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    methodFormat->_returnType = index;
}

uint16_t ClassFile::getMethodNumberOfArguments(uint16_t methodNumber) const {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    return methodFormat->_numberOfArguments;
}

void ClassFile::setMethodNumberOfArguments(uint16_t methodNumber, uint16_t number) {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    this->destroyMethodArgumentTypes(methodFormat);
    methodFormat->_numberOfArguments = number;
    methodFormat->_argumentTypes = (uint32_t*)calloc(number, sizeof(uint32_t));
}

uint32_t* ClassFile::getMethodArgument(MethodFormat* method, uint16_t argumentNumber) const {
    if(argumentNumber >= method->_numberOfArguments)
        throw std::out_of_range("This argument does not exist.");
    return method->_argumentTypes + argumentNumber;
}

BvSlot* ClassFile::getMethodArgumentType(uint16_t methodNumber, uint16_t argumentNumber) const {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    return _header._literals + *this->getMethodArgument(methodFormat, argumentNumber);
}

uint32_t ClassFile::getMethodArgumentTypeIndex(uint16_t methodNumber, uint16_t argumentNumber) const {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    return *this->getMethodArgument(methodFormat, argumentNumber);
}

void ClassFile::setMethodArgumentTypeIndex(uint16_t methodNumber, uint16_t argumentNumber, uint32_t index) {
    MethodFormat* methodFormat = this->getMethodFormat(methodNumber);
    *this->getMethodArgument(methodFormat, argumentNumber) = index;
}

uint16_t ClassFile::getNumberOfVariables() const {
    return _header._numberOfVariables;
}

void ClassFile::setNumberOfVariables(uint16_t number) {
    this->destroyVariableFormats();
    _header._numberOfVariables = number;
    _header._variableFormats = (VariableFormat*)calloc(number, sizeof(VariableFormat));
}

VariableFormat* ClassFile::getVariableFormat(uint16_t variableNumber) const {
    if(variableNumber >= _header._numberOfVariables)
        throw std::out_of_range("This variable does not exist.");
    return _header._variableFormats + variableNumber;
}

BvSlot* ClassFile::getVariableName(uint16_t variableNumber) const {
    VariableFormat* variableFormat = this->getVariableFormat(variableNumber);
    return _header._literals + variableFormat->_name;
}

uint32_t ClassFile::getVariableNameIndex(uint16_t variableNumber) const {
    VariableFormat* variableFormat = this->getVariableFormat(variableNumber);
    return variableFormat->_name;
}

void ClassFile::setVariableNameIndex(uint16_t variableNumber, uint32_t index) {
    VariableFormat* variableFormat = this->getVariableFormat(variableNumber);
    variableFormat->_name = index;
}

BvSlot* ClassFile::getVariableType(uint16_t variableNumber) const {
    VariableFormat* variableFormat = this->getVariableFormat(variableNumber);
    return _header._literals + variableFormat->_type;
}

uint32_t ClassFile::getVariableTypeIndex(uint16_t variableNumber) const {
    VariableFormat* variableFormat = this->getVariableFormat(variableNumber);
    return variableFormat->_type;
}

void ClassFile::setVariableTypeIndex(uint16_t variableNumber, uint32_t index) {
    VariableFormat* variableFormat = this->getVariableFormat(variableNumber);
    variableFormat->_type = index;
}


uint32_t ClassFile::getBytecodesSize() const {
    return _header._bytecodeSize;
}

void ClassFile::setBytecodesSize(uint32_t size) {
    this->destroyBytecodes();
    _bytecodes = (BvBytecode*) calloc(size, sizeof(BvBytecode));
    _header._bytecodeSize = size;
}

BvBytecode* ClassFile::getBytecodes() const {
    return _bytecodes;
}

void ClassFile::setBytecodes(uint32_t size, BvBytecode* bytecodes) {
    this->destroyBytecodes();
    _bytecodes = bytecodes;
    _header._bytecodeSize = size;
}
