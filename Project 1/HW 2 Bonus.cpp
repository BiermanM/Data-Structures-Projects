/*
    Assignment: Homework #2: BONUS
    Author:     Matthew Bierman
    Course:     CS 3345.502 (Data Structures and Introduction to Algorithmic Analysis)
    Instructor: Professor Kamran Khan
    Date Due:   September 17, 2017 at 11:30pm
*/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <ctime>

using namespace std;

class Book {
public:
    int isbn;
    string title;

    Book() {
        isbn = 0;
        title = "";
    }

    Book(int i) {
        isbn = i;
    }

    Book (int i, string str) {
        isbn = i;
        title = str;
    }
};

class AVLNode {
public:
    int key;
    Book value;
    int height;
    AVLNode * leftPtr;
    AVLNode * rightPtr;

    AVLNode() {
        key = 0;
        value = Book(key);   // random number between 0 and 100
        height = 0;
        leftPtr = nullptr;
        rightPtr = nullptr;
    }

    AVLNode(int i) {
        key = i;
        value = Book(key);   // random number between 0 and 100
        height = 0;
        leftPtr = nullptr;
        rightPtr = nullptr;
    }
};

class AVLTree {
public:
    AVLNode * root;
    AVLTree() {
        root = nullptr;
    }

    void insert(AVLNode * n, int num) {
        // if tree is empty, set node as root
        if (root == nullptr)
            root = new AVLNode(num);
        else {
            // for each insertion, randomly choose whether to insert on left or right subtree
            // true for left subtree, false for right subtree
            bool lOrR = rand() % 2 == 1;

            // left subtree
            if (lOrR == true) {
                if (n->leftPtr == nullptr)
                    n->leftPtr = new AVLNode(num);
                else
                    insert(n->leftPtr, num);
            }
            // right subtree
            else {
                if (n->rightPtr == nullptr)
                    n->rightPtr = new AVLNode(num);
                else
                    insert(n->rightPtr, num);
            }
        }
    }

    void printTree(AVLNode * n) {
        if (n != nullptr) {
            cout << "Node " << n->key << " -- Left Child: ";
            if (n->leftPtr != nullptr)
                cout << n->leftPtr->key;
            else
                cout << "NULL";
            cout << ", Right Child: ";
            if (n->rightPtr != nullptr)
                cout << n->rightPtr->key;
            else
                cout << "NULL";
            cout << endl;

            printTree(n->leftPtr);
            printTree(n->rightPtr);
        }
    }

    bool isProper(AVLNode * n) {
        // if node is a leaf, then it follows all properties
        if (n->leftPtr == nullptr && n->rightPtr == nullptr) {
            // set correct height for a leaf
            n->height = 0;
            return true;
        }

        // check left and right children recursively for if their subtrees are proper
        bool leftSub = true;
        bool rightSub = true;
        if (n->leftPtr != nullptr) {
            // doesn't follow BST property if left child is larger than parent
            if (n->leftPtr->key > n->key)
                return false;

            leftSub = isProper(n->leftPtr);
        }
        if (n->rightPtr != nullptr) {
            // doesn't follow BST property if right child is smaller than parent
            if (n->rightPtr->key < n->key)
                return false;

            rightSub = isProper(n->rightPtr);
        }

        // set correct height by taking the height of the tallest child's subtree and adding one to it
        if (n->leftPtr != nullptr && n->rightPtr != nullptr) {
            if (n->leftPtr->height > n->rightPtr->height)
                n->height = n->leftPtr->height + 1;
            else
                n->height = n->rightPtr->height + 1;
        }
        // if only one child exists, no need to compare between children
        else if (n->leftPtr != nullptr)
            n->height = n->leftPtr->height + 1;
        else if (n->rightPtr != nullptr)
            n->height = n->rightPtr->height + 1;

        // check if subtree with current node at root follows AVL Balance Condition
        int lHeight = -1;
        int rHeight = -1;
        if (n->leftPtr != nullptr)
            lHeight = n->leftPtr->height;
        if (n->rightPtr != nullptr)
            rHeight = n->rightPtr->height;

        if ( abs(lHeight - rHeight) > 1 )
            return false;

        if (leftSub == true && rightSub == true)
            return true;
        else
            return false;
    }
};

int main() {
    // set seed for rand()
    srand(time(0));

    AVLTree a = AVLTree();

    // randomly select the number of nodes to insert, between 1 and 10 nodes
    int numNodes = rand() % 10 + 1;
    for (int x = 0; x < numNodes; x++) {
        // insert a node with a random ISBN number
        a.insert(a.root, rand());
    }

    a.printTree(a.root);

    if (a.isProper(a.root) == true)
        cout << "It is a proper AVL Tree!" << endl;
    else
        cout << "It is not a proper AVL Tree." << endl;

    return 0;
}
