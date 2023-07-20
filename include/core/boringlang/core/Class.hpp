//
// Created by mathilde on 11/07/23.
//

#ifndef BORINGVM_CLASS_HPP
#define BORINGVM_CLASS_HPP


#include "Namespaceable.hpp"
#include "Method.hpp"
#include "Format.hpp"
#include <map>

namespace BoringLang {
    class Class : public Namespaceable {
    protected:
        Class* _superclass;

        int _numberOfMethods;
        int _numberOfVariables;
        int _definedMethodsStart;
        int _definedVariablesStart;

        std::map<std::string, int> _methodsNames;
        std::map<std::string, int> _variablesNames;

        Method** _methods;
        Class** _variableTypes;

        Format _format;

    public:
        Class(std::string const& name,
              Format format,
              Namespace* ns,
              int methods,
              int variables);

        Class(std::string const& name,
              Format format,
              Namespace* ns,
              Class* superclass,
              int methods,
              int variables);

        ~Class();

        Method* getMethod(int index);

        Method* getMethod(std::string const& name);

        Class* getVariableType(int index);

        Class* getVariableType(std::string const& name);
    };
}


#endif //BORINGVM_CLASS_HPP
