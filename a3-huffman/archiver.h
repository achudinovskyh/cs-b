#ifndef ARCHIVER
#define ARCHIVER

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include "myvector.h"
#include "mypriorityqueue.h"

using namespace std;

class Archiver {
private:
    // struct Node it is a part for a binary tree
    struct Node {
        char c;
        int val;
        Node *left, *right ;

        Node(){
            left=right=NULL;
            newDelCounter++;
        }
        Node(Node *_left, Node *_right){
            left =  _left;
            right = _right;
            val = _left->val + _right->val;
            newDelCounter++;
        }
        Node(char _c,int _val = 0){
            c = _c;
            val = _val;
            left = NULL;
            right = NULL;
            newDelCounter++;
        }
        ~Node(){
            newDelCounter--;
        }
    };

    void    readAndCreateFrequencyTable(string& ,map<unsigned char,unsigned long>& );
    Node*   createTree(map<unsigned char,unsigned long>&);
    void    createCodeTable(Node* ,MyVector<bool>&, map<char,MyVector<bool> >& );
    void    printTable(map<char,MyVector<bool> >& );
    string  encodeTree(Node* root,string tempTreeString = "");
    int     calculateEncodedTreeLength(string& );
    unsigned int calculateEncodedFileLength(map<unsigned char,unsigned long>& ,map<char,MyVector<bool> >& );
    void    encodeFileAndWriteIt(string& ,int , int ,string& ,map<char,MyVector<bool> >& );

    void    readFile(string& ,int& ,unsigned int& ,string& , string& );
    void    decodeTree(string& ,int ,string&);
    string* decodeFile(string* ,unsigned int );
    Node*   rebuildTree(char* &pt);
    void    decodeFileAndWriteIt(string& ,string& ,Node* );

    void    deleteTree(Node* root);
public:
    // this int we use as a counter for memory leaks
    static int newDelCounter;

    // function to zip file
    void zipFile(string& filePath);

    // function to unzip file
    void unzipFile(string& filePath);

    // print Menu function
    void menu();
};


#endif // ARCHIVER

