#pragma once
#include <iostream>
#include <list.hpp>

template <class T>
class Queue : private List<T>{
public:
    Queue();
    Queue(const Queue & o);

    Queue(const Queue<T> & o, size_t startIndex);
    ~Queue();

    Queue & operator = (const Queue & o);

    void split( Queue<T> & newQueue);

    void enqueue(T newElement);
    T dequeue();
    const T & peek() const;

    size_t getSize() const;

    bool isEmpty() const;
};

template <class T>
Queue<T> :: Queue() : List<T>(){

}

template <class T>
Queue<T> :: Queue(const Queue & o) : List<T>(o){
    ;
}

template <class T>
Queue<T> :: Queue(const Queue<T> & o, size_t startIndex) : List<T> :: List(o, startIndex){
    ;
}

template <class T>
Queue<T> :: ~Queue(){
    ;
}

template <class T>
Queue<T> & Queue<T> :: operator = (const Queue<T> & o){
    List<T> :: operator =(o);
    return *this;
}

template <class T>
void Queue<T>::split(Queue<T> & newQueue){
    size_t middle = getSize() / 2;

    Queue<T> secondPart(*this, middle);

    size_t size = getSize();
    for(size_t i = middle; middle < size; ++middle){
        List<T> :: removeAt(i);
    }

    newQueue = secondPart;
}

template <class T>
size_t Queue<T> :: getSize() const{
    return List<T> :: getSize();
}

template <class T>
void Queue<T> :: enqueue(T newElement){
    List<T> :: addAt(getSize(), newElement);
}

template <class T>
T Queue<T> :: dequeue(){
    if (!getSize()){
        throw "empty queue";
    }

    return List<T> :: removeAt(0);
}


template <class T>
const T & Queue<T> :: peek() const{
    if (!getSize()){
        throw "empty queue";
    }

    return List<T> :: getAt(0);
}

template <class T>
bool Queue<T> :: isEmpty() const{
    return getSize() == 0;
}




