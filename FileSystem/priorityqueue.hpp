#pragma once
#include <queue.hpp>

template <class T>
class PriorityQueue : public Queue<T>{
public:
    void enqueue(T newElement);
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
