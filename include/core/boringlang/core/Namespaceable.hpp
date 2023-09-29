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

#ifndef BORINGLANG_NAMESPACEABLE_HPP
#define BORINGLANG_NAMESPACEABLE_HPP

#include <string>
#include <regex>

namespace BoringLang {
    class Namespace;

    class Namespaceable {
    protected:
        Namespace* _parent;
        std::string* _name;
        Namespace* _root;

        Namespaceable(std::string const& name, Namespace* parent);

    public:
        ~Namespaceable();

        std::string* getName();
        void setName(std::string const& name);

        Namespace* getParent();
        void setParent(Namespace* parent);

        virtual bool isNamespace();

        Namespace* getRoot();
    };
}

#include "Namespace.hpp"

#endif //BORINGLANG_NAMESPACEABLE_HPP
