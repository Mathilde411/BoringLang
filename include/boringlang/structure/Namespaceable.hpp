

#ifndef BORINGLANG_NAMESPACEABLE_HPP
#define BORINGLANG_NAMESPACEABLE_HPP


#include <string>

namespace BoringLang::Structure {

    class Namespace;

    class Namespaceable {
    private:
        std::string _name;
        const Namespace* _namespace;

    public:
        Namespaceable(std::string&& name, Namespace* ns);
        const std::string& getName();
        const Namespace* getNamespace();
    };

}
#include "Namespace.hpp"
#endif //BORINGLANG_NAMESPACEABLE_HPP
