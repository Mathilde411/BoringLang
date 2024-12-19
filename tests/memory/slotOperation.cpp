#include "boringlang/memory/SlotOperation.hpp"

#include "catch2/catch_all.hpp"

using namespace BoringLang::Memory;

TEST_CASE("testSlotToPointer", "[slotOperation]") {
    REQUIRE(slot_to_ptr(0x3FFFFFFFFFFFFFFFL) == 0x3FFFFFFFFFFFFFFFL);
    REQUIRE(slot_to_ptr(0x1c5b5c7510d82b2L) == 0x1c5b5c7510d82b2L);
    REQUIRE(slot_to_ptr(0x2ed2f2886073959L) == 0x2ed2f2886073959L);
}

TEST_CASE("testPointerToSlot", "[slotOperation]") {
    REQUIRE(ptr_to_slot(0x3FFFFFFFFFFFFFFFL) == 0x3FFFFFFFFFFFFFFFL);
    REQUIRE(ptr_to_slot(0x1c5b5c7510d82b2L) == 0x1c5b5c7510d82b2L);
    REQUIRE(ptr_to_slot(0x2ed2f2886073959L) == 0x2ed2f2886073959L);
}

TEST_CASE("testSlotToInteger", "[slotOperation]") {
    REQUIRE(slot_to_int(0x7FFFFFFFFFFF5BABL) == -42069L);
    REQUIRE(slot_to_int(0x5FFFFFFFFFFFFFFFL) == 2305843009213693951L);
    REQUIRE(slot_to_int(0x400000000000002A) == 42L);
}

TEST_CASE("testIntegerToSlot", "[slotOperation]") {
    REQUIRE(int_to_slot(-42069L) == 0x7FFFFFFFFFFF5BABL);
    REQUIRE(int_to_slot(2305843009213693951L) == 0x5FFFFFFFFFFFFFFFL);
    REQUIRE(int_to_slot(42L) == 0x400000000000002A);
}

TEST_CASE("testSlotToCharacter", "[slotOperation]") {
    REQUIRE(slot_to_char(0xc000000000000041) == U'A');
    REQUIRE(slot_to_char(0xc0000000000026A7) == U'⚧');
    REQUIRE(slot_to_char(0xc00000000001FA01) == U'🨁');
}

TEST_CASE("testCharacterToSlot", "[slotOperation]") {
    REQUIRE(char_to_slot(U'A') == 0xc000000000000041);
    REQUIRE(char_to_slot(U'⚧') == 0xc0000000000026A7);
    REQUIRE(char_to_slot(U'🨁') == 0xc00000000001FA01);
}

TEST_CASE("testSlotToFloat", "[slotOperation]") {
    REQUIRE(
        abs(slot_to_float(0x8ffe000000000000) - 1.5) < std::numeric_limits<Float>::epsilon()
    );
    REQUIRE(
        abs(slot_to_float(0x8fad3e2d6238da3c) - 0.00000125) < std::numeric_limits<Float>::epsilon()
    );
    REQUIRE(
        abs(slot_to_float(0x903ba12200000000) - 62500.25) < std::numeric_limits<Float>::epsilon()
    );
    REQUIRE(
        abs(slot_to_float(0xb03ba12200000000) + 62500.25) < std::numeric_limits<Float>::epsilon()
    );
    REQUIRE(std::isinf(slot_to_float(0x9ffc000000000000)));
    REQUIRE(std::isinf(-slot_to_float(0xbffc000000000000)));
    REQUIRE(std::isnan(slot_to_float(0x9ffe000000000000)));
}

TEST_CASE("testFloatToSlot", "[slotOperation]") {
    REQUIRE(float_to_slot(1.5) == 0x8ffe000000000000);
    REQUIRE(float_to_slot(0.00000125) == 0x8fad3e2d6238da3c);
    REQUIRE(float_to_slot(62500.25) == 0x903ba12200000000);
    REQUIRE(float_to_slot(-62500.25) == 0xb03ba12200000000);
    REQUIRE(float_to_slot(std::numeric_limits<Float>::infinity()) == 0x9ffc000000000000);
    REQUIRE(float_to_slot(-std::numeric_limits<Float>::infinity()) == 0xbffc000000000000);
    REQUIRE(float_to_slot(std::numeric_limits<Float>::quiet_NaN()) == 0x9ffe000000000000);
}
