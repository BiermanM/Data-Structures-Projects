/*
    Assignment: Homework #2
    Author:     Matthew Bierman
    Course:     CS 3345.502 (Data Structures and Introduction to Algorithmic Analysis)
    Instructor: Professor Kamran Khan
    Date Due:   September 24, 2017 at 11:30pm
*/

#include <iostream>
#include <fstream>
#include <string>

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

    AVLNode * insert(AVLNode * n, int num) {
        AVLNode * newRoot = n;

        // if tree is empty, set new node to root
        if (root == nullptr)
            return new AVLNode(num);
        else {
            // if new node is less than the root of the current subtree
            if (num < n->key) {
                if (n->leftPtr == nullptr)
                    n->leftPtr = new AVLNode(num);
                else
                    n->leftPtr = insert(n->leftPtr, num);
            }
            // if new node is greater than or equal to the root of the current subtree
            else {
                if (n->rightPtr == nullptr)
                    n->rightPtr = new AVLNode(num);
                else
                    n->rightPtr = insert(n->rightPtr, num);
            }

            // set the correct height of the current node
            getNewHeight(n);

            // Rotations
            // if the left subtree's height is greater than the right subtree's height by 2 or more
            if ((getSubtreeHeight(n, "left") - getSubtreeHeight(n, "right")) > 1) {
                // LeftRight Rotation
                if ((getSubtreeHeight(n->leftPtr, "left") - getSubtreeHeight(n->leftPtr, "right")) <= -1) {
                    cout << "Imbalance occurred at inserting ISBN " << n->key << "; fixed in LeftRight Rotation." << endl;
                    n->leftPtr =  rotateLeft(n->leftPtr);
                    newRoot = rotateRight(n);

                    // update heights after rotation
                    getNewHeight(newRoot->rightPtr);
                    getNewHeight(newRoot->leftPtr);
                    getNewHeight(newRoot);
                }
                // Right Rotation
                else {
                    cout << "Imbalance occurred at inserting ISBN " << n->key << "; fixed in Right Rotation." << endl;
                    newRoot = rotateRight(n);

                    // update heights after rotation
                    getNewHeight(newRoot->rightPtr);
                    getNewHeight(newRoot);
                }
            }
            // if the right subtree's height is greater than the left subtree's height by 2 or more
            else if ((getSubtreeHeight(n, "left") - getSubtreeHeight(n, "right")) < -1) {
                // RightLeft Rotation
                if ((getSubtreeHeight(n->rightPtr, "left") - getSubtreeHeight(n->rightPtr, "right")) >= 1) {
                    cout << "Imbalance occurred at inserting ISBN " << n->key << "; fixed in RightLeft Rotation." << endl;
                    n->rightPtr = rotateRight(n->rightPtr);
                    newRoot = rotateLeft(n);

                    // update heights after rotation
                    getNewHeight(newRoot->rightPtr);
                    getNewHeight(newRoot->leftPtr);
                    getNewHeight(newRoot);
                }
                // Left Rotation
                else {
                    cout << "Imbalance occurred at inserting ISBN " << n->key << "; fixed in Left Rotation." << endl;
                    newRoot = rotateLeft(n);
                    getNewHeight(newRoot);

                    // update heights after rotation
                    getNewHeight(newRoot->leftPtr);
                    getNewHeight(newRoot);
                }
            }

            return newRoot;
        }
    }

    // set height of node by looking at heights of left and right subtrees
    void getNewHeight(AVLNode * n) {
        int lHeight = getSubtreeHeight(n, "left");
        int rHeight = getSubtreeHeight(n, "right");

        if (lHeight > rHeight)
            n->height = lHeight + 1;
        else
            n->height = rHeight + 1;
    }

    // get height of left or right subtree of node n
    int getSubtreeHeight(AVLNode * n, string lOrR) {
        int h = -1;
        if (lOrR == "left" && n->leftPtr != nullptr)
            h = n->leftPtr->height;
        else if (lOrR == "right" && n->rightPtr != nullptr)
            h = n->rightPtr->height;

        return h;
    }

    // when height of right subtree is greater than left subtree by 2 or more
    AVLNode * rotateLeft(AVLNode * n) {
        AVLNode * newRoot = n->rightPtr;
        n->rightPtr = newRoot->leftPtr;
        newRoot->leftPtr = n;

        return newRoot;
    }

    // when height of left subtree is greater than right subtree by 2 or more
    AVLNode * rotateRight(AVLNode * n) {
        AVLNode * newRoot = n->leftPtr;
        n->leftPtr = newRoot->rightPtr;
        newRoot->rightPtr = n;

        return newRoot;
    }
};

int main()
{
    AVLTree a = AVLTree();

    // input from file data.txt
    // input should be only one line, with each integer separated by a space
    // for example: "4 2 6 1 3 18"
    ifstream inFile("data.txt");
    if (inFile) {
        int num;

        // read each integer until it reaches the end of the file
        while (inFile.eof() == false) {
            inFile >> num;

            // once the integer is read, insert into the AVL Tree
            cout << num << " is being inserted." << endl;
            a.root = a.insert(a.root, num);
        }
    }
    inFile.close();

    return 0;
}
