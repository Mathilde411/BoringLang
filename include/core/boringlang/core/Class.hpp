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
    class Class : Namespaceable {
    protected:
        Class* _superclass;

        int _numberOfMethods;
        int _numberOfVariables;
        int _definedMethodsStart;
        int _definedVariablesStart;

        std::map<std::string, int> _methodsNames;
        std::map<std::string, int> _variablesNames;

        Method** _methods;
        Class** _variables;

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
    };
}


#endif //BORINGVM_CLASS_HPP
