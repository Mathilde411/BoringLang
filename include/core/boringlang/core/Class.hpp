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

#ifndef BORINGLANG_CLASS_HPP
#define BORINGLANG_CLASS_HPP


#include "Namespaceable.hpp"
#include "Method.hpp"
#include "InstanceVariable.hpp"
#include "ClassFormat.hpp"
#include <map>

namespace BoringLang {
    class Class : public Namespaceable {
    protected:
        Class* _superclass;

        int _numberOfMethods;
        int _numberOfVariables;
        int _definedMethodsStart;
        int _definedVariablesStart;
        int _sizeOfLiterals;

        Method** _methods;
        InstanceVariable** _variableTypes;
        uint64_t* _literals;

        ClassFormat _format;

    public:
        Class(std::string const& name,
              ClassFormat format,
              Namespace* ns,
              int sizeOfLiterals,
              int methods,
              int variables);

        Class(std::string const& name,
              ClassFormat format,
              Namespace* ns,
              Class* superclass,
              int sizeOfLiterals,
              int methods,
              int variables);

        ~Class();

        Method* getMethod(int index);

        InstanceVariable* getVariable(int index);
    };
}


#endif //BORINGLANG_CLASS_HPP
