//
// Created by mathilde on 11/07/23.
//

#ifndef BORINGVM_NAMESPACEABLE_HPP
#define BORINGVM_NAMESPACEABLE_HPP

#include <string>
#include <regex>

namespace BoringLang {
    class Namespace;

    class Namespaceable {
    protected:
        Namespace* _parent;
        std::string* _name;
        Namespace* _root;

        Namespaceable(std::string const& name, Namespace* parent);

    public:
        ~Namespaceable();

        std::string* getName();

        Namespace* getParent();

        virtual bool isNamespace();

        Namespace* getRoot();
    };
}

#include "Namespace.hpp"

#endif //BORINGVM_NAMESPACEABLE_HPP
