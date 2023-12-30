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

#include "boringlang/core/Class.hpp"

using namespace BoringLang;

Class::Class(ClassFile* classFile, ClassLoader* classLoader, Namespace* parent, std::string& name) : _classFile(
        classFile), _classLoader(classLoader), Namespaceable(name, parent), initialized(false), loaded(false) {
}

void Class::init() {

}

void Class::load() {

}
