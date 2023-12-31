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
#include <map>
#include <vector>

#include "util/TypesUtil.hpp"

namespace BoringLang {
    class ClassLoader;
}

namespace BoringLang {
    class NamespacePath;
    class Class;
    class Namespace;
    class RootNamespace;
    class Namespaceable;

    struct NamespaceableComparator {
        bool operator()(const Namespaceable* a, const Namespaceable* b) const;
    };

    struct NamespaceableNameComparator {
        bool operator()(std::string const& a, std::string const& b) const;
    };

    class Namespaceable {
    protected:
        Namespace* _parent;
        std::string _name;
        ClassLoader* _classLoader;
        RootNamespace* _root;
        BvHandle _handle;

    public:

        Namespaceable(std::string const& name, Namespace* parent, ClassLoader* classLoader);
        virtual ~Namespaceable();

        [[nodiscard]]
        const std::string& getName() const;

        [[nodiscard]]
        Namespace* getParent() const;

        [[nodiscard]]
        virtual bool isNamespace() const;

        [[nodiscard]]
        RootNamespace* getRoot() const ;

        [[nodiscard]]
        BvHandle getHandle() const;
        void setHandle(BvHandle handle);
    };

    class Namespace : public Namespaceable {

    protected:
        std::set<Namespace*, NamespaceableComparator> _subspaces;
        std::map<std::string, Namespace*, NamespaceableNameComparator> _indexedSubspaces;

        std::set<Class*, NamespaceableComparator> _classes;
        std::map<std::string, Class*, NamespaceableNameComparator> _indexedClasses;

        void removeChild(Namespaceable* pNamespace);
        friend Namespaceable::~Namespaceable();

        Namespace(const std::string& name, Namespace* parent, ClassLoader* classLoader) : Namespaceable(name, parent, classLoader) {}

    public:

        ~Namespace() override;

        [[nodiscard]]
        bool isNamespace() const override;

        Namespaceable* findOrCreate(NamespacePath const& path);

        Namespaceable* findOrCreate(std::string const& path);

        [[nodiscard]]
        Namespaceable* find(NamespacePath const& path);

        [[nodiscard]]
        Namespaceable* find(std::string const& path);
    };

    class RootNamespace : public Namespace {
    protected:
        std::vector<Namespaceable*> _index;

        BvHandle addToIndex(Namespaceable* namespaceable);
        void removeFromIndex(BvHandle handle);


    public:
        friend Namespaceable* Namespace::findOrCreate(NamespacePath const& path);
        friend Namespaceable::~Namespaceable();
        explicit RootNamespace(ClassLoader* classLoader);

        [[nodiscard]]
        Namespaceable* getFromIndex(BvHandle handle) const;
    };

}

#include "Class.hpp"

#endif //BORINGLANG_NAMESPACE_HPP
