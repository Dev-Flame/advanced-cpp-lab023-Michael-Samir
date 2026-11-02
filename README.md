# Lab 23 — AVL Trees; Splay Trees

## Topic

**7.4 Module 7 — AVL Trees; Splay Trees**

## Overview

Implement two self-adjusting/balancing ordered search structures:

1. An **AVL tree**, maintaining the height-balance property through local trinode restructurings (rotations).
2. A **splay tree**, restructuring the tree after accesses using zig, zig-zig, and zig-zag rotations.

The supplied interface and tests define the required behavior. The core algorithms in `BalancedTrees.tpp` are intentionally incomplete.

## Learning Objectives

By completing this lab, you should be able to:

- Implement a binary-search-tree map with AVL balancing.
- Maintain node heights and balance factors.
- Implement LL, RR, LR, and RL rotations.
- Rebalance after insertion and deletion.
- Verify AVL invariants programmatically.
- Implement parent-aware tree rotations.
- Implement splay operations.
- Understand the difference between worst-case and amortized complexity.
- Compare strict height balancing with access-based self-adjustment.

## Requirements

### Part A — AVL Tree

Implement:

- `insert`
- `erase`
- `find`
- `contains`
- height maintenance
- rotations
- rebalancing
- AVL validation
- correct ownership/destruction
- move construction and move assignment

Duplicate insertion must update the existing value without increasing the size.

### Part B — Splay Tree

Implement:

- BST insertion
- search/access
- erase
- left and right rotations
- `splay`
- zig
- zig-zig
- zig-zag
- parent-pointer maintenance
- subtree joining after deletion
- move construction and move assignment

Successful and unsuccessful searches should splay the appropriate accessed/last-accessed node according to the implementation strategy required by the interface.

## Constraints

- C++17 or later.
- Do not use `std::map`, `std::set`, or other tree containers to implement the structures.
- Do not use a library heap/tree implementation.
- Each tree must own and destroy its dynamically allocated nodes.
- Maintain the BST ordering invariant.
- AVL rotations must be local operations.
- Splay rotations must correctly maintain parent pointers.

## Testing

Run:

```text
make
make run
make test
```

The visible tests are not exhaustive. Additional automated tests may inspect:

- all four AVL rotation cases
- deletion requiring multiple rebalances
- duplicate updates
- empty and singleton trees
- move semantics
- parent-pointer correctness
- zig / zig-zig / zig-zag behavior
- repeated accesses
- unsuccessful searches
- deletion of root, leaf, and internal nodes
- large ordered insertion sequences

## Complexity

An AVL tree has `O(log n)` height, so `find`, `insert`, and `erase` run in `O(log n)`. The course text summarizes this performance explicitly. 

A splay tree does not guarantee `O(log n)` for every individual operation; its important guarantee is **`O(log n)` amortized cost** over a sequence of operations.

See `docs/analysis.md` for the required written analysis.
