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


#include "Namespaceable.hpp"
#include "Method.hpp"
#include "InstanceVariable.hpp"
#include "ClassFile.hpp"
#include <map>

namespace BoringLang {
    class ClassLoader;
    class Class : public Namespaceable {
    private:
        bool initialized{};
        bool loaded{};
        ClassLoader* _classLoader{};
        ClassFile* _classFile{};
        BvSlot* _constantPool{};

    public:
        Class(ClassFile* classFile, ClassLoader* classLoader, Namespace* parent, std::string& name);
        void init();
        void load();
    };
}

#include "ClassLoader.hpp"
#endif //BORINGLANG_CLASS_HPP
