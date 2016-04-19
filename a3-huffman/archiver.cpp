#include "archiver.h"

bool deBug = 1;

int Archiver::newDelCounter = 0;

void Archiver::zipFile(string* filePath){
    map<unsigned char,unsigned long>* frequencyTable = new map<unsigned char,unsigned long>; newDelCounter++;
    readAndCreateFrequencyTable(filePath,frequencyTable);
    Node* root = createTree(frequencyTable);
    vector<bool>* code = new vector<bool>; newDelCounter++;
    map<char,vector<bool> >* codeTable = new map<char,vector<bool> >; newDelCounter++;
    createCodeTable(root,code,codeTable);
    if(deBug){printTable(codeTable); cout << "Encoding Tree!!!\n";}
    string* encodedTree = new string; newDelCounter++;
    *encodedTree = encodeTree(root);
    if(deBug){cout << *encodedTree << endl << "====================\n";}
    int encodedTreeLength = calculateEncodedTreeLength(encodedTree);
    unsigned int encodedFileLength = calculateEncodedFileLength(frequencyTable,codeTable);
    encodeFileAndWrite(filePath,encodedTreeLength,encodedFileLength,encodedTree,codeTable);
    freeMemory(frequencyTable,root,code,codeTable,encodedTree);
    if(deBug){cout << "Memory leaks: "<< newDelCounter << endl;}
}

void Archiver::unzipFile(string* filePath)
{
    int encodedTreeLength = 0;
    unsigned int encodedFileLength = 0;
    string* encodedTree = new string; newDelCounter++;
    string* encodedFile = new string; newDelCounter++;
    readFile(filePath,encodedTreeLength,encodedFileLength,encodedTree,encodedFile);
    string* decodedTree = decodeTree(encodedTree,encodedTreeLength);
    if(deBug){cout << *decodedTree << endl;}
    char* pt = &(*decodedTree)[0];
    Node* root = rebuildTree(pt);
    if(deBug){cout << "The tree is bild!!!\n==============\n";}
    decodeFileAndWrite(filePath,encodedFile,root);
    freeMemory(encodedTree,encodedFile,decodedTree,root);

    if(deBug){cout << "Memory leaks: "<< newDelCounter << endl;}
}

void Archiver::readAndCreateFrequencyTable(string* filePath,map<unsigned char,unsigned long>* frequencyTable)
{
    ifstream is((*filePath).c_str(),ios_base::binary);
    // read file and creating Frequancy Table of characters in file.
    while (!is.eof())
    {
        char c = is.get();
        (*frequencyTable)[c]++;
    }
    is.close();

    if(deBug){
        cout << "Map created!!!\n";
        for(map<unsigned char,unsigned long>::iterator it=(*frequencyTable).begin();it != (*frequencyTable).end();it++){
            cout << it->first << " : " << it->second << endl;
        }
        cout << "====================\n";
    }
}

Archiver::Node* Archiver::createTree(map<unsigned char,unsigned long>* frequencyTable)
{
    //Creating list of Nodes
    list<Node *> *tempList = new list<Node *>;
    //Taking every charackter from map and create a leaf with it
    for (map<unsigned char, unsigned long>::iterator iter = (*frequencyTable).begin(); iter != (*frequencyTable).end(); iter++) {
        Node *pNode = new Node(iter->first, iter->second);
        orderedInsert(pNode, tempList);
    }
    //Build a tree by crossing two lowest Node's, until there is a single node it will be a root.

    while((*tempList).size() != 1)
    {
        list<Node*>::iterator it;
        for ( it = (*tempList).begin(); it != (*tempList).end(); it++)
        {   //Taking wto Node's from the list, and creating their parent Node
            Node* temp1 = *it;
            ++it;
            Node* temp2 = *it;
            Node* newNode = new Node(temp1,temp2);
            (*tempList).pop_front();
            (*tempList).pop_front();
            //Inserting parent Node in sorted list
            orderedInsert(newNode, tempList);
            break;
        }

    }
    //Creating a root.
    Node* root = (*tempList).front();

    if(deBug){ cout << "Tree created!!!\n====================\n";}

    delete tempList; newDelCounter--;
    return root;
}

void Archiver::orderedInsert(Node* node, list<Node*>* tempList) {
    //Inserting a Node sorting it by value.
    bool isInserted = false;
    if ((*tempList).empty()) {
        (*tempList).push_back(node);
        isInserted = true;
    } else {
        list<Node *>::iterator i;
        for (i = (*tempList).begin(); i != (*tempList).end(); ++i) {
            Node* tempCompare = *i;
            if (node->val < tempCompare->val) {
                (*tempList).insert(i, node);
                isInserted = true;
                break;
            }
        }
    }
    if (!isInserted) {
        (*tempList).push_back(node);
    }
}

void Archiver::createCodeTable(Node* root,vector<bool>* &code, map<char,vector<bool> >* codeTable){
    /* Walk throught the tree in recursion to get the code of each leaf. */

    // If ve go left, put '0'.
    if(root->left != NULL){
        (*code).push_back(0);
        createCodeTable(root->left,code,codeTable);
    }
    // If ve go right, put '1'.
    if(root->right != NULL){
        (*code).push_back(1);
        createCodeTable(root->right,code,codeTable);
    }
    // When we get to the leaf we get character that is stored in it, and the code how to get to it.
    if(root->left == NULL || root->right == NULL){
        (*codeTable)[root->c]= (*code);
    }
    // And when we return in recursion we erase one digit from code.
    (*code).pop_back();
}

void Archiver::printTable(map<char,vector<bool> >* codeTable){
    /* This function we need only for debuging*/

    if(deBug){ cout << "CodeTable created!!!\n";
        for(map<char,vector<bool> >::iterator itr=(*codeTable).begin(); itr!=(*codeTable).end(); ++itr){
            cout << itr->first << " : ";
            for(int i =0;i<(itr->second).size();i++){
                cout << (itr->second)[i];
            }
            cout << endl;
        }
        cout << "====================\n";
    }
}

string Archiver::encodeTree(Node* root,string tempTreeString){
    /* Encoding tree to instruction how to bild it. We will need it to decode the file. */

    // If ve get to leaf adding '1' and the character that is stored in it.
    if(root->left == NULL || root->right == NULL){
        tempTreeString += '1';
        tempTreeString += root->c;
        // If it isn't leaf add '0' and move forward in recursion.
    }else {
        tempTreeString += '0';
        tempTreeString = encodeTree(root->left,tempTreeString);
        tempTreeString = encodeTree(root->right,tempTreeString);

    }
    // returning string that contain encoded tree.
    return tempTreeString;
}

int Archiver::calculateEncodedTreeLength(string* encTree)
{
    /* We need to calculate length of the tree in bit's and write this value to file.
     * We will need this for decoding. Encoded tree contains only '0' and '1' that is folowed by char,
     * that's why when we see '0' we count +1 bit and when we see '1' and char count +9 */
    int lengthInBite = 0;
    for(int i = 0; i < (*encTree).length();i++){
        if((*encTree)[i] - '0' == 1){
            lengthInBite += 9;
            i += 1;
        }else{
            lengthInBite += 1;
        }
    }
    return lengthInBite;
}

unsigned int Archiver::calculateEncodedFileLength(map<unsigned char,unsigned long>* frequencyTable,map<char,vector<bool> >* codeTable)
{
    /* we need to calculate length of the file in bit's and write this value to file.
     * We will need this for decoding.We count amount of char's and their code length in codetable */
    unsigned int encodedFileLength = 0;
    for(map<unsigned char,unsigned long>::iterator it = (*frequencyTable).begin();it != (*frequencyTable).end();it++){
        vector<bool>temp = (*codeTable)[it->first];
        encodedFileLength += (it->second) * temp.size();
    }

    return encodedFileLength;
}

void Archiver::encodeFileAndWrite(string* filePath,int encodedTreeLength, int encodedFileLength,string* encodedTree,map<char,vector<bool> >* codeTable)
{
    // adding ".huff" at the end of zipped file
    string newFilePath = *filePath + ".huff";
    ofstream os(newFilePath.c_str(),ios_base::binary);

    //Writing length's to the output file
    os.write(reinterpret_cast<const char *>(&encodedTreeLength),sizeof(int));
    os.write(reinterpret_cast<const char *>(&encodedFileLength),sizeof(unsigned int));

    if(deBug){
        cout << "enTreeLen: " << encodedTreeLength << " enFileLen: " << encodedFileLength << "Encoding TREE!!!\n" << "====================\n" ;
    }

    char buff = 0;
    int count = 0;

    //converting encoded tree from string to group of char's and writing them to output file
    for(int i = 0; i < (*encodedTree).length(); i++){
        // If it is '1' it means that next character is a symbol.
        if(((*encodedTree)[i] - '0')){
            // Putt 1 in buff
            buff = buff | (1 << (7 - count));
            count++;
            i++;
            // Cheking counter when it become 8 it will means that char buff is filled and we need to write it to output.
            if(count == 8){
                count = 0;
                os.put(buff);
                cout << buff;
                buff = 0;
            }
            // Run the cycle for next 8 bits of symbol.
            for(int j = 0; j < 8; j++){
                if((*encodedTree)[i] & 1 << (7- j)){
                    buff = buff | 1 << (7 - count);
                    count++;
                }else{
                    buff = buff | 0 << (7 - count);
                    count++;
                }
                // Cheking counter.
                if(count == 8){
                    count = 0;
                    os.put(buff);
                    cout << buff;
                    buff = 0;
                }
            }
        }
        else
        {
            // If it is '0' just put it in to char buff.
            buff = buff | 0 << (7 - count);
            count++;
            // Cheking counter.
            if(count == 8){
                count = 0;
                os.put(buff);
                cout << buff;
                buff = 0;
            }
        }
    }
    /* In case file length is not a multiple of eight, buff will have some bits,
     * but not writed to output file. Cheking if counter != 0 writing buff to output file. */
    if(count < 8 && count != 0){
        count = 0;   os.put(buff); buff = 0;
    }

    if(deBug){ cout << "\nTree writed out!\n" << "====================\n";}

    //encoding file and writing to output
    count = 0;
    char buff1 = 0;


    ifstream is((*filePath).c_str(),ios_base::binary);
    while(!is.eof()){
        char ch = is.get();
        //  Put each character to map of <char,vector<bool> > and get the code of it.
        vector<bool> temp = (*codeTable)[ch];
        // Put bit's in to a char and writing char to outputfile.
        for(int i = 0; i < temp.size(); i++){
            buff1 = buff1 | temp[i] << (7-count);
            count++;
            // Cheking counter.
            if (count==8) {
                count=0;   os.put(buff1); buff1=0; }
        }
    }
    // Same check if last buff not writed out.
    if(count < 8 && count != 0){
        count = 0;   os.put(buff1); buff1 = 0;
    }
    os.close();

    if(deBug){ cout << "File encoded, and writed out!!!\n" << "====================\n";}


}

void Archiver::deleteTree(Node* ptr)
{
    // walk in recursion to leaf's and on the way back deleting each Node.
    if(ptr->left != NULL){
        deleteTree(ptr->left);
    }
    if(ptr->right != NULL){
        deleteTree(ptr->right);
    }
    //if(deBug){cout << "Deleteing the Node!" << endl;}
    delete ptr;

}

void Archiver::freeMemory(map<unsigned char,unsigned long>* frequencyTable,Node* root,vector<bool>* code,map<char,vector<bool> >* codeTable,string* encodedTree)
{
    // frees memory
    delete frequencyTable;  newDelCounter--;
    deleteTree(root);
    delete code;    newDelCounter--;
    delete codeTable;   newDelCounter--;
    delete encodedTree;    newDelCounter--;
}


void Archiver::readFile(string* filePath, int& encodedTreeLength,unsigned int& encodedFileLength, string* encodedTree, string* encodedFile)
{
    /* Read two int's, that will be Tree length and File length temporarily calculating length's in byte's,
     * and read that amount of bytes in two string's that will be encoded tree and file. */

    ifstream is((*filePath).c_str(),ios_base::binary);

    // Read the tree and file length's
    is.read(reinterpret_cast<char*>(&encodedTreeLength), sizeof(int));
    is.read(reinterpret_cast<char*>(&encodedFileLength), sizeof(unsigned int));
    if(deBug){
        cout << "EnTreeLen : " << encodedTreeLength << " EnFileLen: " << encodedFileLength << endl;
    }

    // Calculate length of Tree in bytes.
    int treeLenByte = 0;
    if((encodedTreeLength % 8) != 0){
        treeLenByte = (encodedTreeLength / 8) + 1;
    }else{
        treeLenByte = encodedTreeLength / 8;
    }
    // Calculate length of File in bytes.
    int fileLenByte = 0;
    if((encodedFileLength % 8) != 0){
        fileLenByte = (encodedFileLength / 8) + 1;
    }else{
        fileLenByte = encodedFileLength / 8;
    }

    if(deBug){
        cout << "EnTreeLenByte : " << treeLenByte << " EnFileLenByte: " << fileLenByte << endl;
    }
    //Encoding Tree from encoded char's to string of 0 and 1.
    *encodedTree = "";
    for(int i = 0; i < treeLenByte; i++){
        char c = is.get();
        for(int j = 0; j < 8; j++)
        {
            if(c & (1 << (7-j)))
            {
                *encodedTree += "1";
            }else
            {
                *encodedTree += "0";
            }
        }
    }
    //Encoding File from encoded char's to string of 0 and 1.
    *encodedFile = "";
    for(int i = 0; i < fileLenByte; i++){
        char c = is.get();
        for(int j = 0; j < 8; j++)
        {
            if(c & (1 << (7-j)))
            {
                *encodedFile += "1";
            }
            else
            {
                *encodedFile += "0";
            }
        }
    }
    //Cut off the garbage part of the file
    *encodedFile = (*encodedFile).substr(0,encodedFileLength - 1);

    if(deBug){ cout << "Encoded File was read!!!"  << endl;}

    is.close();
}

string* Archiver::decodeTree(string* encodedTree, int encodedTreeLength)
{
    /* Decoding from a group of char to a string of encoded tree,
     * which will be an instruction how to build a tree for decoding a file*/
    string* decodedTree = new string; newDelCounter++;

    char enc_buff = 0;
    for(int i = 0; i < encodedTreeLength; i++)
    {
        if((*encodedTree)[i] - '0')
        {
            *decodedTree += "1";
            i++;
            for(int j = 0; j < 8; j++){
                if(((*encodedTree)[i] - '0') == 1){
                    enc_buff = enc_buff | 1 << (7 - j);
                    i++;
                }else{
                    enc_buff = enc_buff | 0 << (7 - j);
                    i++;
                }
            }
            i--;
            *decodedTree += enc_buff;
            cout << enc_buff;
            enc_buff = 0;
        }
        else
        {
            *decodedTree += "0";
        }
    }

    if(deBug){cout << "Tree was decoded!!! \n" <<  "====================\n";}

    return decodedTree;
}


Archiver::Node* Archiver::rebuildTree(char* &pt)
{
    /* Rebuilding a tree by instruction,we walk by string using a char* ,
     *  if we get '0' that means we on the Node that isn't a leaf
     *  if '1' it is a leaf creating the node with char that is folowed after '1'. */
    if (((*pt) - '0'))
    {
        pt += 1;
        char c = *pt;
        return new Node(c);
    }
    else
    {
        pt += 1;
        Node* left = rebuildTree(pt);
        pt += 1;
        Node* right  = rebuildTree(pt);
        return new Node(left, right);
    }
}

void Archiver::decodeFileAndWrite(string* filePath,string* decodedFile,Node* root)
{
    /* Decoding file by walking on a tree, if we get '0' must go left, '1' go right.
     * If there is no right and left pointer's it means that we get to the leaf and need to write
     * a char that contains in it. */

    //deleting ".huff"
    *filePath = (*filePath).substr(0, (*filePath).length() - 5);
    *filePath += ".unz";

    ofstream os((*filePath).c_str(), ios::out | ios::binary);

    Node* tempPtr = root;

    for(int i = 0; i < (*decodedFile).length();i++){
        if(((*decodedFile)[i] - '0') == 1){
            tempPtr=tempPtr->right;
        }else{
            tempPtr=tempPtr->left;
        }
        if (tempPtr->left==NULL && tempPtr->right==NULL){
            os.put(tempPtr->c);
            tempPtr=root;
        }

    }
}

void Archiver::freeMemory(string* encodedTree,string* encodedFile,string* decodedTree,Node* root)
{
    // frees memory
    delete encodedTree;    newDelCounter--;
    delete encodedFile;  newDelCounter--;
    delete decodedTree; newDelCounter--;
    deleteTree(root);

}

void Archiver::menu()
{
    cout << "1. To zip file.\n";
    cout << "2. To unzip file. \n";
    cout << "q. to quit the programm.\n";

}
