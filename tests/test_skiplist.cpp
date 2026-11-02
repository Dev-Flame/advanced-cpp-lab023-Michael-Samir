#include "catch.hpp"
#include "project/SkipList.hpp"
#include <string>

TEST_CASE("New skip list is empty") {
    SkipList<int, std::string> list;
    REQUIRE(list.empty());
    REQUIRE(list.size() == 0);
    REQUIRE(list.items().empty());
}

TEST_CASE("Insert and find") {
    SkipList<int, std::string> list(8, 0.5, 42);
    REQUIRE(list.insert(20, "twenty"));
    REQUIRE(list.insert(10, "ten"));
    REQUIRE(list.insert(30, "thirty"));
    REQUIRE(list.size() == 3);
    REQUIRE(list.contains(10));
    REQUIRE(list.contains(20));
    REQUIRE(list.contains(30));
    REQUIRE_FALSE(list.contains(99));
    REQUIRE(*list.find(20) == "twenty");
}

TEST_CASE("Duplicate key updates value without increasing size") {
    SkipList<int, std::string> list;
    REQUIRE(list.insert(7, "old"));
    REQUIRE_FALSE(list.insert(7, "new"));
    REQUIRE(list.size() == 1);
    REQUIRE(*list.find(7) == "new");
}

TEST_CASE("Items are returned in sorted order") {
    SkipList<int, std::string> list;
    list.insert(50, "fifty"); list.insert(10, "ten"); list.insert(40, "forty");
    list.insert(20, "twenty"); list.insert(30, "thirty");
    const auto items = list.items();
    REQUIRE(items.size() == 5);
    REQUIRE(items[0].first == 10);
    REQUIRE(items[1].first == 20);
    REQUIRE(items[2].first == 30);
    REQUIRE(items[3].first == 40);
    REQUIRE(items[4].first == 50);
}

TEST_CASE("Erase removes keys and preserves remaining keys") {
    SkipList<int, std::string> list;
    for (int i = 1; i <= 10; ++i) list.insert(i, std::to_string(i));
    REQUIRE(list.erase(1));
    REQUIRE(list.erase(5));
    REQUIRE(list.erase(10));
    REQUIRE_FALSE(list.erase(100));
    REQUIRE(list.size() == 7);
    REQUIRE_FALSE(list.contains(1));
    REQUIRE_FALSE(list.contains(5));
    REQUIRE_FALSE(list.contains(10));
    REQUIRE(list.contains(2));
    REQUIRE(list.contains(9));
}

TEST_CASE("Const find works") {
    SkipList<int, std::string> mutableList;
    mutableList.insert(12, "twelve");
    const SkipList<int, std::string>& list = mutableList;
    REQUIRE(list.find(12) != nullptr);
    REQUIRE(*list.find(12) == "twelve");
    REQUIRE(list.find(13) == nullptr);
}

TEST_CASE("Deterministic seeds preserve ordered contents") {
    SkipList<int, int> a(12, 0.5, 1234);
    SkipList<int, int> b(12, 0.5, 1234);
    for (int i = 0; i < 100; ++i) { a.insert(i, i*i); b.insert(i, i*i); }
    REQUIRE(a.items() == b.items());
    REQUIRE(a.size() == 100);
    REQUIRE(b.size() == 100);
}
