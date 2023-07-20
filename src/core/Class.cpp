//
// Created by mathilde on 11/07/23.
//

#include "boringlang/core/Class.hpp"

using namespace BoringLang;


Class::Class(const std::string& name,
             Format format,
             Namespace* parent,
             int methods,
             int variables) : Class(name, format, parent, nullptr, methods, variables) {}

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
    _variableTypes = new Class* [_numberOfVariables];
}

Class::~Class() {
    for (int i = 0; i < _numberOfMethods; i++) {
        delete _methods[i];
    }

    delete[] _methods;
    delete[] _variableTypes;
}

Method* Class::getMethod(int index) {
    if(index < 0 || index >= _numberOfMethods)
        throw std::invalid_argument("Methods index is out of bounds.");

    Class* inspected = this;
    while(inspected->_methods[index] == nullptr && index < inspected->_definedMethodsStart)
        inspected = inspected->_superclass;

    return  inspected->_methods[index];
}

Method* Class::getMethod(const std::string& name) {
    return nullptr;
}

Class* Class::getVariableType(int index) {
    return nullptr;
}

Class* Class::getVariableType(const std::string& name) {
    return nullptr;
}

