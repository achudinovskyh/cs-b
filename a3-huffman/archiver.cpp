#include "archiver.h"

bool deBug = 1;

int Archiver::newDelCounter = 0;

void Archiver::zipFile(string& filePath){
    map<unsigned char,unsigned long> frequencyTable;
    readAndCreateFrequencyTable(filePath,frequencyTable);

    Node* root = createTree(frequencyTable);
    MyVector<bool> code;
    map<char,MyVector<bool> > codeTable;
    createCodeTable(root,code,codeTable);
    if(deBug){
        printTable(codeTable);
        cout << "Encoding Tree!!!" << endl;
    }

    string encodedTree;
    encodedTree = encodeTree(root);
    if(deBug){
        cout << encodedTree << endl << "====================" << endl;
    }

    int encodedTreeLength = calculateEncodedTreeLength(encodedTree);
    unsigned int encodedFileLength = calculateEncodedFileLength(frequencyTable,codeTable);
    encodeFileAndWriteIt(filePath,encodedTreeLength,encodedFileLength,encodedTree,codeTable);
    deleteTree(root);
    if(deBug){
        cout << "Memory leaks: "<< newDelCounter << endl;
    }
}

void Archiver::unzipFile(string& filePath){
    int encodedTreeLength = 0;
    unsigned int encodedFileLength = 0;
    string encodedTree = "";
    string encodedFile = "";
    readFile(filePath,encodedTreeLength,encodedFileLength,encodedTree,encodedFile);

    string decodedTree = "";
    decodeTree(encodedTree,encodedTreeLength,decodedTree);
    if(deBug){
        cout << decodedTree << endl;
    }

    char* pt = &decodedTree[0];
    Node* root = rebuildTree(pt);
    if(deBug){
        cout << "The tree is bild!!!" << endl << "==============" << endl;
    }

    decodeFileAndWriteIt(filePath,encodedFile,root);
    deleteTree(root);

    if(deBug){
        cout << "Memory leaks: " << newDelCounter << endl;
    }
}

void Archiver::readAndCreateFrequencyTable(string& filePath,map<unsigned char,unsigned long>& frequencyTable){
    ifstream is(&filePath[0],ios_base::binary);
    // read file and creating Frequancy Table of characters in file.
    while (!is.eof()){
        char c = is.get();
        frequencyTable[c]++;
    }
    is.close();

    if(deBug){
        cout << "Map created!!!" << endl;
        for(map<unsigned char,unsigned long>::iterator it = frequencyTable.begin(); it != frequencyTable.end(); it++){
            cout << it->first << " : " << it->second << endl;
        }
        cout << "====================" << endl;
    }
}

Archiver::Node* Archiver::createTree(map<unsigned char,unsigned long>& frequencyTable){
    MyPriorityQueue<unsigned long,Node*> queue;

    //Taking every charackter from map and create a leaf with it
    for (map<unsigned char, unsigned long>::iterator iter = frequencyTable.begin(); iter != frequencyTable.end(); iter++) {
        Node *pNode = new Node(iter->first, iter->second);
        unsigned long nodeKey = pNode->val;
        queue.push(nodeKey,pNode);
    }

    //Build a tree by crossing two lowest Node's, until there is a single node it will be a root.
    while(queue.size() != 1){
        Node* temp1 = queue.top().second;
        queue.pop();
        Node* temp2 = queue.top().second;
        queue.pop();
        Node* newNode = new Node(temp1, temp2);
        unsigned long nodeKey = newNode->val;
        queue.push(nodeKey,newNode);
    }

    //Creating a root.
    Node* root = queue.top().second;

    if(deBug){
        cout << "Tree created!!!" << endl << "====================" << endl;
    }

    return root;
}

void Archiver::createCodeTable(Node* root,MyVector<bool>& code, map<char,MyVector<bool> >& codeTable){
    /* Walk throught the tree in recursion to get the code of each leaf. */

    // If ve go left, put '0'.
    if(root->left != NULL){
        code.push_back(0);
        createCodeTable(root->left,code,codeTable);
    }
    // If ve go right, put '1'.
    if(root->right != NULL){
        code.push_back(1);
        createCodeTable(root->right,code,codeTable);
    }
    // When we get to the leaf we get character that is stored in it, and the code how to get to it.
    if(root->left == NULL || root->right == NULL){
        codeTable[root->c] = code;
    }
    // And when we return in recursion we erase one digit from code.
    code.pop_back();
}

void Archiver::printTable(map<char,MyVector<bool> >& codeTable){
    /* This function we need only for debuging*/

    if(deBug){
        cout << "CodeTable created!!!" << endl;
        for(map<char,MyVector<bool> >::iterator itr=codeTable.begin(); itr!=codeTable.end(); ++itr){
            cout << itr->first << " : ";
            for(int i = 0; i < (itr->second).size(); i++){
                cout << (itr->second)[i];
            }
            cout << endl;
        }
        cout << "====================" << endl;
    }
}

string Archiver::encodeTree(Node* root,string tempTreeString){
    /* Encoding tree to instruction how to bild it. We will need it to decode the file. */

    // If ve get to leaf adding '1' and the character that is stored in it.
    if(root->left == NULL || root->right == NULL){
        tempTreeString += '1';
        tempTreeString += root->c;
        // If it isn't leaf add '0' and move forward in recursion.
    }else{
        tempTreeString += '0';
        tempTreeString = encodeTree(root->left,tempTreeString);
        tempTreeString = encodeTree(root->right,tempTreeString);
    }
    // returning string that contain encoded tree.
    return tempTreeString;
}

int Archiver::calculateEncodedTreeLength(string& encTree){
    /* We need to calculate length of the tree in bit's and write this value to file.
     * We will need this for decoding. Encoded tree contains only '0' and '1' that is folowed by char,
     * that's why when we see '0' we count +1 bit and when we see '1' and char count +9 */

    int lengthInBite = 0;
    for(int i = 0; i < encTree.length();i++){
        if(encTree[i] - '0' == 1){
            lengthInBite += 9;
            i += 1;
        }else{
            lengthInBite += 1;
        }
    }
    return lengthInBite;
}

unsigned int Archiver::calculateEncodedFileLength(map<unsigned char,unsigned long>& frequencyTable,map<char,MyVector<bool> >& codeTable){
    /* we need to calculate length of the file in bit's and write this value to file.
     * We will need this for decoding.We count amount of char's and their code length in codetable */

    unsigned int encodedFileLength = 0;
    for(map<unsigned char,unsigned long>::iterator it = frequencyTable.begin(); it != frequencyTable.end(); it++){
        MyVector<bool>temp = codeTable[it->first];
        encodedFileLength += (it->second) * temp.size();
    }

    return encodedFileLength;
}

void Archiver::encodeFileAndWriteIt(string& filePath,int encodedTreeLength, int encodedFileLength,string& encodedTree,map<char,MyVector<bool> >& codeTable){
    // adding ".huff" at the end of zipped file
    string newFilePath = filePath + ".huff";
    ofstream os(&newFilePath[0],ios_base::binary);

    //Writing length's to the output file
    os.write((const char*)(&encodedTreeLength),sizeof(int));
    os.write((const char*)(&encodedFileLength),sizeof(unsigned int));

    if(deBug){
        cout << "enTreeLen: " << encodedTreeLength << " enFileLen: " << encodedFileLength << "Encoding TREE!!!" << endl << "====================" << endl;
    }

    char buff = 0;
    int count = 0;

    //converting encoded tree from string to group of char's and writing them to output file
    for(int i = 0; i < encodedTree.length(); i++){
        // If it is '1' it means that next character is a symbol.
        if(encodedTree[i] - '0'){
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
                if(encodedTree[i] & 1 << (7- j)){
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
        }else{
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
        count = 0;
        os.put(buff);
        buff = 0;
    }

    if(deBug){
        cout << endl << "Tree writed out!"  << endl << "====================" << endl;
    }

    //encoding file and writing to output
    count = 0;
    char buff1 = 0;

    ifstream is(&filePath[0],ios_base::binary);
    while(!is.eof()){
        char ch = is.get();
        //  Put each character to map of <char,vector<bool> > and get the code of it.
        MyVector<bool> temp = codeTable[ch];

        // Put bit's in to a char and writing char to outputfile.
        for(int i = 0; i < temp.size(); i++){
            buff1 = buff1 | temp[i] << (7-count);
            count++;
            // Cheking counter.
            if (count==8){
                count=0;
                os.put(buff1);
                buff1=0;
            }
        }
    }
    // Same check if last buff not writed out.
    if(count < 8 && count != 0){
        count = 0;
        os.put(buff1);
        buff1 = 0;
    }
    os.close();

    if(deBug){
        cout << "File encoded, and writed out!!!" << endl << "====================" << endl;
    }
}

void Archiver::deleteTree(Node* ptr){
    // walk in recursion to leaf's and on the way back deleting each Node.
    if(ptr->left != NULL){
        deleteTree(ptr->left);
    }
    if(ptr->right != NULL){
        deleteTree(ptr->right);
    }
    delete ptr;
}




void Archiver::readFile(string& filePath, int& encodedTreeLength,unsigned int& encodedFileLength, string& encodedTree, string& encodedFile){
    /* Read two int's, that will be Tree length and File length temporarily calculating length's in byte's,
     * and read that amount of bytes in two string's that will be encoded tree and file. */

    ifstream is(&filePath[0],ios_base::binary);

    // Read the tree and file length's
    is.read((char*)&encodedTreeLength, sizeof(int));
    is.read((char*)&encodedFileLength, sizeof(unsigned int));

    if(deBug){
        cout << "EnTreeLen : " << encodedTreeLength << " EnFileLen: " << encodedFileLength << endl;
    }

    // Calculate length of Tree in bytes.
    int treeLenByte = 0;
    encodedTreeLength % 8 ? treeLenByte = (encodedTreeLength / 8) + 1 : treeLenByte = encodedTreeLength / 8;

    // Calculate length of File in bytes.
    int fileLenByte = 0;
    encodedFileLength % 8 ? fileLenByte = (encodedFileLength / 8) + 1 : fileLenByte = encodedFileLength / 8;

    if(deBug){
        cout << "EnTreeLenByte : " << treeLenByte << " EnFileLenByte: " << fileLenByte << endl;
    }

    //Encoding Tree from encoded char's to string of 0 and 1.
    for(int i = 0; i < treeLenByte; i++){
        char c = is.get();
        for(int j = 0; j < 8; j++) {
            if(c & (1 << (7-j))){
                encodedTree += "1";
            }else{
                encodedTree += "0";
            }
        }
    }

    //Encoding File from encoded char's to string of 0 and 1.
     for(int i = 0; i < fileLenByte; i++){
        char c = is.get();
        for(int j = 0; j < 8; j++){
            if(c & (1 << (7-j))){
                encodedFile += "1";
            }else{
                encodedFile += "0";
            }
        }
    }

    //Cut off the garbage part of the file
    encodedFile = encodedFile.substr(0,encodedFileLength - 1);

    if(deBug){
        cout << "Encoded File was read!!!"  << endl;
    }

    is.close();
}

void Archiver::decodeTree(string& encodedTree, int encodedTreeLength,string& decodedTree)
{
    /* Decoding from a group of char to a string of encoded tree,
     * which will be an instruction how to build a tree for decoding a file*/

    char enc_buff = 0;
    for(int i = 0; i < encodedTreeLength; i++){
        if(encodedTree[i] - '0'){
            decodedTree += "1";
            i++;
            for(int j = 0; j < 8; j++){
                if((encodedTree[i] - '0') == 1){
                    enc_buff = enc_buff | 1 << (7 - j);
                    i++;
                }else{
                    enc_buff = enc_buff | 0 << (7 - j);
                    i++;
                }
            }
            i--;
            decodedTree += enc_buff;
            enc_buff = 0;
        }else{
            decodedTree += "0";
        }
    }

    if(deBug){
        cout << "Tree was decoded!!! " << endl <<  "====================" << endl;
    }
}


Archiver::Node* Archiver::rebuildTree(char* &pt){
    /* Rebuilding a tree by instruction,we walk by string using a char* ,
     *  if we get '0' that means we on the Node that isn't a leaf
     *  if '1' it is a leaf creating the node with char that is folowed after '1'. */

    if ((*pt) - '0'){
        pt += 1;
        char c = *pt;
        return new Node(c);
    }else{
        pt += 1;
        Node* left = rebuildTree(pt);
        pt += 1;
        Node* right  = rebuildTree(pt);
        return new Node(left, right);
    }
}

void Archiver::decodeFileAndWriteIt(string& filePath,string& decodedFile,Node* root){
    /* Decoding file by walking on a tree, if we get '0' must go left, '1' go right.
     * If there is no right and left pointer's it means that we get to the leaf and need to write
     * a char that contains in it. */

    //deleting ".huff"
    filePath = filePath.substr(0, filePath.length() - 5);
    filePath += ".unz";

    ofstream os(&filePath[0], ios::out | ios::binary);

    Node* tempPtr = root;

    for(int i = 0; i < decodedFile.length(); i++){
        if((decodedFile[i] - '0') == 1){
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



void Archiver::menu(){
    cout << "1. To zip file.\n";
    cout << "2. To unzip file. \n";
    cout << "q. to quit the programm.\n";

}
