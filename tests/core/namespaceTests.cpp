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
#include "boringlang/core/Namespace.hpp"

TEST_CASE("testIsNamespace", "[testIsNamespace]") {
    BoringLang::RootNamespace ns(nullptr);
    REQUIRE(ns.isNamespace());
}

TEST_CASE("testNoChildInEmptyNamespace", "[testNoChildInEmptyNamespace]") {
    BoringLang::RootNamespace ns(nullptr);
    REQUIRE(ns.find("test/") == nullptr);
}

TEST_CASE("testRootIsSlash", "[testRootIsSlash]") {
    BoringLang::RootNamespace ns(nullptr);
    REQUIRE(ns.find("/") == &ns);
}

TEST_CASE("testSelf", "[testSelf]") {
    BoringLang::RootNamespace ns(nullptr);
    BoringLang::Namespace* child = dynamic_cast<BoringLang::Namespace*>(ns.findOrCreate("test/"));
    REQUIRE(ns.find("") == &ns);
    REQUIRE(child->find("") == child);
}

TEST_CASE("testCaseInsensitiveFind", "[testCaseInsensitiveFind]") {
    BoringLang::RootNamespace ns(nullptr);
    BoringLang::Namespace* child = dynamic_cast<BoringLang::Namespace*>(ns.findOrCreate("test/"));
    REQUIRE(ns.find("test/") == child);
    REQUIRE(ns.find("TEST/") == child);
}

TEST_CASE("testFindOrCreateReturnsExisting", "[testFindOrCreateReturnsExisting]") {
    BoringLang::RootNamespace ns(nullptr);
    BoringLang::Namespace* child = dynamic_cast<BoringLang::Namespace*>(ns.findOrCreate("test/"));
    REQUIRE(ns.findOrCreate("test/") == child);
}

TEST_CASE("testChildFindsRoot", "[testChildFindsRoot]") {
    BoringLang::RootNamespace ns(nullptr);
    BoringLang::Namespace* child = dynamic_cast<BoringLang::Namespace*>(ns.findOrCreate("test/"));
    REQUIRE(child->find("/") == &ns);
}

TEST_CASE("testClassIsNull", "[testChildFindsRoot]") {
    BoringLang::RootNamespace ns(nullptr);
    ns.findOrCreate("test/");
    REQUIRE(ns.find("test") == nullptr);
}

TEST_CASE("testFindFromRoot", "[testChildFindsRoot]") {
    BoringLang::RootNamespace ns(nullptr);
    BoringLang::Namespace* child = dynamic_cast<BoringLang::Namespace*>(ns.findOrCreate("test/"));
    REQUIRE(child->find("/test/") == child);
}

TEST_CASE("testComplexTree", "[testChildFindsRoot]") {
    BoringLang::RootNamespace ns(nullptr);
    BoringLang::Namespace* child1 = dynamic_cast<BoringLang::Namespace*>(ns.findOrCreate("test1/"));
    BoringLang::Namespace* child2 = dynamic_cast<BoringLang::Namespace*>(ns.findOrCreate("test2/"));
    BoringLang::Namespace* child3 = dynamic_cast<BoringLang::Namespace*>(child2->findOrCreate("test1/"));
    REQUIRE(child2->find("/test1/") == child1);
    REQUIRE(child2->find("test1/") == child3);
}

TEST_CASE("testWrongNameCreation", "[testWrongNameCreation]") {
    BoringLang::RootNamespace ns(nullptr);
    REQUIRE_THROWS_AS(ns.findOrCreate("*"), std::invalid_argument);
    REQUIRE_THROWS_AS(ns.findOrCreate("//test"), std::invalid_argument);
    REQUIRE_THROWS_AS(ns.findOrCreate("/te st"), std::invalid_argument);
}
