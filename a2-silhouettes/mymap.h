#ifndef MYMAP_H
#define MYMAP_H
#include <iostream>

using namespace std;

template <class K, class V>
class MyMap{
    struct Node {
        K key;
        V value;
        bool color;
        Node *left, *right, *parent;

        Node(){
            left = right = parent = 0;
        }
        Node(K _key,V _value){
            left = right = parent = 0;
            key = _key;
            value = _value;
            color = 1;
        }
        Node(K _key,V _value, Node* _parent){
            left = right  = 0;
            parent = _parent;
            key = _key;
            value = _value;
            color = 1;
        }
        ~Node(){}
    };

    int mSize;
    static Node* root;

    void balanceTree(Node*);
    void case1(Node*);
    void case2(Node*);
    void case3(Node*);
    void case1l(Node*);
    void case2l(Node*);
    void case3l(Node*);

    Node* findMinKey(Node*);
    Node* findMaxKey(Node*);

    void deleteTree(Node* ptr);
    V* findValue(Node* ,const K& );
public:

    struct iterator{
       Node* node;
       iterator(Node* node = 0);
       void operator++();
       void operator++(int);
       bool operator!=(iterator);
       Node* operator->();
       bool hasNext;
       Node* findMinAfterThis(K ,Node*);
       K findMaxKey(Node*);
    };
    iterator begin();
    iterator end();

    MyMap();
    ~MyMap();
    void insert(K ,V = 0 , Node* ptr = root);
    void printTree(Node* ptr = root, int n = 0);
    V& operator[](const K&);
    bool isEmpty()const;
};



template <class K, class V>
MyMap<K,V>::MyMap(){
    mSize = 0;
    root = 0;
}

template <class K, class V>
MyMap<K,V>::~MyMap(){
    deleteTree(root);
}

template <class K, class V>
typename MyMap<K,V>::Node* MyMap<K,V>::root = 0;

template <class K, class V>
void MyMap<K,V>::insert(K key,V value, Node* nodePtr){
    if(nodePtr == 0){
        root = new Node(key,value);
        mSize++;
        root->color = 0;
    }else if(key < nodePtr->key){
        if(nodePtr->left != 0){
            insert(key,value,nodePtr->left);
        }else{
            nodePtr->left = new Node(key,value,nodePtr);
            mSize++;
            balanceTree(nodePtr->left);
        }
    }else if(key > nodePtr->key){
        if(nodePtr->right != 0){
            insert(key,value,nodePtr->right);
        }else{
            nodePtr->right = new Node(key,value,nodePtr);
            mSize++;
            balanceTree(nodePtr->right);
        }
    }
}

template <class K, class V>
void MyMap<K,V>::balanceTree(Node* x){
    while(x != root && x->parent->color){
        if(x->parent == x->parent->parent->left){
            if(x->parent->parent->right != 0 && x->parent->parent->right->color){
                case1(x);
            }else if((x->parent->parent->right == 0 || !x->parent->parent->right->color ) && x == x->parent->right ){
                case2(x);
            }else if((x->parent->parent->right == 0 || !x->parent->parent->right->color ) && x == x->parent->left){
                case3(x);
            }
        }else{
            if(x->parent->parent->left != 0 && x->parent->parent->left->color){
                case1l(x);
            }else if((x->parent->parent->left == 0 || !x->parent->parent->left->color) && x == x->parent->left){
                case2l(x);
            }else if((x->parent->parent->left == 0 || !x->parent->parent->left->color) && x == x->parent->right){
                case3l(x);
            }
        }
    }
    root->color = 0;
}


template <class K, class V>
void MyMap<K,V>::case1(Node* x){
    x->parent->color = 0;
    x->parent->parent->color = 1;
    x->parent->parent->right->color = 0;
    balanceTree(x->parent->parent);
}

template <class K, class V>
void MyMap<K,V>::case2(Node* x){
    Node* parent = x->parent;
    Node* grandParent = x->parent->parent;
    Node* xLeft = x->left;

    grandParent->left = x;
    x->parent = grandParent;
    x->left = parent;
    parent->parent = x;
    parent->right = xLeft;
    if(xLeft){
        xLeft->parent = parent;
    }
    balanceTree(x->left);
}

template <class K, class V>
void MyMap<K,V>::case3(Node* x){
    Node* parentRight = x->parent->right;
    Node* parent = x->parent;
    Node* grandParent = x->parent->parent;
    Node* uncle = x->parent->parent->right;

    K tempKey = parent->key;
    parent->key = grandParent->key;
    grandParent->key = tempKey;

    V tempValue = parent->value;
    parent->value = grandParent->value;
    grandParent->value = tempValue;

    grandParent->left = x;
    x->parent = grandParent;
    grandParent->right = parent;
    parent->parent = grandParent;
    parent->left = parentRight;
    parent->right = uncle;
    if(uncle){
        uncle->parent = parent;
    }
}

template <class K, class V>
void MyMap<K,V>::case1l(Node* x){
    x->parent->color = 0;
    x->parent->parent->color = 1;
    x->parent->parent->left->color = 0;
    balanceTree(x->parent->parent);
}

template <class K, class V>
void MyMap<K,V>::case2l(Node* x){
    Node* parent = x->parent;
    Node* grandParent = x->parent->parent;
    Node* xRight = x->right;

    grandParent->right = x;
    x->parent = grandParent;
    x->right = parent;
    parent->parent = x;
    parent->left = xRight;
    if(xRight){
        xRight->parent = parent;
    }
    balanceTree(x->right);

}

template <class K, class V>
void MyMap<K,V>::case3l(Node* x){
    Node* parentLeft = x->parent->left;
    Node* parent = x->parent;
    Node* grandParent = x->parent->parent;
    Node* uncle = x->parent->parent->left;

    K tempKey = parent->key;
    parent->key = grandParent->key;
    grandParent->key = tempKey;

    V tempValue = parent->value;
    parent->value = grandParent->value;
    grandParent->value = tempValue;

    grandParent->right = x;
    x->parent = grandParent;
    grandParent->left = parent;
    parent->parent = grandParent;
    parent->right = parentLeft;
    parent->left = uncle;
    if(uncle){
        uncle->parent = parent;
    }
}

template <class K, class V>
typename MyMap<K,V>::Node *MyMap<K,V>::findMinKey(Node* ptr){
    if(ptr->left){
        ptr = findMinKey(ptr->left);
    }
    return ptr;
}

template <class K, class V>
typename MyMap<K,V>::Node *MyMap<K,V>::findMaxKey(Node* ptr){
    if(ptr->right){
        ptr = findMaxKey(ptr->right);
    }
    return ptr;
}

template <class K, class V>
void MyMap<K,V>::deleteTree(MyMap::Node *ptr){
    // walk in recursion to leaf's and on the way back deleting each Node.
    if(ptr->left != 0){
        deleteTree(ptr->left);
    }
    if(ptr->right != 0){
        deleteTree(ptr->right);
    }
    delete ptr;
}

template <class K, class V>
typename MyMap<K,V>::iterator MyMap<K,V>::begin(){
    return iterator(findMinKey(root));
}

template <class K, class V>
typename MyMap<K,V>::iterator MyMap<K,V>::end(){
    if(mSize){
        iterator it = findMaxKey(root);
        it.hasNext = false;
        return it;
    }else{
        return 0;
    }
}

template <class K, class V>
void MyMap<K,V>::printTree(Node* root,int n){
    if(root->right){
        printTree(root->right,n+10);
    }
    for(int i = 0; i < n;i++){
        cout << " ";
    }
    cout << "K:" << root->key << " V:"<< root->value << " c. " << root->color<< endl;
    if(root->left){
        printTree(root->left,n+10);
    }
}

template <class K, class V>
V* MyMap<K,V>::findValue(Node* ptr, const K& _key){
    V* tempValue = 0;
    if(ptr->key == _key){
       tempValue = &ptr->value;
    }else{
        if(ptr->right && ptr->key < _key){
            tempValue = findValue(ptr->right,_key);
        }else if(ptr->left && ptr->key > _key){
            tempValue = findValue(ptr->left,_key);
        }
    }
    return tempValue;
}

template <class K, class V>
V& MyMap<K,V>::operator[](const K& key){
    V* temp = 0;
    if(root){
        temp = findValue(root,key);
    }
    // if not exist
    if(!temp){
        this->insert(key);
        temp = findValue(root,key);
    }
    return *temp;
}

template <class K, class V>
bool MyMap<K,V>::isEmpty() const{
   return !mSize;
}

template <class K, class V>
MyMap<K,V>::iterator::iterator(MyMap::Node* _node){
    this->node = _node;
    hasNext = true;
}

template <class K, class V>
void MyMap<K,V>::iterator::operator++(){
    if(hasNext){
        node = findMinAfterThis(this->node->key,root);
    }
    if(!node){
        hasNext = false;
    }
}

template <class K, class V>
void MyMap<K,V>::iterator::operator++(int){
    if(hasNext){
        node = findMinAfterThis(this->node->key,root);
    }
    if(!node){
        hasNext = false;
    }
}

template <class K, class V>
bool MyMap<K,V>::iterator::operator!=(MyMap::iterator){
    return this->node != 0;
}

template <class K, class V>
typename MyMap<K,V>::Node *MyMap<K,V>::iterator::operator->(){
    return this->node;
}

template <class K, class V>
typename MyMap<K,V>::Node *MyMap<K,V>::iterator::findMinAfterThis(K incValue, Node *ptr){
    Node* tempNode;
    if(ptr->key > incValue){
        if(ptr->left != NULL && findMaxKey(ptr->left) > incValue){
            tempNode = findMinAfterThis(incValue,ptr->left);
        }else{
            return ptr;
        }
    }else{
        if(ptr->key == incValue && !ptr->right){
            return 0;
        }
        tempNode = findMinAfterThis(incValue,ptr->right);
    }
}

template <class K, class V>
K MyMap<K,V>::iterator::findMaxKey(MyMap::Node *ptr){
    if(ptr->right){
        findMaxKey(ptr->right);
    }
    return ptr->key;

}

#endif // MYMAP_H
