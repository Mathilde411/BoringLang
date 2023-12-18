#include "boringlang/core/Class.hpp"

using namespace BoringLang;

Class::Class(ClassFile* classFile, ClassLoader* classLoader, Namespace* parent, std::string& name) :
    _classFile(classFile), _classLoader(classLoader), Namespaceable(name, parent) {
}

void Class::init() {
}

void Class::load() {

}
