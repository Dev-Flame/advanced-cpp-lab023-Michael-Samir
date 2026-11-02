# Lab 23 Analysis — AVL Trees and Splay Trees

## Required Analysis

1. Explain why an AVL tree maintains `O(log n)` height.
2. Identify the four AVL imbalance cases:
   - LL
   - RR
   - LR
   - RL
3. Explain how insertion and deletion differ in their rebalancing behavior.
4. Explain why each AVL `find`, `insert`, and `erase` operation is `O(log n)`.
5. Explain the purpose of a splay operation.
6. Describe:
   - zig
   - zig-zig
   - zig-zag
7. Explain why a single splay operation can take `O(n)` in the worst case even though splay trees provide logarithmic amortized performance.
8. Compare AVL and splay trees for:
   - guaranteed height
   - amortized access cost
   - repeated access to hot keys
   - update overhead
   - memory metadata
9. Construct an access sequence where a splay tree benefits substantially from locality of reference.
10. Explain why a splay tree cannot be described as having `O(log n)` worst-case time for every individual operation.

## Expected Complexity

AVL:
- search: O(log n)
- insertion: O(log n)
- deletion: O(log n)
- space: O(n)

Splay:
- individual operation: O(n) worst case
- amortized operation: O(log n)
- space: O(n)
