//
// Created by mathilde on 11/07/23.
//

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
