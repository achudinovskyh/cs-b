#ifndef MYLINKEDLIST_H
#define MYLINKEDLIST_H

template<typename T>
class MyLinkedList{
    struct Node {
        int data;
        Node* next;
        Node* prev;
        Node(){
            data = 0;
            next = prev = 0;
        }
        Node(int _data){
            data = _data;
            next = prev = 0;
        }
    };
    Node* first;
    Node* last;
    int lSize;
public:
    MyLinkedList();
    ~MyLinkedList();
    bool empty()const;
    int size()const;
    T& front()const;
    T& back()const;
    void push_front(const T&);
    void push_back(const T&);
    void pop_front();
    void pop_back();
};

template<typename T>
MyLinkedList<T>::MyLinkedList(){
    first = last = 0;
    lSize = 0;
}

template<typename T>
MyLinkedList<T>::~MyLinkedList(){
    if(lSize){
        while(first != last){
            Node* delPtr = first;
            first = first->next;
            delete delPtr;
        }
        delete first;
    }
}

template<typename T>
bool MyLinkedList<T>::empty() const{
    return !lSize;
}

template<typename T>
int MyLinkedList<T>::size() const{
    return lSize;
}

template<typename T>
T& MyLinkedList<T>::front() const{
    return first->data;
}

template<typename T>
T& MyLinkedList<T>::back() const{
    return last->data;
}

template<typename T>
void MyLinkedList<T>::push_front(const T &_data){
    if(lSize){
        Node* tempPtr = first;
        first = new Node(_data);
        first->next = tempPtr;
        tempPtr->prev = first;
        lSize++;
    }else{
        first = last = new Node(_data);
        lSize++;
    }
}

template<typename T>
void MyLinkedList<T>::push_back(const T &_data){
    if(lSize){
        Node* tempPtr = last;
        last = new Node(_data);
        last->prev = tempPtr;
        tempPtr->next = last;
        lSize++;
    }else{
        first = last = new Node(_data);
        lSize++;
    }
}

template<typename T>
void MyLinkedList<T>::pop_front(){
    if(lSize == 1){
        first->next = first->prev = 0;
        delete first;
        lSize--;
    }else if(lSize > 1){
        Node* delPtr = first;
        first = first->next;
        first->prev = 0;
        delete delPtr;
        lSize--;
    }
}

template<typename T>
void MyLinkedList<T>::pop_back(){
    if(lSize > 1){
        Node* delPtr = last;
        last = last->prev;
        last->next = 0;
        delete delPtr;
        lSize--;
    }else if(lSize == 1){
        Node* delPtr = last;
        last = last->prev;
        delete delPtr;
        lSize--;
    }
}

#endif // MYLINKEDLIST_H
