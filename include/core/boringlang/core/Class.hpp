//
// Created by mathilde on 11/07/23.
//

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
