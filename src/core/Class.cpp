//
// Created by mathilde on 11/07/23.
//

#include "boringlang/core/Class.hpp"

using namespace BoringLang;


Class::Class(const std::string& name,
             Format format,
             Namespace* parent,
             int methods,
             int variables) : Class(name, format, parent, nullptr, methods, variables) {

}

Class::Class(const std::string& name,
             Format format,
             Namespace* parent,
             Class* superclass,
             int methods,
             int variables) : Namespaceable(name, parent) {
    _superclass = superclass;
    _format = format;

    _numberOfMethods = methods + (_superclass == nullptr ? 0 : _superclass->_numberOfMethods);
    _numberOfVariables = variables + (_superclass == nullptr ? 0 : _superclass->_numberOfVariables);
    _definedMethodsStart = methods - _numberOfMethods;
    _definedVariablesStart = variables - _numberOfVariables;

    _methods = new Method* [_numberOfMethods];
    _variables = new Class* [_numberOfVariables];
}

Class::~Class() {
    for (int i = 0; i < _numberOfMethods; i++) {
        delete _methods[i];
    }

    delete[] _methods;
    delete[] _variables;
}

