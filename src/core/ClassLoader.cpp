//     BoringLang Project
//     Copyright (C) 2023 Mathilde Pascal
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

#include "boringlang/core/ClassLoader.hpp"

using namespace BoringLang;

Class* ClassLoader::lazyLoad(ClassFile* classFile) {
    std::string namespaceString;
    PrimitivesUtil::copyUnslotedString(classFile->getNamespace(), namespaceString);
    std::string className;
    PrimitivesUtil::copyUnslotedString(classFile->getClassName(), className);

    NamespacePath path(namespaceString);

    if(!path.isNamespace())
        throw ClassLoadingError("Namespace is invalid.");

    if(!NamespacePath::isNameValid(className))
        throw ClassLoadingError("Class name is invalid.");

    auto* ns = (Namespace*)_root.findOrCreate(path);

    if(ns->find(className) != nullptr)
        throw ClassLoadingError("A class with this name already exists in the same namespace.");

    auto* clazz = new Class(classFile, this, ns, className);
    ns->addClass(clazz);

    clazz->load();
    return clazz;
}

Class* ClassLoader::fullLoadLoad(ClassFile* classFile) {
    Class* clazz = lazyLoad(classFile);
    clazz->init();
    return clazz;
}

Class* ClassLoader::lazyLoad(std::istream& classFile) {
    ClassFile* file = nullptr;
    Class* clazz = nullptr;
    try {
        file = new ClassFile();
        file->importClass(classFile);
        clazz = lazyLoad(file);
    } catch (ClassLoadingError& e) {
        delete file;
        delete clazz;
        throw;
    }
    return clazz;
}

Class* ClassLoader::fullLoadLoad(std::istream& classFile) {
    ClassFile* file = nullptr;
    Class* clazz = nullptr;
    try {
        file = new ClassFile();
        file->importClass(classFile);
        clazz = fullLoadLoad(file);
    } catch (ClassLoadingError& e) {
        delete file;
        delete clazz;
        throw;
    }
    return clazz;
}
