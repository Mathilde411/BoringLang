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

#include "boringlang/core/Namespace.hpp"
#include <algorithm>

using namespace BoringLang;


Namespace::Namespace() : Namespaceable("", nullptr) {
    _root = this;
}

Namespace::Namespace(const std::string& name, Namespace* parent) : Namespaceable(name, parent) {}

Namespace::~Namespace() {
    for (Namespace* child: _children) {
        delete child;
    }

    for (Class* clazz: _classes) {
        delete clazz;
    }
}

void Namespace::removeChild(Namespace* child) {

    _indexedChildren.erase(*child->getName());
    _children.erase(child);
}


void Namespace::destroy() {
    delete this;
}

bool Namespace::isNamespace() {
    return true;
}

Namespaceable* Namespace::findOrCreate(NamespacePath const& path) {

    Namespaceable* returnValue = path.isRooted() ? _root : this;
    int depth = path.getDepth();

    for (int i = 0; i < depth; i++) {
        std::string name = path.getComponents()[i];
        Namespace* ns = ((Namespace*) returnValue);

        if (!path.isNamespace() && i == depth - 1) {
            if (ns->_indexedClasses.count(name) > 0) {
                returnValue = ns->_indexedClasses[name];
            } else {
                return nullptr;
            }
        } else {
            if (ns->_indexedChildren.count(name) > 0) {
                returnValue = ns->_indexedChildren[name];
            } else {
                ns = new Namespace(name, ns);
                ns->_parent->_children.insert(ns);
                ns->_parent->_indexedChildren[name] = ns;
                returnValue = ns;
            }
        }

    }

    return returnValue;
}

Namespaceable* Namespace::findOrCreate(std::string const& path) {
    NamespacePath nsPath(path);
    return this->findOrCreate(nsPath);
}

Namespaceable* Namespace::find(NamespacePath const& path) {
    Namespaceable* returnValue = path.isRooted() ? _root : this;
    int depth = path.getDepth();
    for (int i = 0; i < depth; i++) {
        std::string name = path.getComponents()[i];
        Namespace* ns = ((Namespace*) returnValue);

        if (!path.isNamespace() && i == depth - 1) {
            if (ns->_indexedClasses.count(name) > 0) {
                returnValue = ns->_indexedClasses[name];
            } else {
                return nullptr;
            }
        } else {
            if (ns->_indexedChildren.count(name) > 0) {
                returnValue = ns->_indexedChildren[name];
            } else {
                return nullptr;
            }
        }
    }

    return returnValue;
}

Namespaceable* Namespace::find(std::string const& path) {
    NamespacePath nsPath(path);
    return this->find(nsPath);
}

void Namespace::addClass(Class* clazz) {



}

bool stringEqualsIgnoreCase(const std::string& a, const std::string& b) {
    return std::equal(a.begin(), a.end(),
                      b.begin(), b.end(),
                      [](char a, char b) {
                          return tolower(a) == tolower(b);
                      });
}

bool NamespaceableComparator::operator()(Namespaceable* a, Namespaceable* b) const {
    return stringEqualsIgnoreCase(*a->getName(), *b->getName());
}

bool NamespaceableNameComparator::operator()(const std::string& a, const std::string& b) const {
    return stringEqualsIgnoreCase(a, b);
}
