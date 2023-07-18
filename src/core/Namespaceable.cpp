//
// Created by mathilde on 11/07/23.
//

#include <iostream>
#include "boringlang/core/Namespaceable.hpp"

using namespace BoringLang;

Namespaceable::Namespaceable(std::string const& name, Namespace* parent) {
    _parent = parent;
    _name = new std::string(name);
    _root = parent == nullptr ? nullptr : parent->getRoot();

    std::cout << this << " created" << std::endl;
}

std::string* Namespaceable::getName() {
    return _name;
}

Namespace* Namespaceable::getParent() {
    return _parent;
}

bool Namespaceable::isNamespace() {
    return false;
}

Namespace* Namespaceable::getRoot() {
    return _root;
}

Namespaceable::~Namespaceable() {
    delete _name;
    _name = nullptr;
    std::cout << this << " destroyed" << std::endl;
}

