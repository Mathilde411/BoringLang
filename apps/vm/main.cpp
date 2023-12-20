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

#include <fstream>
#include <cstring>
#include <iostream>
#include "boringlang/core/ClassFile.hpp"
#include "boringlang/core/util/TypesUtil.hpp"

#define strSlotHeadSize(str) ((str.size()/8) + ((str.size() % 8) > 0 ? 1 : 0) + 1)

int main1() {
    BoringLang::ClassFile clazz;
    std::ifstream myfile;
    myfile.open("/home/mathilde/Documents/testclass.blx");
    clazz.importClass(myfile);
    myfile.close();
    std::string res;
    BoringLang::PrimitivesUtil::copyUnslotedString(clazz.getSuperclass(), res);
    std::cout << res << std::endl;
}

int main() {
    BoringLang::ClassFile clazz;

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

    clazz.setLiteralsSize(litSize);
    BoringLang::BvSlot* litSlot = clazz.getLiterals();
    for(int i = 0; i < 16; i++) {
        BoringLang::PrimitivesUtil::putUnslotedString(litSlot, literalTypes[i], literalStrings[i]);
        litSlot = BoringLang::ObjectHeader::nextObject(litSlot);
    }

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

    clazz.setNumberOfVariables(2);
    clazz.setVariableFlags(0, 505);
    clazz.setVariableFlags(1, 606);
    clazz.setVariableNameIndex(0, 11);
    clazz.setVariableNameIndex(1, 12);
    clazz.setVariableTypeIndex(0, 13);
    clazz.setVariableTypeIndex(1, 14);

    BoringLang::BvBytecode bc[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    clazz.setBytecodesSize(16);
    memcpy(clazz.getBytecodes(), bc, 16);

    std::string testStr;
    std::ofstream myfile;
    myfile.open("/home/pop_os/Documents/testclass.blx");
    clazz.exportClass(myfile);
    myfile.close();

    return 0;
}
