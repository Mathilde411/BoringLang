//
// Created by mathilde on 16/07/23.
//

#ifndef BORINGLANG_METHOD_HPP
#define BORINGLANG_METHOD_HPP

#include <map>
#include <string>
#include "Types.hpp"

namespace BoringLang {
    class Class;
    class Method {
    protected:
        Class* returnType;
        int _parametersSize;
        Class** _parameters;
        int _variablesSize;
        Class** _variables;
        int _codeSize;
        BvBytecode* _code;
    };
}

#include "Class.hpp"
#endif //BORINGLANG_METHOD_HPP
