#ifndef MYQUEUE_H
#define MYQUEUE_H

template<typename T>
class MyQueue{
    struct Node {
        T data;
        Node* next;
        Node()
        {
            data = 0;
            next = 0;
        }
        Node(T _data)
        {
            data = _data;
            next = 0;
        }
    };

    Node* first;
    Node* last;
    int qSize;

public:
    MyQueue();
    ~MyQueue();
    MyQueue(const MyQueue& income);
    MyQueue& operator=(const MyQueue& income);
    bool empty()const;
    int size()const;
    T& front()const;
    T& back()const;
    void push(const T&);
    void pop();
};

template<typename T>
MyQueue<T>::MyQueue(){
    first = last = 0;
    qSize = 0;
}

template<typename T>
MyQueue<T>::~MyQueue(){
    if(qSize){
        while(first != last){
            Node* delPtr = first;
            first = first->next;
            delete delPtr;
        }
        delete first;
    }
}

template<typename T>
MyQueue<T>::MyQueue(const MyQueue& income){
    qSize = income.qSize;
    if(!qSize){
        first = last = 0;
    }else if(qSize == 1){
        first = last = new Node(income.first->data);
    }else{
        first = new Node(income.first->data);
        Node* tempPtr = first;
        Node* tempPtr_Income = income.first->next;
        while(tempPtr_Income){
            tempPtr->next = new Node(tempPtr_Income->data);
            tempPtr = tempPtr->next;
            tempPtr_Income = tempPtr_Income->next;
        }
        last = tempPtr;
    }
}

template<typename T>
MyQueue<T>& MyQueue<T>::operator=(const MyQueue& income){
    if(this != &income){
        delete this;
        this = MyQueue(income);
    }
    return *this;
}

template<typename T>
bool MyQueue<T>::empty() const{
    return qSize;
}

template<typename T>
int MyQueue<T>::size() const{
    return qSize;
}

template<typename T>
T& MyQueue<T>::front() const{
    if(qSize){
        return first->data;
    }
}

template<typename T>
T& MyQueue<T>::back() const{
    if(qSize){
        return last->data;
    }
}

template<typename T>
void MyQueue<T>::push(const T& _data){
    if(qSize){
        last->next = new Node(_data);
        last = last->next;
        qSize++;
    }else{
        first = last = new Node(_data);
        qSize++;
    }
}

template<typename T>
void MyQueue<T>::pop(){
    if(qSize == 1){
        delete first;
        first = last = 0;
        qSize--;
    }else if(qSize){
        Node* tempPtr = first->next;
        delete first;
        first = tempPtr;
        qSize--;
    }
}

#endif // MYQUEUE_H
