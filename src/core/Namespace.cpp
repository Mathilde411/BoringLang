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
#include "boringlang/core/NamespacePath.hpp"
#include <algorithm>

using namespace BoringLang;


Namespaceable::Namespaceable(std::string const& name, Namespace* parent, ClassLoader* classLoader) {
    _parent = parent;
    _name.assign(name);
    _classLoader = classLoader;
    if(parent == nullptr) {
        _root = nullptr;
    } else {
        _root = parent->getRoot();
    }
    _handle = 0;
}

Namespaceable::~Namespaceable() {
    if(_parent != nullptr)
        _parent->removeChild(this);
    //_root->removeFromIndex(_handle);
}

const std::string& Namespaceable::getName() const {
    return _name;
}

Namespace* Namespaceable::getParent() const {
    return _parent;
}

bool Namespaceable::isNamespace() const {
    return false;
}

RootNamespace* Namespaceable::getRoot() const {
    return _root;
}

BvHandle Namespaceable::getHandle() const {
    return _handle;
}

void Namespaceable::setHandle(BvHandle handle) {
    _handle = handle;
}

Namespace::~Namespace() {
    for(Namespace* child : _subspaces) {
        delete child;
    }

    for(Class* clazz : _classes) {
        delete clazz;
    }
}

void Namespace::removeChild(Namespaceable* child) {
    if(child->isNamespace()) {
        _indexedSubspaces.erase(child->getName());
        _subspaces.erase(dynamic_cast<Namespace*>(child));
    } else {
        _indexedClasses.erase(child->getName());
        _classes.erase(dynamic_cast<Class*>(child));
    }
}

bool Namespace::isNamespace() const {
    return true;
}

Namespaceable* Namespace::findOrCreate(NamespacePath const& path) {
    Namespaceable* returnValue = path.isRooted() ? _root : this;
    int depth = path.getDepth();

    for(int i = 0; i < depth; i++) {
        std::string name = path.getComponents()[i];
        auto* ns = dynamic_cast<Namespace*>(returnValue);

        if(i == depth - 1) {
            if(!path.isNamespace()) {
                if(ns->_indexedClasses.contains(name)) {
                    return ns->_indexedClasses[name];
                }
                auto* clazz = new Class(name, ns, _classLoader);
                ns->_classes.insert(clazz);
                ns->_indexedClasses[name] = clazz;
                returnValue = clazz;
            } else {
                if(ns->_indexedSubspaces.contains(name)) {
                    return ns->_indexedSubspaces[name];
                }
                ns = new Namespace(name, ns, _classLoader);
                ns->_parent->_subspaces.insert(ns);
                ns->_parent->_indexedSubspaces[name] = ns;
                returnValue = ns;
            }
        } else {
            if(ns->_indexedSubspaces.contains(name)) {
                returnValue = ns->_indexedSubspaces[name];
            } else {
                ns = new Namespace(name, ns, _classLoader);
                ns->_parent->_subspaces.insert(ns);
                ns->_parent->_indexedSubspaces[name] = ns;
                returnValue = ns;
            }
        }
    }

    _root->addToIndex(returnValue);
    return returnValue;
}

Namespaceable* Namespace::findOrCreate(std::string const& path) {
    NamespacePath nsPath(path);
    return this->findOrCreate(nsPath);
}

Namespaceable* Namespace::find(NamespacePath const& path) {
    Namespaceable* returnValue = path.isRooted() ? _root : this;
    int depth = path.getDepth();
    for(int i = 0; i < depth; i++) {
        const std::string name = path.getComponents()[i];
        auto* ns = dynamic_cast<Namespace*>(returnValue);

        if(!path.isNamespace() && i == depth - 1) {
            if(ns->_indexedClasses.contains(name)) {
                returnValue = ns->_indexedClasses[name];
            } else {
                return nullptr;
            }
        } else {
            if(ns->_indexedSubspaces.contains(name)) {
                returnValue = ns->_indexedSubspaces[name];
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

bool stringEqualsIgnoreCase(const std::string& a, const std::string& b) {
    auto startA = a.begin();
    auto endA = a.end();
    auto startB = b.begin();
    auto endB = b.end();
    auto fun = [](char a, char b) {
        return tolower(a) < tolower(b);
    };

    auto res = std::lexicographical_compare(startA, endA, startB, endB, fun);

    return res;
}

RootNamespace::RootNamespace(ClassLoader* classLoader) : Namespace("", nullptr, classLoader) {
    _root = this;
    _index.push_back(this);
}

BvHandle RootNamespace::addToIndex(Namespaceable* namespaceable) {
    BvHandle res = _index.size();
    _index.push_back(namespaceable);
    namespaceable->setHandle(res);
    return res;
}

void RootNamespace::removeFromIndex(BvHandle handle) {
    if(handle >= _index.size())
        throw std::out_of_range("This handle does not exist");
    _index[handle] = nullptr;
}

Namespaceable* RootNamespace::getFromIndex(BvHandle handle) const {
    if(handle > _index.size())
        throw std::out_of_range("This handle does not exist");
    return _index[handle];
}

bool NamespaceableComparator::operator()(const Namespaceable* a, const Namespaceable* b) const {
    return stringEqualsIgnoreCase(a->getName(), b->getName());
}

bool NamespaceableNameComparator::operator()(const std::string& a, const std::string& b) const {
    return stringEqualsIgnoreCase(a, b);
}
