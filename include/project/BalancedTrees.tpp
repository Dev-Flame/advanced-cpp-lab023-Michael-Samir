#include <algorithm>

namespace project {

// ========================= AVLTree =========================

template <typename Key, typename Value, typename Compare>
int AVLTree<Key, Value, Compare>::nodeHeight(const Node* n) {
    return n ? n->height : 0;
}

template <typename Key, typename Value, typename Compare>
int AVLTree<Key, Value, Compare>::balanceFactor(const Node* n) {
    return n ? nodeHeight(n->left) - nodeHeight(n->right) : 0;
}

template <typename Key, typename Value, typename Compare>
void AVLTree<Key, Value, Compare>::destroy(Node* n) {
    if (n) {
        destroy(n->left);
        destroy(n->right);
        delete n;
    }
}

template <typename Key, typename Value, typename Compare>
AVLTree<Key, Value, Compare>::~AVLTree() {
    destroy(root_);
}

template <typename Key, typename Value, typename Compare>
AVLTree<Key, Value, Compare>::AVLTree(AVLTree&& other) noexcept
    : root_(other.root_), size_(other.size_), comp_(std::move(other.comp_)) {
    other.root_ = nullptr;
    other.size_ = 0;
}

template <typename Key, typename Value, typename Compare>
AVLTree<Key, Value, Compare>&
AVLTree<Key, Value, Compare>::operator=(AVLTree&& other) noexcept {
    if (this != &other) {
        destroy(root_);
        root_ = other.root_;
        size_ = other.size_;
        comp_ = std::move(other.comp_);
        other.root_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename Key, typename Value, typename Compare>
bool AVLTree<Key, Value, Compare>::empty() const noexcept {
    return size_ == 0;
}

template <typename Key, typename Value, typename Compare>
std::size_t AVLTree<Key, Value, Compare>::size() const noexcept {
    return size_;
}

template <typename Key, typename Value, typename Compare>
int AVLTree<Key, Value, Compare>::height() const noexcept {
    return nodeHeight(root_);
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    x->height = 1 + std::max(nodeHeight(x->left), nodeHeight(x->right));
    y->height = 1 + std::max(nodeHeight(y->left), nodeHeight(y->right));
    return y;
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    y->height = 1 + std::max(nodeHeight(y->left), nodeHeight(y->right));
    x->height = 1 + std::max(nodeHeight(x->left), nodeHeight(x->right));
    return x;
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::rebalance(Node* n) {
    if (!n) {
        return nullptr;
    }
    n->height = 1 + std::max(nodeHeight(n->left), nodeHeight(n->right));
    const int balance = balanceFactor(n);
    if (balance > 1) {
        if (balanceFactor(n->left) < 0) {
            n->left = rotateLeft(n->left);  // LR: first make it an LL case.
        }
        return rotateRight(n);
    }
    if (balance < -1) {
        if (balanceFactor(n->right) > 0) {
            n->right = rotateRight(n->right);  // RL: first make it an RR case.
        }
        return rotateLeft(n);
    }
    return n;
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::insert(Node* n, const Key& key,
                                     const Value& value, bool& inserted) {
    if (!n) {
        Node* node = new Node(key, value);
        inserted = true;
        return node;
    }
    if (comp_(key, n->key)) {
        n->left = insert(n->left, key, value, inserted);
    } else if (comp_(n->key, key)) {
        n->right = insert(n->right, key, value, inserted);
    } else {
        n->value = value;
        return n;
    }
    return rebalance(n);
}

template <typename Key, typename Value, typename Compare>
void AVLTree<Key, Value, Compare>::insert(const Key& key, const Value& value) {
    bool inserted = false;
    root_ = insert(root_, key, value, inserted);
    if (inserted) {
        ++size_;
    }
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::minimum(Node* n) {
    while (n && n->left) {
        n = n->left;
    }
    return n;
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::erase(Node* n, const Key& key, bool& removed) {
    if (!n) {
        return nullptr;
    }
    if (comp_(key, n->key)) {
        n->left = erase(n->left, key, removed);
    } else if (comp_(n->key, key)) {
        n->right = erase(n->right, key, removed);
    } else {
        removed = true;
        if (!n->left || !n->right) {
            Node* child = n->left ? n->left : n->right;
            delete n;
            return child;
        }
        // Replace a two-child node with its inorder successor.
        Node* successor = minimum(n->right);
        n->key = successor->key;
        n->value = successor->value;
        n->right = erase(n->right, n->key, removed);
    }
    // Deletion can require rebalancing at multiple ancestors.
    return rebalance(n);
}

template <typename Key, typename Value, typename Compare>
bool AVLTree<Key, Value, Compare>::erase(const Key& key) {
    bool removed = false;
    root_ = erase(root_, key, removed);
    if (removed) {
        --size_;
    }
    return removed;
}

template <typename Key, typename Value, typename Compare>
const typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::findNode(const Node* n, const Key& key) const {
    while (n) {
        if (comp_(key, n->key)) {
            n = n->left;
        } else if (comp_(n->key, key)) {
            n = n->right;
        } else {
            return n;
        }
    }
    return nullptr;
}

template <typename Key, typename Value, typename Compare>
bool AVLTree<Key, Value, Compare>::contains(const Key& key) const {
    return findNode(root_, key) != nullptr;
}

template <typename Key, typename Value, typename Compare>
const Value* AVLTree<Key, Value, Compare>::find(const Key& key) const {
    const Node* n = findNode(root_, key);
    return n ? &n->value : nullptr;
}

template <typename Key, typename Value, typename Compare>
void AVLTree<Key, Value, Compare>::inorder(const Node* n,
                                           std::vector<Key>& out) {
    if (n) {
        inorder(n->left, out);
        out.push_back(n->key);
        inorder(n->right, out);
    }
}

template <typename Key, typename Value, typename Compare>
std::vector<Key> AVLTree<Key, Value, Compare>::inorder() const {
    std::vector<Key> out;
    inorder(root_, out);
    return out;
}

template <typename Key, typename Value, typename Compare>
bool AVLTree<Key, Value, Compare>::validate(const Node* n, const Key* lo,
                                            const Key* hi, int& computedHeight) const {
    if (!n) {
        computedHeight = 0;
        return true;
    }
    if ((lo && !comp_(*lo, n->key)) || (hi && !comp_(n->key, *hi))) {
        return false;
    }
    int leftHeight = 0;
    int rightHeight = 0;
    if (!validate(n->left, lo, &n->key, leftHeight) ||
        !validate(n->right, &n->key, hi, rightHeight)) {
        return false;
    }
    computedHeight = 1 + std::max(leftHeight, rightHeight);
    const int balance = leftHeight - rightHeight;
    return n->height == computedHeight && balance >= -1 && balance <= 1;
}

template <typename Key, typename Value, typename Compare>
bool AVLTree<Key, Value, Compare>::isValidAVL() const {
    int h = 0;
    return validate(root_, nullptr, nullptr, h);
}

// ========================= SplayTree =========================

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::destroy(Node* n) {
    // A splay tree can be a chain. Flatten left links while walking so cleanup
    // uses constant extra space and cannot exhaust the call stack.
    while (n) {
        if (n->left) {
            Node* left = n->left;
            n->left = left->right;
            left->right = n;
            n = left;
        } else {
            Node* next = n->right;
            delete n;
            n = next;
        }
    }
}

template <typename Key, typename Value, typename Compare>
SplayTree<Key, Value, Compare>::~SplayTree() {
    destroy(root_);
}

template <typename Key, typename Value, typename Compare>
SplayTree<Key, Value, Compare>::SplayTree(SplayTree&& other) noexcept
    : root_(other.root_), size_(other.size_), comp_(std::move(other.comp_)) {
    other.root_ = nullptr;
    other.size_ = 0;
}

template <typename Key, typename Value, typename Compare>
SplayTree<Key, Value, Compare>&
SplayTree<Key, Value, Compare>::operator=(SplayTree&& other) noexcept {
    if (this != &other) {
        destroy(root_);
        root_ = other.root_;
        size_ = other.size_;
        comp_ = std::move(other.comp_);
        other.root_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename Key, typename Value, typename Compare>
bool SplayTree<Key, Value, Compare>::empty() const noexcept {
    return size_ == 0;
}

template <typename Key, typename Value, typename Compare>
std::size_t SplayTree<Key, Value, Compare>::size() const noexcept {
    return size_;
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (x->right) {
        x->right->parent = x;
    }
    transplant(x, y);
    y->left = x;
    x->parent = y;
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (x->left) {
        x->left->parent = x;
    }
    transplant(x, y);
    y->right = x;
    x->parent = y;
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::splay(Node* x) {
    while (x && x->parent) {
        Node* parent = x->parent;
        Node* grandparent = parent->parent;
        if (!grandparent) {  // Zig: x is a child of the root.
            if (x == parent->left) {
                rotateRight(parent);
            } else {
                rotateLeft(parent);
            }
        } else if (x == parent->left && parent == grandparent->left) {
            rotateRight(grandparent);  // Left-left zig-zig.
            rotateRight(parent);
        } else if (x == parent->right && parent == grandparent->right) {
            rotateLeft(grandparent);  // Right-right zig-zig.
            rotateLeft(parent);
        } else if (x == parent->right && parent == grandparent->left) {
            rotateLeft(parent);  // Left-right zig-zag.
            rotateRight(grandparent);
        } else {
            rotateRight(parent);  // Right-left zig-zag.
            rotateLeft(grandparent);
        }
    }
}

template <typename Key, typename Value, typename Compare>
typename SplayTree<Key, Value, Compare>::Node*
SplayTree<Key, Value, Compare>::findNode(const Key& key) {
    Node* current = root_;
    Node* last = nullptr;
    while (current) {
        last = current;
        if (comp_(key, current->key)) {
            current = current->left;
        } else if (comp_(current->key, key)) {
            current = current->right;
        } else {
            splay(current);
            return current;
        }
    }
    splay(last);
    return nullptr;
}

template <typename Key, typename Value, typename Compare>
bool SplayTree<Key, Value, Compare>::contains(const Key& key) {
    return findNode(key) != nullptr;
}

template <typename Key, typename Value, typename Compare>
const Value* SplayTree<Key, Value, Compare>::find(const Key& key) {
    Node* n = findNode(key);
    return n ? &n->value : nullptr;
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::insert(const Key& key,
                                            const Value& value) {
    Node** link = &root_;
    Node* parent = nullptr;
    while (*link) {
        Node* current = *link;
        parent = current;
        if (comp_(key, current->key)) {
            link = &current->left;
        } else if (comp_(current->key, key)) {
            link = &current->right;
        } else {
            current->value = value;
            splay(current);
            return;
        }
    }
    Node* node = new Node(key, value);
    node->parent = parent;
    *link = node;
    ++size_;
    splay(node);
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::transplant(Node* u, Node* v) {
    if (!u->parent) {
        root_ = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) {
        v->parent = u->parent;
    }
}

template <typename Key, typename Value, typename Compare>
typename SplayTree<Key, Value, Compare>::Node*
SplayTree<Key, Value, Compare>::subtreeMinimum(Node* n) {
    while (n && n->left) {
        n = n->left;
    }
    return n;
}

template <typename Key, typename Value, typename Compare>
bool SplayTree<Key, Value, Compare>::erase(const Key& key) {
    Node* target = findNode(key);
    if (!target) {
        return false;
    }
    Node* left = target->left;
    Node* right = target->right;
    if (left) {
        left->parent = nullptr;
    }
    if (right) {
        right->parent = nullptr;
    }
    delete target;
    --size_;
    if (!right) {
        root_ = left;
    } else {
        root_ = right;
        // Splaying the right subtree's minimum leaves its left link empty.
        splay(subtreeMinimum(right));
        root_->left = left;
        if (left) {
            left->parent = root_;
        }
    }
    return true;
}

template <typename Key, typename Value, typename Compare>
const Key* SplayTree<Key, Value, Compare>::rootKey() const noexcept {
    return root_ ? &root_->key : nullptr;
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::inorder(const Node* n,
                                             std::vector<Key>& out) {
    // An explicit stack handles a potentially linear-height tree safely.
    std::vector<const Node*> stack;
    while (n || !stack.empty()) {
        while (n) {
            stack.push_back(n);
            n = n->left;
        }
        n = stack.back();
        stack.pop_back();
        out.push_back(n->key);
        n = n->right;
    }
}

template <typename Key, typename Value, typename Compare>
std::vector<Key> SplayTree<Key, Value, Compare>::inorder() const {
    std::vector<Key> out;
    inorder(root_, out);
    return out;
}

template <typename Key, typename Value, typename Compare>
std::size_t SplayTree<Key, Value, Compare>::depth(const Node* n) {
    if (!n) {
        return 0;
    }
    // Parent pointers allow a depth-first walk without allocation or recursion,
    // preserving the public depth() method's noexcept contract.
    const Node* boundary = n->parent;
    const Node* previous = boundary;
    std::size_t currentDepth = 1;
    std::size_t maximumDepth = 0;
    while (n != boundary) {
        maximumDepth = std::max(maximumDepth, currentDepth);
        const Node* next;
        if (previous == n->parent && n->left) {
            next = n->left;
            ++currentDepth;
        } else if ((previous == n->parent || previous == n->left) && n->right) {
            next = n->right;
            ++currentDepth;
        } else {
            next = n->parent;
            --currentDepth;
        }
        previous = n;
        n = next;
    }
    return maximumDepth;
}

template <typename Key, typename Value, typename Compare>
std::size_t SplayTree<Key, Value, Compare>::depth() const noexcept {
    return depth(root_);
}

} // namespace project
