#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "project/BalancedTrees.hpp"

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
