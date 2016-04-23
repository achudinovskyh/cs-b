#include <iostream>
#include "mylinkedlist.h"
#include "mymap.h"
#include "mypriorityqueue.h"
#include "myqueue.h"
#include "myvector.h"

using namespace std;

int main()
{
    MyLinkedList<int> list;
    cout << "===== MyLinkedList test =====\n\n";

    list.push_back(3);
    list.push_back(4);
    list.push_back(5);
    list.push_front(2);
    list.push_front(1);

    if(!list.empty()){
        cout << "Not empty!!!\n";
    }

    cout << "Syze: " << list.size() << endl;
    cout << "Front: " << list.front() << endl;
    cout << "Back: "  << list.back() << endl;

    list.pop_front();
    list.pop_back();

    cout << "Front after pop_front: " << list.front() << endl;
    cout << "Back after pop_back: "  << list.back() << endl << endl;

//===========================================================================
    MyQueue<int> queue;
    cout << "===== MyQueue test =====\n\n";

    queue.push(1);
    queue.push(2);
    queue.push(3);
    queue.push(4);
    queue.push(5);

    if(!queue.empty()){
        cout << "Not empty!!!\n";
    }
    cout << "Syze: " << queue.size() << endl;
    cout << "Front: " << queue.front() << endl;
    cout << "Back: "  << queue.back() << endl;

    queue.pop();

    cout << "Front after pop: " << queue.front() << endl << endl;

//===========================================================================
    MyVector<int> vector;
    cout << "===== MyVector test =====\n\n";

    vector.push_back(1);
    vector.push_back(2);
    vector.push_back(3);
    vector.push_back(4);
    vector.push_back(5);

    cout << "Syze: " << vector.size() << endl;
    cout << "vector[0] " << vector[0] << endl;
    cout << "vector.at(4) " << vector.at(4) << endl;
    vector.pop_back();
    cout << "Syze: after pop_back" << vector.size() << endl << endl;

//===========================================================================
    MyPriorityQueue<int,string> prQue;
    cout << "===== MyPriorityQueue test =====\n\n";

    prQue.push(5,"five");
    prQue.push(3,"three");
    prQue.push(1,"one");
    prQue.push(2,"two");
    prQue.push(4,"four");

    if(!prQue.empty()){
        cout << "Not empty!!!\n";
    }
    cout << "Syze: " << prQue.size() << endl;
    cout << "top() " <<  prQue.top().first << " : "<< prQue.top().second << endl;
    prQue.pop();
    cout << "top() after pop() " <<  prQue.top().first << " : "<< prQue.top().second << endl << endl;

//===========================================================================

}

