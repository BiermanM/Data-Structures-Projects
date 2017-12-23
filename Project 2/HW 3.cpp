/*
    Assignment: Homework #3
    Author:     Matthew Bierman
    Course:     CS 3345.502 (Data Structures and Introduction to Algorithmic Analysis)
    Instructor: Professor Kamran Khan
    Date Due:   October 8, 2017 at 11:30pm
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

class Word {
public:
    string name, type, definition;
    unsigned int hashCode;

    // constructors
    Word() {
        name = "";
        type = "";
        definition = "";
        hashCode = 0;
    }
    Word(string n) {
        name = convertToProper(n);
        type = "";
        definition = "";
        hashCode = hashFunc();
    }
    Word(string n, string t, string d) {
        name = n;
        type = t;
        definition = d;
        hashCode = hashFunc();
    }

    // replace all spaces with underscores and convert all uppercase to lowercase
    string convertToProper(string str) {
        string newStr = "";
        for (int i = 0; i < str.length(); i++) {
            if (str.at(i) == ' ')
                newStr += '_';
            else if (str.at(i) >= 65 && str.at(i) <= 90)
                newStr += (str.at(i) + 32);
            else
                newStr += str.at(i);
        }
        return newStr;
    }

    // get hash for word
    unsigned int hashFunc() {
        unsigned int hash = 0;
        for (int i = 0; i < name.length(); i++)
            hash = (29 * hash) + (name.at(i) * 5);

        return hash;
    }

    // print the name, type, definition, and hash code for the word
    void print() {
        cout << "{" << name << "}, {" << type << "}, {" << definition << "}, hash code = " << hashCode << endl;
    }
};

class Node {
public:
    Word * word;
    Node * next;

    // constructors
    Node() {
        word = nullptr;
        next = nullptr;
    }
    Node(Word * w) {
        word = w;
        next = nullptr;
    }
    Node(Word * w, Node * n) {
        word = w;
        next = n;
    }
};

class OpenAddressing {
public:
    int tableSize, numUsedCells;
    Word ** table;

    // default constructor
    OpenAddressing() {
        tableSize = 13;
        numUsedCells = 0;
        table = new Word * [tableSize]();
    }

    // insert word if table uses linear probing
    void insertLinearProbing(Word * w) {
        int tableIndex = 0;

        // if index from hashing is already filled, add one to the index until an empty index is found
        for (int i = 0; i < tableSize; i++) {
            tableIndex = (w->hashCode + i) % tableSize;

            if (table[tableIndex] == nullptr) {
                table[tableIndex] = w;
                numUsedCells++;

                break;
            }
        }

        // check if table needs rehashing
        rehash('l');
    }

    // insert word if table uses quadratic probing
    void insertQuadraticProbing(Word * w) {
        int tableIndex = 0;

        // if index from hashing is already filled, add one and square the index until an empty index is found
        for (int i = 0; i < tableSize; i++) {
            tableIndex = (w->hashCode + (int) pow(i, 2)) % tableSize;
            if (table[tableIndex] == nullptr) {
                table[tableIndex] = w;
                numUsedCells++;

                break;
            }
        }

        // check if table needs rehashing
        rehash('q');
    }

    // a different hashing function to be used in Double Hashing
    unsigned int secondHash(Word * w) {
        unsigned int hash2 = 0;
        for (int i = 0; i < w->name.length(); i++)
            hash2 = (37 * hash2) + (w->name.at(i) * 47);

        return hash2;
    }

    // insert word if table uses double hashing
    void insertDoubleHashing(Word * w) {
        unsigned int hash2 = secondHash(w);
        int tableIndex = 0;

        // if index from hashing is already filled, use a second hashing function and
        // multiply the result of the second hashing function by the index until an
        // empty index is found
        for (int i = 0; i < tableSize; i++) {
            tableIndex = (w->hashCode + ((unsigned) i * hash2)) % tableSize;
            if (table[tableIndex] == nullptr) {
                table[tableIndex] = w;
                numUsedCells++;

                break;
            }
        }

        // check if table needs rehashing
        rehash('d');
    }

    // get next prime number that is the smallest number that is larger than double the current table size
    int getNextPrime(int currTableSize) {
        int num = currTableSize * 2;

        while (true){
            bool hasFactors = false;
            for (int i = 2; i <= (num / 2); i++)
            {
                // found a factor that isn't 1 or n, so number isn't prime
                if (num % i == 0)
                    hasFactors = true;
            }

            if (hasFactors == false)
                break;
            else
                num++;
        }

        return num;
    }

    // Rehash if load factor is greater than 1/2
    void rehash(char type) {
        if ((double) numUsedCells / tableSize > 0.5) {
            // if rehash is needed, find the smallest number that is larger than double the table size
            int newTableSize = getNextPrime(tableSize);
            Word ** newTable = new Word * [newTableSize]();

            // for each element in table that's not empty, move to newTable
            for (int i = 0; i < tableSize; i++) {
                if (table[i] != nullptr) {
                    if (type == 'l') {
                        // insert w at table[i] into newTable
                        int tableIndex = 0;
                        for (int j = 0; j < newTableSize; j++) {
                            // use Linear Probing to insert into new table
                            tableIndex = (table[i]->hashCode + j) % newTableSize;
                            if (newTable[tableIndex] == nullptr) {
                                newTable[tableIndex] = table[i];
                                break;
                            }
                        }
                    }
                    else if (type == 'q') {
                        // insert w at table[i] into newTable
                        int tableIndex = 0;
                        for (int j = 0; j < newTableSize; j++) {
                            // use Quadratic Probing to insert into new table
                            tableIndex = (table[i]->hashCode + (int) pow(j, 2)) % newTableSize;
                            if (newTable[tableIndex] == nullptr) {
                                newTable[tableIndex] = table[i];
                                break;
                            }
                        }
                    }
                    else if (type == 'd') {
                        unsigned int hash2 = 0;
                        for (int j = 0; j < table[i]->name.length(); j++)
                            hash2 = (37 * hash2) + (table[i]->name.at(j) * 47);

                        // insert w at table[i] into newTable
                        int tableIndex = 0;
                        for (int j = 0; j < newTableSize; j++) {
                            // use Double Hashing to insert into new table
                            tableIndex = (table[i]->hashCode + ((unsigned) j * hash2)) % newTableSize;
                            if (newTable[tableIndex] == nullptr) {
                                newTable[tableIndex] = table[i];
                                break;
                            }
                        }
                    }
                }
            }

            // replace the old table with the new, larger table
            table = newTable;
            tableSize = newTableSize;
        }
    }

    // used for converting integer to string in return of find functions
    string intToStr(int num) {
        ostringstream ss;
        ss << num;
        return ss.str();
    }

    // find word if table uses Linear Probing
    string findLinearProbing(Word * w) {
        bool success = false;
        int investigated = 0;
        string type = "";
        string def = "";

        int tableIndex = 0;
        // if index from hashing doesn't contain the word, add one to the index until the word is found
        for (int i = 0; i < tableSize; i++) {
            investigated++;
            tableIndex = (w->hashCode + i) % tableSize;
            if (table[tableIndex] != nullptr && table[tableIndex]->name == w->name) {
                success = true;
                break;
            }
        }

        if (success == true)
            return "yes\t\t" + intToStr(investigated);
        else
            return "no\t\t" + intToStr(investigated);
    }

    // find word if table uses Quadratic Probing
    string findQuadraticProbing(Word * w) {
        bool success = false;
        int investigated = 0;
        string type = "";
        string def = "";

        int tableIndex = 0;
        // if index from hashing doesn't contain the word, add one and square the index until the word is found
        for (int i = 0; i < tableSize; i++) {
            investigated++;
            tableIndex = (w->hashCode + (int) pow(i, 2)) % tableSize;
            if (table[tableIndex] != nullptr && table[tableIndex]->name == w->name) {
                success = true;
                break;
            }
        }

        if (success == true)
            return "yes\t\t" + intToStr(investigated);
        else
            return "no\t\t" + intToStr(investigated);
    }

    // find word if table uses Double Hashing
    string findDoubleHashing(Word * w) {
        bool success = false;
        int investigated = 0;
        string type = "";
        string def = "";
        unsigned int hash2 = secondHash(w);

        int tableIndex = 0;
        // if index from hashing doesn't contain the word, use a second hashing function
        // and multiply the result of the second hashing function by the index until the
        // word is found
        for (int i = 0; i < tableSize; i++) {
            investigated++;
            tableIndex = (w->hashCode + ((unsigned) i * hash2)) % tableSize;
            if (table[tableIndex] != nullptr && table[tableIndex]->name == w->name) {
                success = true;
                break;
            }
        }

        if (success == true)
            return "yes\t\t" + intToStr(investigated);
        else
            return "no\t\t" + intToStr(investigated);
    }
};

class SeparateChaining {
public:
    int tableSize, numUsedCells;
    Node ** table;

    // default constructor
    SeparateChaining() {
        tableSize = 13;
        numUsedCells = 0;
        table = new Node * [tableSize]();
    }

    // insert word into table
    void insertSeparateChaining(Word * w) {
        int tableIndex = w->hashCode % tableSize;

        // if index in table is empty, place word at beginning
        if (table[tableIndex] == nullptr)
            table[tableIndex] = new Node(w);
        // if index already contains words, place word at beginning of list of words
        else {
            Node * newNode = new Node(w);
            newNode->next = table[tableIndex];
            table[tableIndex] = newNode;
        }
        numUsedCells++;
        rehash();
    }

    // get next prime number that is the smallest number that is larger than double the current table size
    int getNextPrime(int currTableSize) {
        int num = currTableSize * 2;

        while (true){
            bool hasFactors = false;
            for (int i = 2; i <= (num / 2); i++)
            {
                // found a factor that isn't 1 or n, so number isn't prime
                if (num % i == 0)
                    hasFactors = true;
            }

            if (hasFactors == false)
                break;
            else
                num++;
        }

        return num;
    }

    // Rehash if load factor is greater than 1
    void rehash() {
        // rehash if load factor is greater than 1
        if ((double) numUsedCells / tableSize > 1.0) {
            // if rehash is needed, find the smallest number that is larger than double the table size
            int newTableSize = getNextPrime(tableSize);
            Node ** newTable = new Node * [newTableSize]();

            for (int i = 0; i < tableSize; i++) {
                // for each element in table that's not empty, move to newTable
                if (table[i] != nullptr) {
                    // if only one element in index
                    if (table[i]->next == nullptr) {
                        int newTableIndex = table[i]->word->hashCode % newTableSize;
                        // use Separate Chaining to insert into new table
                        if (newTable[newTableIndex] == nullptr)
                            newTable[newTableIndex] = table[i];
                        else {
                            table[i]->next = newTable[newTableIndex];
                            newTable[newTableIndex] = table[i];
                        }
                    }
                    // if more than one element in index
                    else {
                        // move each element that's in index until index is empty
                        while (table[i] != nullptr) {
                            Node * temp = table[i]->next;
                            table[i]->next = nullptr;

                            int newTableIndex = table[i]->word->hashCode % newTableSize;
                            // use Separate Chaining to insert into new table
                            if (newTable[newTableIndex] == nullptr)
                                newTable[newTableIndex] = table[i];
                            else {
                                table[i]->next = newTable[newTableIndex];
                                newTable[newTableIndex] = table[i];
                            }

                            table[i] = temp;
                        }
                    }
                }
            }

            // replace the old table with the new, larger table
            table = newTable;
            tableSize = newTableSize;
        }
    }

    // used for converting integer to string in return of findSeparateChaining
    string intToStr(int num) {
        ostringstream ss;
        ss << num;
        return ss.str();
    }

    // find word in table
    string findSeparateChaining(Word * w) {
        bool success = false;
        int investigated = 0;
        string type = "";
        string def = "";

        int tableIndex = w->hashCode % tableSize;
        // if index is empty, end search
        if (table[tableIndex] == nullptr) {
            investigated++;
        }
        else {
            // if index isn't empty, look through each node until word is found
            Node * n = table[tableIndex];
            while (n != nullptr) {
                investigated++;
                if (n->word->name != w->name)
                    n = n->next;
                else {
                    type = n->word->type;
                    def = n->word->definition;
                    success = true;
                    break;
                }
            }
        }



        if (success == true)
            return type + "|" + def + "|yes\t\t" + intToStr(investigated);
        else
            return type + "|" + def + "|no\t\t" + intToStr(investigated);
    }
};

int main() {
    // Linear Probing
    OpenAddressing * lp = new OpenAddressing();
    // Quadratic Probing
    OpenAddressing * qp = new OpenAddressing();
    // Double Hashing
    OpenAddressing * dh = new OpenAddressing();
    // Separate Chaining
    SeparateChaining * sc = new SeparateChaining();

    // Total number of words in dictionary.txt
    int numWords = 0;

    // load dictionary into data structures
    ifstream inFile("dictionary.txt");
    if (inFile) {
        string line;

        while (inFile.eof() == false) {
            getline(inFile, line);
            string name, type, def;

            // separate line from dictionary.txt into name, type, and definition
            int pipe1 = line.find('|');
            int pipe2 = line.substr(pipe1 + 1).find('|');
            name = line.substr(0, pipe1);
            type = line.substr(pipe1 + 1, pipe2);
            def = line.substr(pipe1 + pipe2 + 2);

            // create word object
            Word * w = new Word(name, type, def);

            // inset word into each data structure
            lp->insertLinearProbing(w);
            qp->insertQuadraticProbing(w);
            dh->insertDoubleHashing(w);
            sc->insertSeparateChaining(w);

            numWords++;
        }
    }
    inFile.close();

    string input = " ";
    while (true) {
        cout << "Word to search for: ";
        getline(cin, input);

        // user can exit program by inputting nothing or -1
        if (input != "-1" && input != "") {
            Word * w = new Word(input);

            // perform search for word in each data structure
            string scResult = sc->findSeparateChaining(w);
            string lpResult = lp->findLinearProbing(w);
            string qpResult = qp->findQuadraticProbing(w);
            string dhResult = dh->findDoubleHashing(w);

            // if the word was found, print the name, type, and definition, and save the rest of the result
            // only result from separate chaining find contains type and definition, because if the word was
            // found in one data structure, it'll be found in the other three
            // format of result is:     type|definition|yes/no[tab]numberOfInvestigations
            if (scResult.find('|') != 0) {
                cout << input << " (" << scResult.substr(0, scResult.find('|')) << "): ";
                scResult = scResult.substr(scResult.find('|') + 1);
                cout << scResult.substr(0, scResult.find('|')) << "\n" << endl;
                scResult = scResult.substr(scResult.find('|') + 1);
            }
            else
                scResult = scResult.substr(2);

            // round to 3 decimals for lambda
            cout << fixed << setprecision(3);
            cout << "Total words: " << numWords << endl;
            cout << "Data Structure\t\tTable Size\tLambda\t\tSuccess\t\tItems Investigated" << endl;
            cout << "Linear Probing\t\t" << lp->tableSize << "\t\t" << ((double) lp->numUsedCells / lp->tableSize) << "\t\t" << lpResult << endl;
            cout << "Quadratic Probing\t" << qp->tableSize << "\t\t" << ((double) qp->numUsedCells / qp->tableSize) << "\t\t" << qpResult << endl;
            cout << "Separate Chaining\t" << sc->tableSize << "\t\t" << ((double) sc->numUsedCells / sc->tableSize) << "\t\t" << scResult << endl;
            cout << "Double Hashing\t\t" << dh->tableSize << "\t\t" << ((double) dh->numUsedCells / dh->tableSize) << "\t\t" << dhResult << "\n" << endl;
        }
        else
            break;
    }

    return 0;
}
