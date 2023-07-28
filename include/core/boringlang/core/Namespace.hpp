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

#ifndef BORINGLANG_NAMESPACE_HPP
#define BORINGLANG_NAMESPACE_HPP


#include <string>
#include <set>
#include "Namespaceable.hpp"

namespace BoringLang {
    class NamespacePath;
    class Class;

    struct NamespaceableComparator {
        bool operator()(Namespaceable* a, Namespaceable* b) const;
    };

    struct NamespaceableNameComparator {
        bool operator()(std::string const& a, std::string const& b) const;
    };

    class Namespace : public Namespaceable {

    protected:
        std::set<Namespace*, NamespaceableComparator> _children;
        std::map<std::string, Namespace*, NamespaceableNameComparator> _indexedChildren;

        std::set<Class*, NamespaceableComparator> _classes;
        std::map<std::string, Class*, NamespaceableNameComparator> _indexedClasses;

        Namespace(std::string const& name, Namespace* parent);

    public:
        Namespace();

        ~Namespace();

        void destroy();

        void removeChild(Namespace* pNamespace);

        void addClass(Class* clazz);

        bool isNamespace() override;

        Namespaceable* findOrCreate(NamespacePath const& path);

        Namespaceable* findOrCreate(std::string const& path);

        Namespaceable* find(NamespacePath const& path);

        Namespaceable* find(std::string const& path);
    };

}

#include "Class.hpp"
#include "NamespacePath.hpp"

#endif //BORINGLANG_NAMESPACE_HPP
