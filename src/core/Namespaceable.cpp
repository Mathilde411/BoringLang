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

#include <iostream>
#include "boringlang/core/Namespaceable.hpp"

using namespace BoringLang;

Namespaceable::Namespaceable(std::string const& name, Namespace* parent) {
    _parent = parent;
    _name = new std::string(name);
    _root = parent == nullptr ? nullptr : parent->getRoot();
}

std::string* Namespaceable::getName() {
    return _name;
}

void Namespaceable::setName(const std::string &name) {
    _name = new std::string(name);
}

Namespace* Namespaceable::getParent() {
    return _parent;
}

void Namespaceable::setParent(Namespace *parent) {
    _parent = parent;
    _root = parent == nullptr ? nullptr : parent->getRoot();
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
}

