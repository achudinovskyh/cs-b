#ifndef MYPRIORITYQUEUE_H
#define MYPRIORITYQUEUE_H
#include <vector>

using namespace std;

template<typename T,typename U>
class MyPriorityQueue{
    int elementCount;
    vector<pair<T,U> >* vec;
    void bubbleUp(int);
    int findLesserChild(const int&);
    void swap(const int& , const int&);
    void bubbleDown(int);
public:
    MyPriorityQueue();
    ~MyPriorityQueue();
    bool empty();
    int size();
    pair<T,U> top();
    void push(const T& key,const U& value);
    void pop();
};

template<typename T,typename U>
void MyPriorityQueue<T,U>::bubbleUp(int position){
    int parent = (position - 1) / 2;

    while((parent >= 0) && ((*vec)[position]).first < ((*vec)[parent]).first){
        swap(position,parent);
        position = parent;
        parent = position / 2;
    }
}

template<typename T,typename U>
int MyPriorityQueue<T,U>::findLesserChild(const int& position){
    int leftChildPos = 2 * position + 1;
    int rightChildPos = 2 * position + 2;
    int lesserChildPos = 0;

    if(rightChildPos <= elementCount){
        ((*vec)[leftChildPos]).first < ((*vec)[rightChildPos]).first ? lesserChildPos = leftChildPos : lesserChildPos = rightChildPos;
        return lesserChildPos;
    }else if(leftChildPos <= elementCount){
        lesserChildPos = leftChildPos;
        return lesserChildPos;
    }else{
        return -1;
    }
}

template<typename T,typename U>
void MyPriorityQueue<T,U>::swap(const int& childPos, const int& parentPos){
    pair<T,U> temp = (*vec)[childPos];
    (*vec)[childPos] = (*vec)[parentPos];
    (*vec)[parentPos] = temp;
}

template<typename T,typename U>
void MyPriorityQueue<T,U>::bubbleDown(int position){
    int lesserChildPos = findLesserChild(position);

    while((lesserChildPos != -1) && ((*vec)[position]).first > ((*vec)[lesserChildPos]).first){
        swap(position,lesserChildPos);
        position = lesserChildPos;
        lesserChildPos = findLesserChild(position);
    }
}

template<typename T,typename U>
MyPriorityQueue<T,U>::MyPriorityQueue(){
    vec = new vector<pair<T,U> >;
    elementCount = -1;
}

template<typename T,typename U>
MyPriorityQueue<T,U>::~MyPriorityQueue(){
    delete vec;
}

template<typename T,typename U>
bool MyPriorityQueue<T,U>::empty(){
    return !(*vec).size();
}

template<typename T,typename U>
int MyPriorityQueue<T,U>::size(){
    return (*vec).size();
}

template<typename T,typename U>
pair<T,U> MyPriorityQueue<T,U>::top(){
    return (*vec)[0];
}

template<typename T,typename U>
void MyPriorityQueue<T,U>::push(const T& key,const U& value){
    elementCount++;
    pair<T,U> tempPair(key,value);
    (*vec).push_back(tempPair);
    bubbleUp(elementCount);
}

template<typename T,typename U>
void MyPriorityQueue<T,U>::pop(){
   swap(0,elementCount);
   (*vec).pop_back();
   elementCount--;
   bubbleDown(0);
}

#endif // MYPRIORITYQUEUE_H
