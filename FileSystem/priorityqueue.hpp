#pragma once
#include <queue.hpp>

template <class T>
class PriorityQueue : public Queue<T>{
public:
    void enqueue(T newElement);
    T dequeue();
    const T & peek() const;
    bool contains(const T & value) const;

    typename List<T> :: Iterator begin() const;
    typename List<T> :: Iterator end() const;
};

///FIX ME!
template <class T>
void PriorityQueue<T> :: enqueue(T newElement){
    size_t size = Queue<T> :: getSize();
    if(size == 0){
        Queue<T> :: enqueue(newElement);
        return;
    }

    for(size_t i = 0; i < size; ++i){
        if(List<T> :: getAt(i) > newElement){
            List<T> :: addAt(i, newElement);
            return;
        }
    }

    Queue<T> :: enqueue(newElement);
}


template <class T>
T PriorityQueue<T> :: dequeue(){
    return Queue<T> :: removeAt(0);
}

template <class T>
const T & PriorityQueue<T> :: peek() const{
    return Queue<T> :: operator [](0);
}

template <class T>
typename List<T> :: Iterator PriorityQueue<T> :: begin() const{
    return typename List<T> :: Iterator(List<T> :: first);
}

template <class T>
typename List<T> :: Iterator PriorityQueue<T> :: end() const{
    return List<T> :: Iterator();
}

template <class T>
bool PriorityQueue<T> :: contains(const T & element) const{
    for(typename PriorityQueue<T> :: Iterator it = begin(); it; it++){
        if(*it == element){
            return true;
        }
    }

    return false;
}
