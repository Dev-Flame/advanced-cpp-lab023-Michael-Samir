#pragma once

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace project {

template <typename Key, typename Value, typename Compare = std::less<Key>>
class AVLTree {
public:
    AVLTree() = default;
    ~AVLTree();

    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree(AVLTree&& other) noexcept;
    AVLTree& operator=(AVLTree&& other) noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    bool contains(const Key& key) const;
    const Value* find(const Key& key) const;

    void insert(const Key& key, const Value& value);
    bool erase(const Key& key);

    int height() const noexcept;
    std::vector<Key> inorder() const;
    bool isValidAVL() const;

private:
    struct Node {
        Key key;
        Value value;
        Node* left{nullptr};
        Node* right{nullptr};
        int height{1};

        Node(const Key& k, const Value& v) : key(k), value(v) {}
    };

    Node* root_{nullptr};
    std::size_t size_{0};
    Compare comp_{};

    static int nodeHeight(const Node* n);
    static int balanceFactor(const Node* n);
    static void destroy(Node* n);

    static Node* rotateLeft(Node* x);
    static Node* rotateRight(Node* y);
    static Node* rebalance(Node* n);

    Node* insert(Node* n, const Key& key, const Value& value, bool& inserted);
    Node* erase(Node* n, const Key& key, bool& removed);
    const Node* findNode(const Node* n, const Key& key) const;
    static Node* minimum(Node* n);
    static void inorder(const Node* n, std::vector<Key>& out);
    static bool validate(const Node* n, const Key* lo, const Key* hi,
                         int& computedHeight);
};

template <typename Key, typename Value, typename Compare = std::less<Key>>
class SplayTree {
public:
    SplayTree() = default;
    ~SplayTree();

    SplayTree(const SplayTree&) = delete;
    SplayTree& operator=(const SplayTree&) = delete;
    SplayTree(SplayTree&& other) noexcept;
    SplayTree& operator=(SplayTree&& other) noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    bool contains(const Key& key);
    const Value* find(const Key& key);
    void insert(const Key& key, const Value& value);
    bool erase(const Key& key);

    // Exposes the root key so students can observe the effect of splaying.
    const Key* rootKey() const noexcept;

    std::vector<Key> inorder() const;
    std::size_t depth() const noexcept;

private:
    struct Node {
        Key key;
        Value value;
        Node* left{nullptr};
        Node* right{nullptr};
        Node* parent{nullptr};

        Node(const Key& k, const Value& v) : key(k), value(v) {}
    };

    Node* root_{nullptr};
    std::size_t size_{0};
    Compare comp_{};

    static void destroy(Node* n);
    void rotateLeft(Node* x);
    void rotateRight(Node* x);
    void splay(Node* x);
    Node* findNode(const Key& key);
    static Node* subtreeMinimum(Node* n);
    void transplant(Node* u, Node* v);
    static void inorder(const Node* n, std::vector<Key>& out);
    static std::size_t depth(const Node* n);
};

} // namespace project

#include "project/BalancedTrees.tpp"
