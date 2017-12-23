# Project 2 Instructions

## Part 1
Create an optional small GUI program using javax.swing components or have a console output if
you prefer not to create GUI.

## Part 2
A text file will be provided for the database of English words, their type (noun, verb, etc.) and their meaning. Create an efficient hash function to convert each word into an int Key.

__Note__: The text file will containe single words or multiple words connected via underscore, the type of the word (noun, verb, etc) and its meaning. So if we search for Data Structure, we will find Data_Structure which will be valid result to return.

## Part 3
Implement _Linear Probing_, _Quadtratic Probing_, _Separate Chaining_, _Double Hashing_ techniques discussed in class. Choose an appropriate Lamda value per implementation. Use Key generation in part 2 to store the node (key, type, meaning) in the hash table.

## Part 4
Once a word is searched, output its meaning and provide the follow information related to each specific implementation in the ouput:
* Data structure
* Table size
* Lambda
* Success
* Items investigated
