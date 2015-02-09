#pragma once

template <class T>
class Node{
    Node * next;
    T value;
public:
    Node();
    Node(T newValue, Node * newNext);
    ~Node();

    Node * getNext() const;
    void setNext(Node * newNext);

    const T & getValue() const;
    void setValue(T newValue);
};

template <class T>
Node<T> :: Node(){
    next = NULL;
}

template <class T>
Node<T> :: Node(T newValue, Node * newNext){
    value = newValue;
    next = newNext;

}

template <class T>
Node<T> :: ~Node(){
    delete next;
    next = NULL;
}

template <class T>
Node<T> * Node<T> :: getNext() const{
    return next;
}

template <class T>
void Node<T> :: setNext(Node<T> * newNext){
    next = newNext;
}

template <class T>
const T & Node<T> :: getValue() const{
    return value;
}

template <class T>
void Node<T> :: setValue(T newValue){
    value = newValue;
}
