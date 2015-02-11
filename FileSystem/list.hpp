#pragma once
#include <iostream>
#include <node.hpp>

template <class T>
class List{
    Node<T> * first;
    Node<T> * last;

    size_t size;

    Node<T> * getNodeAt(int index);
public:
    List();
    List(const List & o);
    List(const List & o, size_t startIndex);
    ~List();

    List & operator = (const List & o);
    const T & operator [](int index) const;
    T & operator [](int index);

    void addAt(int index, T newElement);

    T removeAt(int index);
    const T & getAt(int index) const;



    size_t getSize() const;
    bool isEmpty() const;

    class Iterator{
        Node<T> * current;
    public:
        Iterator();
        Iterator(Node<T> * newCurrent);

        Iterator & operator++(int uselessVar);

        Iterator & operator++();

        T operator *();

        bool operator ==(const Iterator & r) const;
        operator bool() const;
    };

    Iterator begin() const;
    Iterator end() const;
};


template <class T>
List<T> :: List(){
    first = NULL;
    last = NULL;
    size = 0;
}

template <class T>
List<T> :: List(const List & o){
    if(&o == this){  //check for self-assignment
        throw "invalid assignment";
    }

    first = NULL;
    last = NULL;
    size = 0;

    size_t oSize = o.getSize();

    for(size_t i = 0; i < oSize; ++i){
        addAt(i, o.getAt(i));
    }
}

template <class T>
List<T> :: List(const List & o, size_t startIndex){
    if(&o == this){  //check for self-assignment
        throw "invalid assignment";
    }

    first = NULL;
    last = NULL;
    size = 0;

    size_t oSize = o.getSize();

    for(size_t i = startIndex; i < oSize; ++i){
        addAt(size, o.getAt(i));
    }
}

template <class T>
List<T> :: ~List(){
    delete first;
}

template <class T>
List<T> & List<T> :: operator = (const List<T> & o){
    if(this == &o){  //check for self-assignment
        return * this;
    }

    //destroy current data
    delete first;
    size = 0;

    size_t oSize = o.getSize();

    for(size_t i = 0; i < oSize; ++i){
        addAt(i, o.getAt(i));
    }

    return * this;
}

template <class T>
const T & List<T> :: operator [](int index) const{
    return getAt(index);
}

template <class T>
T & List<T> :: operator [](int index){
    return const_cast<T &>(getAt(index));
}

template <class T>
void List<T> :: addAt(int index, T newElement){
    if(index < 0 || index > size + 1){
        throw "invalid index";
    }

    Node<T> * newNode = new Node<T>(newElement, NULL);

    if(index == 0){
        if(size == 0){
            first = last = newNode;
        }else{
            newNode->setNext(first);
            first = newNode;
        }
    }else{
        Node<T> * beforeNode = getNodeAt(index - 1);
        newNode->setNext(beforeNode->getNext());

        beforeNode->setNext(newNode);
    }

    ++size;
}

template <class T>
T List<T> :: removeAt(int index){
    if(index < 0 || index > size){
        throw "invalid index";
    }

    if(index == 0){
        Node<T> * afterNode = first->getNext();
        first->setNext(NULL);
        T value = first->getValue();
        delete first;
        first = afterNode;
        size--;

        return value;
    }else{
        Node<T> * beforeNode = first;

        for(int i = 0; i < index - 1; ++i){
            beforeNode = beforeNode->getNext();
        }

        T value = beforeNode->getNext()->getValue();

        Node<T> * afterNode = beforeNode->getNext()->getNext();
        beforeNode->getNext()->setNext(NULL);


        delete beforeNode->getNext();

        beforeNode->setNext(afterNode);
        --size;

        return value;
    }
}

template <class T>
const T & List<T> :: getAt(int index) const{
    if (index < 0 || index > size){
        throw "invalid index";
    }

    if(index == 0){
        return first->getValue();
    }else{
        Node<T> * node = first;

        for(int i = 0; i < index; ++i){
            node = node->getNext();
        }

        return node->getValue();
    }
}

template <class T>
Node<T> * List<T>::getNodeAt(int index){
    if (index < 0 || index > size){
        throw "invalid index";
    }

    if(index == 0){
        return first;
    }else{
        Node<T> * node = first;

        for(int i = 0; i < index; ++i){
            node = node->getNext();
        }

        return node;
    }
}

template <class T>
bool List<T> :: isEmpty() const{
    return size == 0;
}

template <class T>
size_t List<T> :: getSize() const{
    return size;
}

template <class T>
typename List<T> :: Iterator List<T> :: begin() const{
    return Iterator(first);
}

template <class T>
typename List<T> :: Iterator List<T> :: end() const{
    return Iterator();
}


template <class T>
List<T> :: Iterator :: Iterator(){
    current = NULL;
}

template <class T>
List<T> :: Iterator :: Iterator(Node<T> * newCurrent){
    current = newCurrent;
}

template <class T>
typename List<T> :: Iterator & List<T> :: Iterator :: operator ++(int uselessVar){
    ++uselessVar;


    current = current->getNext();

    return *(this);
}

template <class T>
typename List<T> :: Iterator & List<T> :: Iterator :: operator ++(){
    ++(*this);
}

template <class T>
T List<T> :: Iterator ::operator *(){
    return current->getValue();
}

template <class T>
bool List<T> :: Iterator :: operator ==(const List<T> :: Iterator & rh) const{
    return current == rh.current;
}

template <class T>
List<T> :: Iterator :: operator bool() const{
    return current != NULL;
}
