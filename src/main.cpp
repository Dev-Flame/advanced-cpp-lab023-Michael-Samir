#include "project/BalancedTrees.hpp"
#include <iostream>

int main() {
    project::AVLTree<int, std::string> avl;
    project::SplayTree<int, std::string> splay;

    for (int k : {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45}) {
        avl.insert(k, "value-" + std::to_string(k));
        splay.insert(k, "value-" + std::to_string(k));
    }

    std::cout << "AVL valid: " << std::boolalpha << avl.isValidAVL() << "\n";
    std::cout << "AVL height: " << avl.height() << "\n";

    splay.contains(10);
    if (const int* root = splay.rootKey()) {
        std::cout << "Splay root after searching for 10: " << *root << "\n";
    }

    std::cout << "AVL inorder: ";
    for (int k : avl.inorder()) std::cout << k << ' ';
    std::cout << "\n";

    std::cout << "Splay inorder: ";
    for (int k : splay.inorder()) std::cout << k << ' ';
    std::cout << "\n";

    return 0;
}
