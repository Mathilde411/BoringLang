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

#include <catch2/catch_all.hpp>
#include "boringlang/core/util/TypesUtil.hpp"


TEST_CASE("testBuildFromSlot1", "[testBuildFromSlot1]") {
    BoringLang::BvSlot slot =
            (((BoringLang::BvSlot)BoringLang::INT_TYPE) << 56)
            | 8;
    BoringLang::ObjectHeader header(&slot);

    REQUIRE(header.isPrimitive());
    REQUIRE_FALSE(header.isBytes());
    REQUIRE_FALSE(header.isDoubleHeader());
    REQUIRE(header.getFormat() == BoringLang::INT_TYPE);
    REQUIRE(header.getSize() == 8);
    REQUIRE(header.getSlotSize() == 1);
}

TEST_CASE("testBuildFromSlot2", "[testBuildFromSlot2]") {
    BoringLang::BvSlot slot =
            (((BoringLang::BvSlot)BoringLang::STRING_TYPE) << 56)
            | 42;
    BoringLang::ObjectHeader header(&slot);

    REQUIRE(header.isPrimitive());
    REQUIRE(header.isBytes());
    REQUIRE_FALSE(header.isDoubleHeader());
    REQUIRE(header.getFormat() == BoringLang::STRING_TYPE);
    REQUIRE(header.getSize() == 42);
    REQUIRE(header.getSlotSize() == 6);
    REQUIRE(header.getSlotSizeWithHeader() == 7);
}

TEST_CASE("testBuildFromSlot3", "[testBuildFromSlot3]") {
    BoringLang::BvSlot slot =(1ULL << 63) |
                             (42ULL << 56) |
                             (0x012345ULL << 32) |
                             (0x67ULL << 24) |
                             (0x89ABCDULL);
    BoringLang::ObjectHeader header(&slot);

    REQUIRE_FALSE(header.isPrimitive());
    REQUIRE_FALSE(header.isDoubleHeader());
    REQUIRE(header.getSize() == 42);
    REQUIRE(header.getSlotSize() == 6);
    REQUIRE(header.getSlotSizeWithHeader() == 7);
    REQUIRE(header.getHash() == 0x012345);
    REQUIRE(header.getFlags() == 0x67);
    REQUIRE(header.getClassIndex() == 0x89ABCD);
}

TEST_CASE("testBuildFromSlot4", "[testBuildFromSlot4]") {
    BoringLang::BvSlot slot[] = {
            (0xFFULL << 56) | 99999999,
            (0xFFULL << 56) |
            (0x012345ULL << 32) |
            (0x67ULL << 24) |
            (0x89ABCDULL)
    };
    BoringLang::ObjectHeader header(slot);

    REQUIRE_FALSE(header.isPrimitive());
    REQUIRE(header.isDoubleHeader());
    REQUIRE(header.getSize() == 99999999);
    REQUIRE(header.getSlotSize() == 12500000);
    REQUIRE(header.getSlotSizeWithHeader() == 12500002);
    REQUIRE(header.getHash() == 0x012345);
    REQUIRE(header.getFlags() == 0x67);
    REQUIRE(header.getClassIndex() == 0x89ABCD);
}

TEST_CASE("testBuildFromArgs1", "[testBuildFromArgs1]") {
    BoringLang::BvSlot slot =
            (((BoringLang::BvSlot)BoringLang::INT_TYPE) << 56)
            | 8;
    BoringLang::BvSlot newSlot;

    BoringLang::ObjectHeader header(BoringLang::INT_TYPE, 8);
    header.exportHeader(&newSlot);


    REQUIRE(slot == newSlot);
}

TEST_CASE("testBuildFromArgs2", "[testBuildFromArgs2]") {
    BoringLang::BvSlot slot =
            (((BoringLang::BvSlot)BoringLang::STRING_TYPE) << 56)
            | 42;
    BoringLang::BvSlot newSlot;

    BoringLang::ObjectHeader header(BoringLang::STRING_TYPE, 42);
    header.exportHeader(&newSlot);


    REQUIRE(slot == newSlot);
}

TEST_CASE("testBuildFromArgs3", "[testBuildFromArgs3]") {
    BoringLang::BvSlot slot =(1ULL << 63) |
                             (42ULL << 56) |
                             (0x012345ULL << 32) |
                             (0x67ULL << 24) |
                             (0x89ABCDULL);
    BoringLang::BvSlot newSlot;

    BoringLang::ObjectHeader header(42, 0x012345, 0x89ABCD, 0x67);
    header.exportHeader(&newSlot);


    REQUIRE(slot == newSlot);
}

TEST_CASE("testBuildFromArgs4", "[testBuildFromArgs4]") {
    BoringLang::BvSlot slot[] = {
            (0xFFULL << 56) | 99999999,
            (0xFFULL << 56) |
            (0x012345ULL << 32) |
            (0x67ULL << 24) |
            (0x89ABCDULL)
    };
    BoringLang::BvSlot newSlot[2];

    BoringLang::ObjectHeader header(99999999, 0x012345, 0x89ABCD, 0x67);
    header.exportHeader(newSlot);


    REQUIRE(slot[0] == newSlot[0]);
    REQUIRE(slot[1] == newSlot[1]);
}

TEST_CASE("testNextObject", "[testNextObject]") {
    BoringLang::BvSlot slot[8];
    slot[0] = (1ULL << 63) |
    (42ULL << 56) |
    (0x012345ULL << 32) |
    (0x67ULL << 24) |
    (0x89ABCDULL);

    REQUIRE(BoringLang::ObjectHeader::nextObject(slot) == slot + 7);
}