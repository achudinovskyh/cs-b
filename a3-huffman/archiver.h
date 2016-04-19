#ifndef ARCHIVER
#define ARCHIVER

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <string>
#include <fstream>
#include "mylist.h"

using namespace std;

class Archiver {
private:
    // struct Node it is a part for a binary tree
    struct Node {
        char c;
        int val;
        Node *left, *right ;

        Node()
        {
            left=right=NULL;
            newDelCounter++;
        }
        Node(Node *_left, Node *_right)
        {
            left =  _left;
            right = _right;
            val = _left->val + _right->val;
            newDelCounter++;
        }
        Node(char _c,int _val = 0)
        {
            c = _c;
            val = _val;
            left = NULL;
            right = NULL;
            newDelCounter++;
        }
        ~Node()
        {
            newDelCounter--;
        }
    };

    void    readAndCreateFrequencyTable(string* ,map<unsigned char,unsigned long>* );
    Node*   createTree(map<unsigned char,unsigned long>* );
    void    orderedInsert(Node* , list<Node*>* );
    void    createCodeTable(Node* ,vector<bool>* &, map<char,vector<bool> >* );
    void    printTable(map<char,vector<bool> >* );
    string  encodeTree(Node* root,string tempTreeString = "");
    int     calculateEncodedTreeLength(string* );
    unsigned int calculateEncodedFileLength(map<unsigned char,unsigned long>* ,map<char,vector<bool> >* );
    void    encodeFileAndWrite(string* ,int , int ,string* ,map<char,vector<bool> >* );
    void    deleteTree(Node* root);
    void    freeMemory(map<unsigned char,unsigned long>* ,Node* ,vector<bool>* ,map<char,vector<bool> >* ,string* );
    void    readFile(string* ,int& ,unsigned int& ,string* , string* );
    string* decodeTree(string* ,int );
    string* decodeFile(string* ,unsigned int );
    Node*   rebuildTree(char* &pt);
    void    decodeFileAndWrite(string* ,string* ,Node* );
    void    freeMemory(string* ,string* ,string* ,Node* );

public:
    // this int we use as a counter to find memory leaks
    static int newDelCounter;
    // function to zip file
    void zipFile(string* filePath);
    // function to unzip file
    void unzipFile(string* filePath);
    // print Menu function
    void menu();
};


#endif // ARCHIVER

