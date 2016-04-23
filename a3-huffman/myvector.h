#ifndef MYVECTOR_H
#define MYVECTOR_H

template<typename T>
class MyVector{
    int maxSize,filled;
    T* array;
    void allocateMemory();
public:
    MyVector();
    MyVector(int);
    MyVector(const MyVector&);
    ~MyVector();
    void push_back(const T&);
    int size();
    T operator[](int);
    MyVector& operator=(const MyVector&);
    T at(int);
    void pop_back();
};


template<typename T>
MyVector<T>::MyVector(){
    array = 0;
    maxSize = filled = 0;
}

template<typename T>
MyVector<T>::MyVector(int size){
    array = new T[size];
    maxSize = size;
    filled = 0;
}

template<typename T>
MyVector<T>::MyVector(const MyVector& source){
    maxSize = source.maxSize;
    filled = source.filled;
    array = new T[maxSize];
    for(int i = 0; i < filled; i++){
        array[i] = source.array[i];
    }
}

template<typename T>
MyVector<T>::~MyVector(){
    delete[] array;
}

template<typename T>
void MyVector<T>::push_back(const T& elem){
    if(maxSize == 0){
        array = new T[filled + 1];
        maxSize = filled + 1;
        array[filled] = elem;
        filled++;
    }else if(filled == maxSize){
        T* newArray = new T[maxSize*2];
        for(int i = 0;i < filled;i++){
            newArray[i] = array[i];
        }
        delete []array;
        array = newArray;
        maxSize *=2;
        array[filled] = elem;
        filled++;
    }else{
        array[filled] = elem;
        filled++;
    }
}

template<typename T>
int MyVector<T>::size(){
    return filled;
}

template<typename T>
T MyVector<T>::operator[](int nElem){
    if(nElem >= 0 && nElem <= filled){
        return array[nElem];
    }
}

template<typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& income){
    if(this != &income){
        delete[] array;
        this->maxSize = income.maxSize;
        this->filled = income.filled;
        array = new T[maxSize];
        for(int i = 0; i < filled; i++){
            array[i] = income.array[i];
        }
    }
    return *this;
}

template<typename T>
T MyVector<T>::at(int pos){
    if(pos >= 0 && pos <= filled){
        return array[pos];
    }
}

template<typename T>
void MyVector<T>::pop_back(){
    if(filled != 0){
        filled--;
    }
}

#endif // MYVECTOR_H
