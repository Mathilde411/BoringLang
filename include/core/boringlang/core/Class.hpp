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

#ifndef BORINGLANG_CLASS_HPP
#define BORINGLANG_CLASS_HPP

#include "Method.hpp"
#include "InstanceVariable.hpp"
#include "ClassFile.hpp"
#include "Namespace.hpp"

namespace BoringLang {
    class ClassLoader;
    class Class : public Namespaceable {
    private:
        bool _initialized = false;
        ClassLoader* _classLoader{};
        ClassFile* _classFile{};
        BvSlot* _constantPool{};

    public:
        Class(const std::string& name, Namespace* parent, ClassLoader* classLoader) : Namespaceable(name, parent, classLoader) {};
        void init();
        void setClassFile(ClassFile* file);
    };
}


#endif //BORINGLANG_CLASS_HPP
