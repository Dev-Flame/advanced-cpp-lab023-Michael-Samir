#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "project/BalancedTrees.hpp"
#include <algorithm>
#include <memory>
#include <random>

TEST_CASE("AVL insertion preserves ordering and balance") {
    project::AVLTree<int, int> tree;
    for (int k : {30, 20, 10, 25, 28, 27, 40, 50, 45, 5})
        tree.insert(k, k);

    REQUIRE(tree.size() == 10);
    REQUIRE(tree.isValidAVL());
    REQUIRE(tree.inorder() == std::vector<int>{5,10,20,25,27,28,30,40,45,50});
}

TEST_CASE("AVL search and duplicate update semantics") {
    project::AVLTree<int, std::string> tree;
    tree.insert(10, "ten");
    tree.insert(5, "five");
    tree.insert(15, "fifteen");
    tree.insert(10, "TEN");

    REQUIRE(tree.size() == 3);
    REQUIRE(tree.contains(10));
    REQUIRE(tree.find(10) != nullptr);
    REQUIRE(*tree.find(10) == "TEN");
    REQUIRE_FALSE(tree.contains(99));
}

TEST_CASE("AVL deletion preserves balance") {
    project::AVLTree<int, int> tree;
    for (int i = 1; i <= 100; ++i) tree.insert(i, i);

    for (int i = 1; i <= 100; i += 2)
        REQUIRE(tree.erase(i));

    REQUIRE(tree.isValidAVL());
    REQUIRE(tree.size() == 50);
    REQUIRE_FALSE(tree.erase(999));
}

TEST_CASE("Splay search moves accessed node to root") {
    project::SplayTree<int, int> tree;
    for (int k : {50, 30, 70, 20, 40, 60, 80})
        tree.insert(k, k);

    REQUIRE(tree.contains(20));
    REQUIRE(tree.rootKey() != nullptr);
    REQUIRE(*tree.rootKey() == 20);

    REQUIRE(tree.contains(60));
    REQUIRE(*tree.rootKey() == 60);
}

TEST_CASE("Splay maintains BST ordering through updates") {
    project::SplayTree<int, int> tree;
    for (int k : {8, 3, 10, 1, 6, 14, 4, 7, 13})
        tree.insert(k, k);

    REQUIRE(tree.inorder() == std::vector<int>{1,3,4,6,7,8,10,13,14});
    REQUIRE(tree.size() == 9);
}

TEST_CASE("Splay deletion preserves ordering") {
    project::SplayTree<int, int> tree;
    for (int i = 1; i <= 20; ++i) tree.insert(i, i);

    REQUIRE(tree.erase(10));
    REQUIRE_FALSE(tree.contains(10));
    REQUIRE_FALSE(tree.erase(10));
    REQUIRE(tree.inorder() == std::vector<int>{
        1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20
    });
}

TEST_CASE("Empty trees report consistent state") {
    project::AVLTree<int, int> avl;
    project::SplayTree<int, int> splay;

    REQUIRE(avl.empty());
    REQUIRE(avl.size() == 0);
    REQUIRE(avl.height() == 0);
    REQUIRE(avl.inorder().empty());
    REQUIRE(avl.isValidAVL());

    REQUIRE(splay.empty());
    REQUIRE(splay.size() == 0);
    REQUIRE(splay.rootKey() == nullptr);
    REQUIRE(splay.inorder().empty());
    REQUIRE(splay.depth() == 0);
}

TEST_CASE("AVL height remains logarithmic for sorted insertions") {
    project::AVLTree<int, int> tree;
    for (int key = 1; key <= 127; ++key)
        tree.insert(key, key);

    REQUIRE(tree.isValidAVL());
    REQUIRE(tree.height() <= 8);
    REQUIRE(tree.inorder().front() == 1);
    REQUIRE(tree.inorder().back() == 127);
}

TEST_CASE("AVL supports a custom key ordering") {
    project::AVLTree<int, int, std::greater<int>> tree;
    for (int key : {2, 5, 1, 4, 3})
        tree.insert(key, key);

    REQUIRE(tree.isValidAVL());
    REQUIRE(tree.inorder() == std::vector<int>{5, 4, 3, 2, 1});
    REQUIRE(tree.contains(4));
    REQUIRE_FALSE(tree.contains(9));
}

TEST_CASE("AVL move construction transfers ownership") {
    project::AVLTree<int, std::string> original;
    original.insert(2, "two");
    original.insert(1, "one");
    original.insert(3, "three");

    project::AVLTree<int, std::string> moved(std::move(original));

    REQUIRE(original.empty());
    REQUIRE(original.size() == 0);
    REQUIRE(moved.size() == 3);
    REQUIRE(moved.inorder() == std::vector<int>{1, 2, 3});
    REQUIRE(*moved.find(2) == "two");
}

TEST_CASE("Splay duplicate insertion updates without growing") {
    project::SplayTree<int, std::string> tree;
    tree.insert(10, "old");
    tree.insert(10, "new");

    REQUIRE(tree.size() == 1);
    REQUIRE(tree.inorder() == std::vector<int>{10});
    REQUIRE(*tree.find(10) == "new");
    REQUIRE(tree.rootKey() != nullptr);
    REQUIRE(*tree.rootKey() == 10);
}

TEST_CASE("Splay misses preserve contents and report null") {
    project::SplayTree<int, int> tree;
    for (int key : {10, 5, 15, 2, 7, 12, 20})
        tree.insert(key, key);

    REQUIRE_FALSE(tree.contains(99));
    REQUIRE(tree.find(99) == nullptr);
    REQUIRE(tree.size() == 7);
    REQUIRE(tree.inorder() == std::vector<int>{2, 5, 7, 10, 12, 15, 20});
}

TEST_CASE("Splay move assignment replaces the destination") {
    project::SplayTree<int, int> source;
    source.insert(8, 80);
    source.insert(3, 30);

    project::SplayTree<int, int> destination;
    destination.insert(99, 990);
    destination = std::move(source);

    REQUIRE(source.empty());
    REQUIRE(destination.size() == 2);
    REQUIRE(destination.inorder() == std::vector<int>{3, 8});
    REQUIRE(destination.rootKey() != nullptr);
}

TEST_CASE("AVL handles all four insertion rotations") {
    for (std::vector<int> keys : {std::vector<int>{3, 2, 1}, {1, 2, 3},
                                  {3, 1, 2}, {1, 3, 2}}) {
        project::AVLTree<int, int> tree;
        for (int key : keys) {
            tree.insert(key, key * 10);
            REQUIRE(tree.isValidAVL());
        }
        REQUIRE(tree.height() == 2);
        REQUIRE(tree.inorder() == std::vector<int>{1, 2, 3});
        for (int key : keys) {
            REQUIRE(*tree.find(key) == key * 10);
        }
    }
}

TEST_CASE("AVL rebalances throughout a shuffled deletion sequence") {
    project::AVLTree<int, int> tree;
    std::vector<int> keys;
    for (int key = 0; key < 512; ++key) {
        tree.insert(key, key + 1000);
        keys.push_back(key);
    }
    std::vector<int> remaining = keys;
    std::mt19937 random(42);
    std::shuffle(keys.begin(), keys.end(), random);
    for (int key : keys) {
        REQUIRE(tree.erase(key));
        remaining.erase(std::lower_bound(remaining.begin(), remaining.end(), key));
        REQUIRE(tree.isValidAVL());
        REQUIRE(tree.inorder() == remaining);
        REQUIRE(tree.size() == remaining.size());
        REQUIRE(tree.find(key) == nullptr);
        REQUIRE_FALSE(tree.erase(key));
        for (int survivor : remaining) {
            REQUIRE(*tree.find(survivor) == survivor + 1000);
        }
    }
    REQUIRE(tree.empty());
    REQUIRE(tree.height() == 0);
    tree.insert(1, 10);
    REQUIRE(tree.erase(1));
    REQUIRE(tree.isValidAVL());
}

TEST_CASE("AVL custom ordering survives updates and deletion") {
    project::AVLTree<int, int, std::greater<int>> tree;
    for (int key = 0; key < 100; ++key) {
        tree.insert(key, key);
    }
    tree.insert(50, -50);
    REQUIRE(tree.size() == 100);
    REQUIRE(*tree.find(50) == -50);
    for (int key = 0; key < 100; key += 2) {
        REQUIRE(tree.erase(key));
        REQUIRE(tree.isValidAVL());
    }
    const auto keys = tree.inorder();
    REQUIRE(std::is_sorted(keys.begin(), keys.end(), std::greater<int>{}));
    REQUIRE(keys.size() == 50);
}

TEST_CASE("Splay rotations preserve ordering across insertion permutations") {
    std::vector<int> keys{1, 2, 3, 4};
    do {
        project::SplayTree<int, int> tree;
        for (int key : keys) {
            tree.insert(key, key * 10);
            REQUIRE(*tree.rootKey() == key);
        }
        for (int key : {1, 4, 2, 3, 1, 3, 2, 4}) {
            REQUIRE(*tree.find(key) == key * 10);
            REQUIRE(*tree.rootKey() == key);
            REQUIRE(tree.inorder() == std::vector<int>{1, 2, 3, 4});
            REQUIRE(tree.depth() >= 3);
            REQUIRE(tree.depth() <= 4);
        }
        REQUIRE_FALSE(tree.contains(0));
        REQUIRE(*tree.rootKey() == 1);
        REQUIRE_FALSE(tree.contains(5));
        REQUIRE(*tree.rootKey() == 4);
    } while (std::next_permutation(keys.begin(), keys.end()));
}

TEST_CASE("Splay unsuccessful search splays the last accessed node") {
    project::SplayTree<int, int> tree;
    REQUIRE(tree.find(20) == nullptr);
    REQUIRE_FALSE(tree.erase(20));
    tree.insert(10, 100);
    tree.insert(30, 300);
    REQUIRE(tree.find(20) == nullptr);
    REQUIRE(*tree.rootKey() == 10);
    REQUIRE(tree.find(20) == nullptr);
    REQUIRE(*tree.rootKey() == 30);
    REQUIRE(tree.erase(30));
    REQUIRE(tree.depth() == 1);
    REQUIRE(tree.erase(10));
    REQUIRE(tree.rootKey() == nullptr);
    REQUIRE(tree.depth() == 0);
}

TEST_CASE("Splay mixed updates agree with an ordered vector") {
    project::SplayTree<int, int> tree;
    std::vector<std::pair<int, int>> expected;
    std::mt19937 random(1234);
    for (int step = 0; step < 2000; ++step) {
        const int key = static_cast<int>(random() % 200);
        const int operation = static_cast<int>(random() % 3);
        auto position = std::lower_bound(expected.begin(), expected.end(), key,
            [](const auto& entry, int target) { return entry.first < target; });
        const bool present = position != expected.end() && position->first == key;
        if (operation == 0) {
            tree.insert(key, step);
            if (present) {
                position->second = step;
            } else {
                expected.insert(position, {key, step});
            }
            REQUIRE(*tree.rootKey() == key);
        } else if (operation == 1) {
            REQUIRE(tree.erase(key) == present);
            if (present) {
                expected.erase(position);
            }
        } else {
            const int* value = tree.find(key);
            if (present) {
                REQUIRE(value != nullptr);
                REQUIRE(*value == position->second);
                REQUIRE(*tree.rootKey() == key);
            } else {
                REQUIRE(value == nullptr);
            }
        }
        std::vector<int> keys;
        for (const auto& entry : expected) {
            keys.push_back(entry.first);
        }
        REQUIRE(tree.size() == expected.size());
        REQUIRE(tree.inorder() == keys);
    }
}

TEST_CASE("Splay custom ordering works for search and subtree joins") {
    project::SplayTree<int, int, std::greater<int>> tree;
    for (int key : {5, 2, 8, 1, 3, 7, 9}) {
        tree.insert(key, key * 10);
    }
    REQUIRE(tree.inorder() == std::vector<int>{9, 8, 7, 5, 3, 2, 1});
    REQUIRE(*tree.find(5) == 50);
    REQUIRE(*tree.rootKey() == 5);
    REQUIRE(tree.erase(5));
    REQUIRE(tree.inorder() == std::vector<int>{9, 8, 7, 3, 2, 1});
    for (int key : {9, 1, 7, 2, 8, 3}) {
        REQUIRE(tree.erase(key));
    }
    REQUIRE(tree.empty());
}

TEST_CASE("Long splay chains support depth, traversal, access, and destruction") {
    project::SplayTree<int, int> tree;
    std::vector<int> expected;
    for (int key = 0; key < 50000; ++key) {
        tree.insert(key, key);
        expected.push_back(key);
    }
    REQUIRE(tree.size() == expected.size());
    REQUIRE(tree.depth() == expected.size());
    REQUIRE(tree.inorder() == expected);
    REQUIRE(tree.contains(0));
    REQUIRE(*tree.rootKey() == 0);
    REQUIRE(tree.inorder() == expected);
    REQUIRE(tree.erase(0));
    REQUIRE(tree.erase(49999));
    REQUIRE(tree.size() == 49998);
}

namespace {
template <typename Tree>
void checkMovesAndOwnership() {
    std::weak_ptr<int> transferredValue;
    std::weak_ptr<int> replacedValue;
    {
        Tree source;
        auto value = std::make_shared<int>(10);
        transferredValue = value;
        source.insert(1, value);
        value.reset();
        Tree moved(std::move(source));
        REQUIRE(source.empty());
        REQUIRE(source.inorder().empty());
        REQUIRE(source.find(1) == nullptr);
        source.insert(2, std::make_shared<int>(20));
        REQUIRE(source.size() == 1);

        Tree destination;
        value = std::make_shared<int>(99);
        replacedValue = value;
        destination.insert(99, value);
        value.reset();
        destination = std::move(moved);
        REQUIRE(replacedValue.expired());
        REQUIRE(moved.empty());
        REQUIRE(destination.size() == 1);
        REQUIRE(**destination.find(1) == 10);
        auto& sameTree = destination;
        destination = std::move(sameTree);
        REQUIRE(destination.size() == 1);
        REQUIRE(destination.erase(1));
        REQUIRE(transferredValue.expired());
        destination.insert(3, std::make_shared<int>(30));
    }
    REQUIRE(transferredValue.expired());
}
}

TEST_CASE("Both trees transfer ownership safely and permit moved-from reuse") {
    checkMovesAndOwnership<project::AVLTree<int, std::shared_ptr<int>>>();
    checkMovesAndOwnership<project::SplayTree<int, std::shared_ptr<int>>>();
}
