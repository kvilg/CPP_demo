


#include <utility>
#include "List.h"

template <typename T>
void List<T>::Node::setValue(const T& value) {
    m_value = value;
}

template <typename T>
const T& List<T>::Node::getValue() const {
    return m_value;
}

template <typename T>
void List<T>::Node::setNext(Node* node) {
    m_next = node;
}

template <typename T>
void List<T>::Node::setPrev(Node* node) {
    m_prev = node;
}

template <typename T>
typename List<T>::Node* List<T>::Node::getNext() const {
    return m_next;
}

template <typename T>
typename List<T>::Node* List<T>::Node::getPrev() const {
    return m_prev;
}

template<typename T>
List<T>::Node::Node(const T &value) {
    m_value=value;
}

template <typename T>
typename List<T>::Node* List<T>::nodeAt(std::size_t index) {
    Node* cur = m_head;
    for (std::size_t i = 0; i < index && cur != nullptr; ++i) {
        cur = cur->getNext();
    }
    return cur;
}


template <typename T>
List<T>::List(const List& other) {
    List tmp;
    for (Node* n = other.m_head; n != nullptr; n = n->getNext()) {
        tmp.add_last(n->getValue());
    }
    swap(tmp);
}

template <typename T>
List<T>& List<T>::operator=(const List& other) {
    if (this != &other) {
        List tmp(other);
        swap(tmp);
    }
    return *this;
}

template <typename T>
List<T>::~List() {
    Node* cur = m_head;
    while (cur != nullptr) {
        Node* next = cur->getNext();
        delete cur;
        cur = next;
    }
}


template <typename T>
void List<T>::add_last(const T& value) {
    Node *node = new Node(value);
    node->setPrev(m_tail);

    if (m_tail != nullptr) {
        m_tail->setNext(node);
    } else {
        m_head = node;
    }
    m_tail = node;
    ++m_size;
}

template <typename T>
void List<T>::add_start(const T& value) {
    Node* node = new Node(value);
    node->setNext(m_head);

    if (m_head != nullptr) {
        m_head->setPrev(node);
    }
    else {
        m_tail = node;
    }

    m_head = node;
    ++m_size;
}


template <typename T>
std::size_t List<T>::getSize() const {
    return m_size;
}

template <typename T>
const T & List<T>::getValue(std::size_t index) {
    return nodeAt(index)->getValue();
}

template <typename T>
void List<T>::setValue(std::size_t index, const T& value) {
    nodeAt(index)->setValue(value);
}


template <typename T>
void List<T>::swap(List& other) noexcept {
    std::swap(m_size, other.m_size);
    std::swap(m_head, other.m_head);
    std::swap(m_tail, other.m_tail);
}

template class List<int>;