//
// Created by mathilde on 12/07/23.
//

#ifndef BORINGVM_NAMESPACEPATH_HPP
#define BORINGVM_NAMESPACEPATH_HPP

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

        NamespacePath(NamespacePath const& parent, std::string const& child);

        ~NamespacePath();

        static bool isNameValid(std::string const& name);

        static bool isPathValid(std::string const& name);

        std::string getFullyQualified();

        int getDepth() const;

        std::string* getComponents() const;

        bool isNamespace() const;

        bool isRooted() const;
    };

} // BoringVM

#endif //BORINGVM_NAMESPACEPATH_HPP
