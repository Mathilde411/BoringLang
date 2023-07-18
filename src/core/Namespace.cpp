//
// Created by mathilde on 11/07/23.
//

#include "boringlang/core/Namespace.hpp"


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

    Namespace* returnValue = path.isRooted() ? _root : this;

    for (int i = 0; i < path.getDepth(); i++) {
        std::string name = path.getComponents()[i];
        if (returnValue->_indexedChildren.count(name) > 0) {
            if (!path.isNamespace()) {
                throw std::invalid_argument("Not implemented.");
            } else {
                returnValue = returnValue->_indexedChildren[name];
            }
        } else {
            if (!path.isNamespace()) {
                throw std::invalid_argument("The path points to a non-existent class-like member.");
            } else {
                returnValue = new Namespace(name, returnValue);
                returnValue->_parent->_children.insert(returnValue);
                returnValue->_parent->_indexedChildren[name] = returnValue;
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
    Namespace* returnValue = path.isRooted() ? _root : this;

    for (int i = 0; i < path.getDepth(); i++) {
        std::string name = path.getComponents()[i];
        if (returnValue->_indexedChildren.count(name) > 0) {
            if (!path.isNamespace()) {
                throw std::invalid_argument("Not implemented.");
            } else {
                returnValue = returnValue->_indexedChildren[name];
            }
        } else {
            return nullptr;
        }
    }

    return returnValue;
}

Namespaceable* Namespace::find(std::string const& path) {
    NamespacePath nsPath(path);
    return this->find(nsPath);
}
