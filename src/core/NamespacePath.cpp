//
// Created by mathilde on 12/07/23.
//

#include <iostream>
#include "boringlang/core/NamespacePath.hpp"

using namespace BoringLang;

std::regex NamespacePath::NAME_REGEX("^[a-zA-Z_$][a-zA-Z_$0-9]*$");
std::regex NamespacePath::PATH_SEPARATION_REGEX(
        "^(\\/?(?:(?:[a-zA-Z_$][a-zA-Z_$0-9]*)+\\/)*)([a-zA-Z_$][a-zA-Z_$0-9]*)?$");

bool NamespacePath::isNameValid(const std::string& name) {
    return std::regex_match(name, NamespacePath::NAME_REGEX);
}

bool NamespacePath::isPathValid(const std::string& name) {
    return std::regex_match(name, NamespacePath::PATH_SEPARATION_REGEX);
}

NamespacePath::NamespacePath() : NamespacePath("/") {}

NamespacePath::NamespacePath(Namespaceable* namespaceable) {

    _representsNamespace = namespaceable->isNamespace();
    _depth = 0;
    _rooted = true;

    std::stack<Namespaceable*> namespaceStack;
    Namespaceable* currentNamespace = namespaceable;
    while (currentNamespace != nullptr && currentNamespace->getParent() != nullptr) {
        namespaceStack.push(currentNamespace);
        _depth += 1;
        currentNamespace = currentNamespace->getParent();
    }

    _components = new std::string[_depth];

    for (int i = 0; i < _depth; i++) {
        _components[i] = *(namespaceStack.top()->getName());
        namespaceStack.pop();
    }
}

NamespacePath::NamespacePath(const std::string& path) {


    _components = nullptr;
    _depth = 0;
    _representsNamespace = false;
    _rooted = (path[0] == '/');

    std::smatch fullPathMatch;
    if (std::regex_search(path, fullPathMatch, PATH_SEPARATION_REGEX)) {

        std::string ns = fullPathMatch[1].str();
        std::string className;
        _representsNamespace = (fullPathMatch.size() != 3) || (className = fullPathMatch[2].str()).empty();


        std::vector<std::string> components;
        size_t begin = 0;
        size_t end;
        while ((end = ns.find('/', begin)) != std::string::npos) {
            if (end > begin)
                components.push_back(ns.substr(begin, end - begin));
            begin = end + 1;
        }

        _depth = ((int) components.size()) + (_representsNamespace ? 0 : 1);
        _components = new std::string[_depth];

        for (int i = 0; i < components.size(); i++)
            _components[i] = components[i];

        if (!_representsNamespace)
            _components[_depth - 1] = className;
    } else {
        throw std::invalid_argument("The path is invalid.");
    }
}

NamespacePath::~NamespacePath() {
    delete[] _components;
}

std::string NamespacePath::getFullyQualified() {
    std::string path;

    for (int i = 0; i < _depth; i++)
        path += '/' + _components[i];

    if (_representsNamespace)
        path += '/';

    return path;
}

int NamespacePath::getDepth() const {
    return _depth;
}

std::string* NamespacePath::getComponents() const {
    return _components;
}

bool NamespacePath::isNamespace() const {
    return _representsNamespace;
}

bool NamespacePath::isRooted() const {
    return _rooted;
}

