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

class OutMemoryBuffer final : public std::basic_streambuf<char> {
public:
    OutMemoryBuffer(uint8_t* arr, int size) {
        this->setp(reinterpret_cast<char*>(arr), reinterpret_cast<char*>(arr + size));
    }
};

class InMemoryBuffer final : public std::basic_streambuf<char> {
public:
    InMemoryBuffer(uint8_t* arr, int size) {
        this->setg(reinterpret_cast<char*>(arr), reinterpret_cast<char*>(arr), reinterpret_cast<char*>(arr + size));
    }
};

static const int classFileSize = 532;
uint8_t classFile[532] = {
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

    uint32_t literalsSize = 0;
    for(std::string const& literalString : literalStrings)
        literalsSize += strSlotHeadSize(literalString);
    literalsSize += 2;

    BoringLang::BvSlot literals[literalsSize] = {};
    BoringLang::BvSlot* litSlot = literals;
    for(int i = 0; i < 16; i++) {
        BoringLang::PrimitivesUtil::putUnslotedString(litSlot, literalTypes[i], literalStrings[i]);
        litSlot = BoringLang::ObjectHeader::nextObject(litSlot);
    }
    BoringLang::PrimitivesUtil::putUnslotedInt(litSlot, 42424242);
    clazz.setLiterals(literalsSize, literals);

    clazz.getClassFormat()->setNameIndex(0);
    clazz.getClassFormat()->setNamespaceIndex(1);
    clazz.getClassFormat()->setSuperclassIndex(15);
    clazz.getClassFormat()->setFlags(101);

    auto* method1 = new BoringLang::MethodFormat();
    auto* method2 = new BoringLang::MethodFormat();
    auto* method3 = new BoringLang::MethodFormat();

    method1->setFlags(202);
    method2->setFlags(303);
    method3->setFlags(404);

    method1->setNameIndex(2);
    method2->setNameIndex(3);
    method3->setNameIndex(4);

    method1->setReturnTypeIndex(5);
    method2->setReturnTypeIndex(6);
    method3->setReturnTypeIndex(7);

    method2->addArgumentTypeIndex(8);
    method3->addArgumentTypeIndex(9);
    method3->addArgumentTypeIndex(10);

    auto* codeAttr = new BoringLang::CodeAttribute();
    codeAttr->setMaxStack(10);
    codeAttr->setCodeStart(0);
    codeAttr->setCodeLength(16);
    method2->addAttribute(codeAttr);

    clazz.addMethodFormat(method1);
    clazz.addMethodFormat(method2);
    clazz.addMethodFormat(method3);

    auto* variable1 = new BoringLang::VariableFormat();
    auto* variable2 = new BoringLang::VariableFormat();

    variable1->setFlags(505);
    variable2->setFlags(606);
    variable1->setNameIndex(11);
    variable2->setNameIndex(12);
    variable1->setTypeIndex(13);
    variable2->setTypeIndex(14);

    auto* constAttr = new BoringLang::ConstantValueAttribute();
    constAttr->setValueIndex(16);
    variable1->addAttribute(constAttr);

    clazz.addVariableFormat(variable1);
    clazz.addVariableFormat(variable2);

    const BoringLang::BvBytecode bc[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    clazz.setBytecodes(16, bc);
}

TEST_CASE("testExportClassFile", "[testExportClassFile]") {
    uint8_t buffer[1024];
    OutMemoryBuffer writeBuf(buffer, 1024);
    std::ostream out(&writeBuf);

    BoringLang::ClassFile clazz;
    fillClassFile(clazz);
    clazz.output(out);


    REQUIRE(memcmp(buffer, classFile, classFileSize) == 0);
}

TEST_CASE("testImportClassFile", "[testImportClassFile]") {
    InMemoryBuffer readBuf(classFile, classFileSize);
    std::istream in(&readBuf);

    BoringLang::ClassFile clazz;
    clazz.input(in);

    BoringLang::ClassFile clazzCompare;
    fillClassFile(clazzCompare);

    std::string testStr;
    std::string compareStr;
    long val;
    REQUIRE_NOTHROW(val = BoringLang::PrimitivesUtil::getUnslotedInt(clazz.getLiteral(16)));
    REQUIRE(val == 42424242);

    REQUIRE(clazz.getClassFormat()->getFlags() == 101);

    BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getClassFormat()->getName(), testStr);
    BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getClassFormat()->getName(), compareStr);
    REQUIRE(testStr == compareStr);

    BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getClassFormat()->getNamespace(), testStr);
    BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getClassFormat()->getNamespace(), compareStr);
    REQUIRE(testStr == compareStr);

    BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getClassFormat()->getSuperclass(), testStr);
    BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getClassFormat()->getSuperclass(), compareStr);
    REQUIRE(testStr == compareStr);

    REQUIRE_FALSE(clazz.getClassFormat()->isIndexable() ^ clazzCompare.getClassFormat()->isIndexable());
    REQUIRE_FALSE(clazz.getClassFormat()->isPrimitive() ^ clazzCompare.getClassFormat()->isPrimitive());

    REQUIRE(clazz.getClassFormat()->getNumberOfAttributes() == 0);

    REQUIRE(clazz.getNumberOfMethods() == clazzCompare.getNumberOfMethods());
    for(uint16_t methodNumber = 0; methodNumber < clazz.getNumberOfMethods(); methodNumber++) {
        BoringLang::MethodFormat* method = clazz.getMethodFormat(methodNumber);
        BoringLang::MethodFormat* methodCompare = clazzCompare.getMethodFormat(methodNumber);
        REQUIRE(method->getFlags() == 202 + 101 * methodNumber);

        BoringLang::PrimitivesUtil::copyUnslotedString(method->getName(), testStr);
        BoringLang::PrimitivesUtil::copyUnslotedString(methodCompare->getName(), compareStr);
        REQUIRE(testStr == compareStr);

        BoringLang::PrimitivesUtil::copyUnslotedString(method->getReturnType(), testStr);
        BoringLang::PrimitivesUtil::copyUnslotedString(methodCompare->getReturnType(), compareStr);
        REQUIRE(testStr == compareStr);

        REQUIRE(method->getNumberOfArguments() == methodCompare->getNumberOfArguments());
        for(uint16_t argumentNumber = 0; argumentNumber < method->getNumberOfArguments();
            argumentNumber++) {
            BoringLang::PrimitivesUtil::copyUnslotedString(method->getArgumentType(argumentNumber), testStr);
            BoringLang::PrimitivesUtil::copyUnslotedString( methodCompare->getArgumentType(argumentNumber), compareStr);
            REQUIRE(testStr == compareStr);
        }

        if(methodNumber == 1) {
            REQUIRE(method->getNumberOfAttributes() == 1);
            if(method->getNumberOfAttributes() == 1) {
                BoringLang::Attribute* attr = method->getAttribute(0);
                REQUIRE(attr->getType() == BoringLang::CODE);
                if(attr->getType() == BoringLang::CODE) {
                    auto* codeAttr = dynamic_cast<BoringLang::CodeAttribute*>(attr);
                    REQUIRE(codeAttr->getMaxStack() == 10);
                    REQUIRE(codeAttr->getCodeStart() == 0);
                    REQUIRE(codeAttr->getCodeLength() == 16);
                }
            }
        } else {
            REQUIRE(clazz.getMethodFormat(methodNumber)->getNumberOfAttributes() == 0);
        }
    }

    REQUIRE(clazz.getNumberOfVariables() == clazzCompare.getNumberOfVariables());
    for(uint16_t varNumber = 0; varNumber < clazz.getNumberOfVariables(); varNumber++) {
        BoringLang::VariableFormat* variable = clazz.getVariableFormat(varNumber);
        BoringLang::VariableFormat* variableCompare = clazzCompare.getVariableFormat(varNumber);

        REQUIRE(variable->getFlags() == 505 + 101 * varNumber);

        BoringLang::PrimitivesUtil::copyUnslotedString(variable->getName(), testStr);
        BoringLang::PrimitivesUtil::copyUnslotedString(variableCompare->getName(), compareStr);
        REQUIRE(testStr == compareStr);

        BoringLang::PrimitivesUtil::copyUnslotedString(variable->getType(), testStr);
        BoringLang::PrimitivesUtil::copyUnslotedString(variableCompare->getType(), compareStr);
        REQUIRE(testStr == compareStr);

        if(varNumber == 0) {
            REQUIRE(variable->getNumberOfAttributes() == 1);
            if(variable->getNumberOfAttributes() == 1) {
                BoringLang::Attribute* attr = variable->getAttribute(0);
                REQUIRE(attr->getType() == BoringLang::CONSTANT_VALUE);
                if(attr->getType() == BoringLang::CONSTANT_VALUE) {
                    auto* constAttr = dynamic_cast<BoringLang::ConstantValueAttribute*>(attr);
                    REQUIRE(constAttr->getValueIndex() == 16);
                }
            }
        } else {
            REQUIRE(variable->getNumberOfAttributes() == 0);
        }
    }

    REQUIRE(clazz.getBytecodesSize() == clazzCompare.getBytecodesSize());
    REQUIRE(memcmp(clazz.getBytecodes(), clazzCompare.getBytecodes(), clazz.getBytecodesSize()) == 0);
}
