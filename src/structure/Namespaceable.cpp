

#include "boringlang/structure/Namespaceable.hpp"


namespace BoringLang::Structure {

    Namespaceable::Namespaceable(std::string&& name, Namespace* ns) : _name(name), _namespace(ns){}

    const std::string& Namespaceable::getName() {
        return _name;
    }

    const Namespace* Namespaceable::getNamespace() {
        return _namespace;
    }
}
