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

static const int classFileSize = 446;
static const uint8_t classFile[]  = {
0xbe, 0x11, 0xec, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2a, 0x64, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x09, 0x54, 0x65, 0x73, 0x74, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x2f, 0x74, 0x65, 0x73,
0x74, 0x2f, 0x6e, 0x61, 0x6d, 0x65, 0x73, 0x70, 0x61, 0x63, 0x65, 0x2f, 0x65, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x0b, 0x66, 0x69, 0x72, 0x73, 0x74, 0x4d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x00,
0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x73, 0x65, 0x63, 0x6f,
0x6e, 0x64, 0x4d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x0b, 0x74, 0x68, 0x69, 0x72, 0x64, 0x4d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x00,
0x00, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x2f, 0x56, 0x6f, 0x69,
0x64, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x2f, 0x49, 0x6e, 0x74,
0x00, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x2f, 0x46, 0x6c, 0x6f,
0x61, 0x74, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x2f, 0x43, 0x68, 0x61,
0x72, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x2f, 0x53, 0x74, 0x72,
0x69, 0x6e, 0x67, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x2f, 0x43, 0x6c, 0x61,
0x73, 0x73, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x66, 0x69, 0x72, 0x73,
0x74, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x0e, 0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62,
0x6c, 0x65, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x2f, 0x41, 0x72, 0x72,
0x61, 0x79, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x2f, 0x42, 0x6f, 0x6f,
0x6c, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0x2f, 0x74, 0x65, 0x73,
0x74, 0x2f, 0x6e, 0x61, 0x6d, 0x65, 0x73, 0x70, 0x61, 0x63, 0x65, 0x2f, 0x53, 0x75, 0x70, 0x65,
0x72, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x08, 0x00, 0x00, 0x03, 0x00, 0x00,
0x00, 0x06, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x11,
0x00, 0x01, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02,
0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x19, 0x00, 0x02, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00,
0x00, 0x21, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01,
0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
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

    BoringLang::BvSlot* literalPointers[16];

    uint32_t litSize = 0;
    for( std::string const& literalString : literalStrings)
        litSize += strSlotHeadSize(literalString);

    clazz.setLiteralsSize(litSize);
    BoringLang::BvSlot* litSlot = clazz.getLiterals();
    for(int i = 0; i < 16; i++) {
        literalPointers[i] = litSlot;
        BoringLang::PrimitivesUtil::putUnslotedString(litSlot, literalTypes[i], literalStrings[i]);
        litSlot = BoringLang::ObjectHeader::nextObject(litSlot);
    }

    clazz.setClassNameIndex(literalPointers[0] - clazz.getLiterals());
    clazz.setNamespaceIndex(literalPointers[1] - clazz.getLiterals());
    clazz.setSuperclassIndex(literalPointers[15] - clazz.getLiterals());

    clazz.setNumberOfMethods(3);
    clazz.setMethodNameIndex(0, literalPointers[2] - clazz.getLiterals());
    clazz.setMethodNameIndex(1, literalPointers[3] - clazz.getLiterals());
    clazz.setMethodNameIndex(2, literalPointers[4] - clazz.getLiterals());
    clazz.setMethodReturnTypeIndex(0, literalPointers[5] - clazz.getLiterals());
    clazz.setMethodReturnTypeIndex(1, literalPointers[6] - clazz.getLiterals());
    clazz.setMethodReturnTypeIndex(2, literalPointers[7] - clazz.getLiterals());
    clazz.setMethodNumberOfArguments(0, 0);
    clazz.setMethodNumberOfArguments(1, 1);
    clazz.setMethodNumberOfArguments(2, 2);
    clazz.setMethodArgumentTypeIndex(1, 0, literalPointers[8] - clazz.getLiterals());
    clazz.setMethodArgumentTypeIndex(2, 0, literalPointers[9] - clazz.getLiterals());
    clazz.setMethodArgumentTypeIndex(2, 1, literalPointers[10] - clazz.getLiterals());

    clazz.setNumberOfVariables(2);
    clazz.setVariableNameIndex(0, literalPointers[11] - clazz.getLiterals());
    clazz.setVariableNameIndex(1, literalPointers[12] - clazz.getLiterals());
    clazz.setVariableTypeIndex(0, literalPointers[13] - clazz.getLiterals());
    clazz.setVariableTypeIndex(1, literalPointers[14] - clazz.getLiterals());

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

    REQUIRE(clazz.getNumberOfMethods() == clazzCompare.getNumberOfMethods());
    for(uint16_t methodNumber = 0; methodNumber < clazz.getNumberOfMethods(); methodNumber++) {
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

    }

    REQUIRE(clazz.getNumberOfVariables() == clazzCompare.getNumberOfVariables());
    for(uint16_t varNumber = 0; varNumber < clazz.getNumberOfVariables(); varNumber++) {
        BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getVariableName(varNumber), testStr);
        BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getVariableName(varNumber), compareStr);
        REQUIRE(testStr == compareStr);

        BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getVariableType(varNumber), testStr);
        BoringLang::PrimitivesUtil::copyUnslotedString(clazzCompare.getVariableType(varNumber), compareStr);
        REQUIRE(testStr == compareStr);
    }

    REQUIRE(clazz.getBytecodesSize() == clazzCompare.getBytecodesSize());
    REQUIRE(memcmp(clazz.getBytecodes(), clazzCompare.getBytecodes(), clazz.getBytecodesSize()) == 0);
}



