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

#ifndef BORINGLANG_NAMESPACEPATH_HPP
#define BORINGLANG_NAMESPACEPATH_HPP

#include <regex>
#include "Namespace.hpp"

namespace BoringLang {
    class NamespacePath {
    protected:
        bool _representsNamespace;
        bool _rooted;
        int _depth;
        std::string* _components;

        static std::regex NAME_REGEX;
        static std::regex PATH_SEPARATION_REGEX;
        static std::regex COMPONENT_SEPARATION_REGEX;

    public:

        NamespacePath();

        explicit NamespacePath(Namespaceable* namespaceable);

        explicit NamespacePath(std::string const& path);

        ~NamespacePath();

        static bool isNameValid(std::string const& name);

        static bool isPathValid(std::string const& name);

        std::string getFullyQualified();

        [[nodiscard]] int getDepth() const;

        [[nodiscard]] std::string* getComponents() const;

        [[nodiscard]] bool isNamespace() const;

        [[nodiscard]] bool isRooted() const;
    };

} // BoringVM

#endif //BORINGLANG_NAMESPACEPATH_HPP
