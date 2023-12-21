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

#include <cstdint>
#include <cstring>
#include <catch2/catch_all.hpp>
#include "boringlang/core/ClassFile.hpp"

#define strSlotHeadSize(str) ((str.size()/8) + ((str.size() % 8) > 0 ? 1 : 0) + 1)

class OutMemoryBuffer : public std::basic_streambuf<char>
{
public:
    OutMemoryBuffer(uint8_t* arr, int size) {
        this->setp((char*)arr, (char*)(arr + size));
    }
};

class InMemoryBuffer : public std::basic_streambuf<char>
{
public:
    InMemoryBuffer(const uint8_t* arr, int size) {
        this->setg((char*)arr, (char*)arr, (char*)(arr + size));
    }
};

static const int classFileSize = 532;
const uint8_t classFile[532] = {
    0xBE, 0x11, 0xEC, 0x1E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x00,
    0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x54, 0x65, 0x73, 0x74, 0x43, 0x6C, 0x61, 0x73,
    0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x2F, 0x74, 0x65, 0x73, 0x74, 0x2F, 0x6E, 0x61, 0x6D, 0x65, 0x73, 0x70, 0x61, 0x63, 0x65, 0x2F,
    0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x66, 0x69, 0x72, 0x73, 0x74, 0x4D, 0x65, 0x74,
    0x68, 0x6F, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C,
    0x73, 0x65, 0x63, 0x6F, 0x6E, 0x64, 0x4D, 0x65, 0x74, 0x68, 0x6F, 0x64, 0x00, 0x00, 0x00, 0x00,
    0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x74, 0x68, 0x69, 0x72, 0x64, 0x4D, 0x65, 0x74,
    0x68, 0x6F, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
    0x2F, 0x56, 0x6F, 0x69, 0x64, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x2F, 0x49, 0x6E, 0x74, 0x00, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
    0x2F, 0x46, 0x6C, 0x6F, 0x61, 0x74, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
    0x2F, 0x43, 0x68, 0x61, 0x72, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
    0x2F, 0x53, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
    0x2F, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D,
    0x66, 0x69, 0x72, 0x73, 0x74, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6C, 0x65, 0x00, 0x00, 0x00,
    0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x73, 0x65, 0x63, 0x6F, 0x6E, 0x64, 0x56, 0x61,
    0x72, 0x69, 0x61, 0x62, 0x6C, 0x65, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
    0x2F, 0x41, 0x72, 0x72, 0x61, 0x79, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
    0x2F, 0x42, 0x6F, 0x6F, 0x6C, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A,
    0x2F, 0x74, 0x65, 0x73, 0x74, 0x2F, 0x6E, 0x61, 0x6D, 0x65, 0x73, 0x70, 0x61, 0x63, 0x65, 0x2F,
    0x53, 0x75, 0x70, 0x65, 0x72, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x02, 0x87, 0x57, 0xB2,
    0x00, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0xCA, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
    0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x2F, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0A, 0x01, 0x94,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x07, 0x00, 0x02, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00,
    0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0xF9, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00,
    0x00, 0x0D, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00,
    0x00, 0x10, 0x02, 0x5E, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F
};

void fillClassFile(BoringLang::ClassFile& clazz) {
    std::string literalStrings[] = {
            "TestClass", // 0 className
            "/test/namespace/", // 1 namespacePath
            "firstMethod", // 2 method1Name
            "secondMethod", // 3 method2Name
            "thirdMethod", // 4 method3Name
            "/Void", // 5 method1Type
            "/Int", // 6 method2Type
            "/Float", // 7 method3Type
            "/Char", // 8 method2Arg
            "/String", // 9 method3Arg1
            "/Class", // 10 method3Arg2
            "firstVariable", // 11 var1Name
            "secondVariable", // 12 var2Name
            "/Array", // 13 var1Type
            "/Bool", // 14 var2Type
            "/test/namespace/SuperClass" // 15 superclass
    };

    BoringLang::PrimitiveType literalTypes[] = {
            BoringLang::CLASS_NAME_TYPE, // 0 className
            BoringLang::NAMESPACE_PATH_TYPE, // 1 namespacePath
            BoringLang::METHOD_NAME_TYPE, // 2 method1Name
            BoringLang::METHOD_NAME_TYPE, // 3 method2Name
            BoringLang::METHOD_NAME_TYPE, // 4 method3Name
            BoringLang::CLASS_PATH_TYPE, // 5 method1Type
            BoringLang::CLASS_PATH_TYPE, // 6 method2Type
            BoringLang::CLASS_PATH_TYPE, // 7 method3Type
            BoringLang::CLASS_PATH_TYPE, // 8 method2Arg
            BoringLang::CLASS_PATH_TYPE, // 9 method3Arg1
            BoringLang::CLASS_PATH_TYPE, // 10 method3Arg2
            BoringLang::VARIABLE_NAME_TYPE, // 11 var1Name
            BoringLang::VARIABLE_NAME_TYPE, // 12 var2Name
            BoringLang::CLASS_PATH_TYPE, // 13 var1Type
            BoringLang::CLASS_PATH_TYPE, // 14 var2Type
            BoringLang::CLASS_PATH_TYPE // 15 superclassPath
    };

    uint32_t litSize = 0;
    for( std::string const& literalString : literalStrings)
        litSize += strSlotHeadSize(literalString);
    litSize += 2;

    clazz.setLiteralsSize(litSize);
    BoringLang::BvSlot* litSlot = clazz.getLiterals();
    for(int i = 0; i < 16; i++) {
        BoringLang::PrimitivesUtil::putUnslotedString(litSlot, literalTypes[i], literalStrings[i]);
        litSlot = BoringLang::ObjectHeader::nextObject(litSlot);
    }
    BoringLang::PrimitivesUtil::putUnslotedInt(litSlot, 42424242);

    clazz.setClassNameIndex(0);
    clazz.setNamespaceIndex(1);
    clazz.setSuperclassIndex(15);
    clazz.setClassFlags(101);

    clazz.setNumberOfMethods(3);
    clazz.setMethodFlags(0, 202);
    clazz.setMethodFlags(1, 303);
    clazz.setMethodFlags(2, 404);
    clazz.setMethodNameIndex(0, 2);
    clazz.setMethodNameIndex(1, 3);
    clazz.setMethodNameIndex(2, 4);
    clazz.setMethodReturnTypeIndex(0, 5);
    clazz.setMethodReturnTypeIndex(1, 6);
    clazz.setMethodReturnTypeIndex(2, 7);
    clazz.setMethodNumberOfArguments(0, 0);
    clazz.setMethodNumberOfArguments(1, 1);
    clazz.setMethodNumberOfArguments(2, 2);
    clazz.setMethodArgumentTypeIndex(1, 0, 8);
    clazz.setMethodArgumentTypeIndex(2, 0, 9);
    clazz.setMethodArgumentTypeIndex(2, 1, 10);
    auto* codeAttr = new BoringLang::CodeAttribute();
    codeAttr->setMaxStack(10);
    codeAttr->setCodeStart(0);
    codeAttr->setCodeLength(16);
    clazz.getMethodAttributes(1).push_back(codeAttr);

    clazz.setNumberOfVariables(2);
    clazz.setVariableFlags(0, 505);
    clazz.setVariableFlags(1, 606);
    clazz.setVariableNameIndex(0, 11);
    clazz.setVariableNameIndex(1, 12);
    clazz.setVariableTypeIndex(0, 13);
    clazz.setVariableTypeIndex(1, 14);
    auto* constAttr = new BoringLang::ConstantValueAttribute();
    constAttr->setValueIndex(16);
    clazz.getVariableAttributes(0).push_back(constAttr);

    BoringLang::BvBytecode bc[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    clazz.setBytecodesSize(16);
    memcpy(clazz.getBytecodes(), bc, 16);
}

TEST_CASE("testExportClassFile", "[testExportClassFile]") {
    uint8_t buffer[1024];
    OutMemoryBuffer writeBuf(buffer, 1024);
    std::ostream out(&writeBuf);

    BoringLang::ClassFile clazz;
    fillClassFile(clazz);
    clazz.exportClass(out);

    REQUIRE(memcmp(buffer, classFile, classFileSize) == 0);

}

TEST_CASE("testImportClassFile", "[testImportClassFile]") {
    InMemoryBuffer readBuf(classFile, classFileSize);
    std::istream in(&readBuf);

    BoringLang::ClassFile clazz;
    clazz.importClass(in);

    BoringLang::ClassFile clazzCompare;
    fillClassFile(clazzCompare);

    std::string testStr;
    std::string compareStr;
    long val = 0;
    REQUIRE_NOTHROW(val = BoringLang::PrimitivesUtil::getUnslotedInt(clazz.getLiteral(16)));
    REQUIRE(val == 42424242);

    REQUIRE(clazz.getClassFlags() == 101);

    BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getClassName(), testStr);
    BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getClassName(), compareStr);
    REQUIRE(testStr == compareStr);

    BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getNamespace(), testStr);
    BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getNamespace(), compareStr);
    REQUIRE(testStr == compareStr);

    BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getSuperclass(), testStr);
    BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getSuperclass(), compareStr);
    REQUIRE(testStr == compareStr);

    REQUIRE_FALSE(clazz.isIndexable() ^ clazzCompare.isIndexable());
    REQUIRE_FALSE(clazz.isPrimitive() ^ clazzCompare.isPrimitive());

    REQUIRE(clazz.getClassAttributes().empty());

    REQUIRE(clazz.getNumberOfMethods() == clazzCompare.getNumberOfMethods());
    for(uint16_t methodNumber = 0; methodNumber < clazz.getNumberOfMethods(); methodNumber++) {
        REQUIRE(clazz.getMethodFlags(methodNumber) == 202 + 101 * methodNumber);

        BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getMethodName(methodNumber), testStr);
        BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getMethodName(methodNumber), compareStr);
        REQUIRE(testStr == compareStr);

        BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getMethodReturnType(methodNumber), testStr);
        BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getMethodReturnType(methodNumber), compareStr);
        REQUIRE(testStr == compareStr);

        REQUIRE(clazz.getMethodNumberOfArguments(methodNumber) == clazzCompare.getMethodNumberOfArguments(methodNumber));
        for(uint16_t argumentNumber = 0; argumentNumber < clazz.getMethodNumberOfArguments(methodNumber); argumentNumber++) {
            BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getMethodArgumentType(methodNumber, argumentNumber), testStr);
            BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getMethodArgumentType(methodNumber, argumentNumber), compareStr);
            REQUIRE(testStr == compareStr);
        }

        if(methodNumber == 1) {
            REQUIRE(clazz.getMethodAttributes(methodNumber).size() == 1);
            if(clazz.getMethodAttributes(methodNumber).size() == 1) {
                BoringLang::Attribute* attr = clazz.getMethodAttributes(methodNumber).at(0);
                REQUIRE(attr->getType() == BoringLang::CODE);
                if(attr->getType() == BoringLang::CODE) {
                    auto* codeAttr = (BoringLang::CodeAttribute*)attr;
                    REQUIRE(codeAttr->getMaxStack() == 10);
                    REQUIRE(codeAttr->getCodeStart() == 0);
                    REQUIRE(codeAttr->getCodeLength() == 16);
                }
            }
        } else {
            REQUIRE(clazz.getMethodAttributes(methodNumber).empty());
        }

    }

    REQUIRE(clazz.getNumberOfVariables() == clazzCompare.getNumberOfVariables());
    for(uint16_t varNumber = 0; varNumber < clazz.getNumberOfVariables(); varNumber++) {
        REQUIRE(clazz.getVariableFlags(varNumber) == 505 + 101 * varNumber);

        BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getVariableName(varNumber), testStr);
        BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getVariableName(varNumber), compareStr);
        REQUIRE(testStr == compareStr);

        BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getVariableType(varNumber), testStr);
        BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getVariableType(varNumber), compareStr);
        REQUIRE(testStr == compareStr);

        if(varNumber == 0) {
            REQUIRE(clazz.getVariableAttributes(varNumber).size() == 1);
            if(clazz.getVariableAttributes(varNumber).size() == 1) {
                BoringLang::Attribute* attr = clazz.getVariableAttributes(varNumber).at(0);
                REQUIRE(attr->getType() == BoringLang::CONSTANT_VALUE);
                if(attr->getType() == BoringLang::CONSTANT_VALUE) {
                    auto* constAttr = (BoringLang::ConstantValueAttribute*)attr;
                    REQUIRE(constAttr->getValueIndex() == 16);
                }
            }
        } else {
            REQUIRE(clazz.getVariableAttributes(varNumber).empty());
        }
    }

    REQUIRE(clazz.getBytecodesSize() == clazzCompare.getBytecodesSize());
    REQUIRE(memcmp(clazz.getBytecodes(), clazzCompare.getBytecodes(), clazz.getBytecodesSize()) == 0);
}



