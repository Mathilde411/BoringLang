//
// Created by mathilde on 11/07/23.
//

#include "boringlang/core/Class.hpp"

using namespace BoringLang;


Class::Class(const std::string& name,
             ClassFormat format,
             Namespace* parent,
             int sizeOfLiterals,
             int methods,
             int variables) : Class(name, format, parent, nullptr, sizeOfLiterals, methods, variables) {}

Class::Class(const std::string& name,
             ClassFormat format,
             Namespace* parent,
             Class* superclass,
             int sizeOfLiterals,
             int methods,
             int variables) : Namespaceable(name, parent) {
    _superclass = superclass;
    _format = format;

    _numberOfMethods = methods + (_superclass == nullptr ? 0 : _superclass->_numberOfMethods);
    _numberOfVariables = variables + (_superclass == nullptr ? 0 : _superclass->_numberOfVariables);
    _definedMethodsStart = methods - _numberOfMethods;
    _definedVariablesStart = variables - _numberOfVariables;
    _sizeOfLiterals = sizeOfLiterals;

    _methods = new Method* [_numberOfMethods];
    _variableTypes = new InstanceVariable* [_numberOfVariables];
    _literals = new uint64_t [_sizeOfLiterals];
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

InstanceVariable* Class::getVariable(int index) {
    if(index < 0 || index >= _numberOfMethods)
        throw std::invalid_argument("Variable index is out of bounds.");

    Class* inspected = this;
    while(inspected->_variableTypes[index] == nullptr && index < inspected->_definedVariablesStart)
        inspected = inspected->_superclass;

    return  inspected->_variableTypes[index];
}

