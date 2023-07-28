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

