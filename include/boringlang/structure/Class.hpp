

#ifndef BORINGLANG_CLASS_HPP
#define BORINGLANG_CLASS_HPP

#include <cstdint>


namespace BoringLang::Structure {
    class Class {
    private:
        uint32_t _classPointer;

    public:
        [[nodiscard]] uint32_t getClassPointer() const;
        [[nodiscard]] uint64_t getSlotNumber() const;
    };
}


#endif //BORINGLANG_CLASS_HPP
