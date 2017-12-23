# Project 1 Instructions

## Details
* Always Verify Time!
* Write code for `O(n)` worst-case algorithm that verifies that a tree is actually an AVL tree by detecting imbalance. If imbalance is true, then call the proper rotation function provided in the lecture slides to fix the imbalance condition.
  1. Must read AVLNode data from a text file
  2. Create a Book Object; and an AVL node object to be inserted into the AVL tree
  3. At each insert, detect imbalance and fix the AVL tree
  4. Report each imbalance detection and the node where it occurred; and output the message:
      ```
      Imbalance occurred at inserting ISBN 12345; fixed in LeftRight Rotation
      Imbalance occurred at inserting ISBN 87654; fixed in Left Rotation
      Imbalance occurred at inserting ISBN 974321; fixed in RightLeft Rotation
      ```

You must verify the AVL balance condition at each insert and detect and fix imbalance, output result of each insertion. A null node is considered AVL property of height -1.

```
class AVLNode {
  int key; (ISBN number)
  Book value; //create a class representing a book with minimum attributes int height;
  AVLNode leftPtr;
  AVLNode rightPtr;
}
```

## Bonus (20%)
Create a random binary tree and verify BST property and AVL balance condition. Do not hard code the tree inside the code.
