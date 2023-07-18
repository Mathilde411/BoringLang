//
// Created by mathilde on 16/07/23.
//

#ifndef BORINGVM_METHOD_HPP
#define BORINGVM_METHOD_HPP

#include <map>
#include <string>
#include "Types.hpp"

namespace BoringLang {
    class Class;
    class Method {
    protected:
        int _variablesSize;
        Class** _variables;
        int _codeSize;
        BvBytecode* _code;
    };
}

#include "Class.hpp"
#endif //BORINGVM_METHOD_HPP
