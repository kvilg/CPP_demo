#include <iostream>
#include <vector>
#include <algorithm>
#include <adsprop.h>

template <typename T>
class Node{
private:
    T value;
    Node<T> *next;
    Node<T> *prev;

public:
    Node(const T &i) : value(i), next(nullptr), prev(nullptr) {

    }

    ~Node(){

    }

    void set(T t){
        this->value = t;
    }

    void setNext(Node<T> *t){
        this->next = t;
    }

    void setPrev(Node<T> *t) {
        this->prev = t;
    }

    Node<T>* get_next(){
        return next;
    }

    Node<T>* get_prev(){
        return prev;
    }

    T getValue() const {
        return value;
    }
};



template <typename T>
class List{
private:
    size_t size = 0;
    Node<T> *node_start;
    Node<T> *node_last;

    void setNodeStart(Node<T> *nodeStart) {
        node_start = nodeStart;
    }

    void setNodeLast(Node<T> *nodeLast) {
        node_last = nodeLast;
    }


public:
    List(): node_start(nullptr), node_last(nullptr){
        std::cout<<"List";
    }

    List(const List& other) : node_start(nullptr), node_last(nullptr) {

        Node<T>* cur = node_start;
        while (cur) {
            Node<T>* del = cur;
            cur = cur->get_next();
            delete del;
        }
        node_start = nullptr;
        node_last  = nullptr;
        size = 0;

        Node<T>* node = other.node_start;
        for (size_t i = 0; i < other.size; ++i) {
            add_last(node->getValue());   // ← если new бросит bad_alloc ЗДЕСЬ
            node = node->get_next();       //    на середине — что тогда?
        }

    }

    List& operator=(const List& other){
        if (this == &other) return *this;

        Node<T>* def = this->node_start;

        for (size_t i = 0; i < this->size; ++i) {
            Node<T>* del = def->get_next();
            delete def;
            def = del;
        }
        this->node_start = nullptr;
        this->node_last  = nullptr;
        this-> size = 0;

        Node<T>* node = other.getNodeStart();
        for (size_t i = 0; i < other.getSize(); ++i) {
            this->add_last(node->getValue());
            node = node->get_next();
        }

        return *this;
    }

    ~List(){
        Node<T>* def = this->node_start;

        for (size_t i = 0; i < this->size; ++i) {
            Node<T>* del = def->get_next();
            delete def;
            def = del;
        }
        this->node_start = nullptr;
        this->node_last  = nullptr;
        this->size = 0;

    }

    void add_last(const T &i){
        Node<T> *node = new Node<T>(i);
        if(this->size == 0){
            this->node_last = node;
            this->node_start = node;
            size++;
            return;
        }
        Node<T>* rto = this->getNodeLast();
        rto->setNext(node);
        this->setNodeLast(node);
        this->getNodeLast()->setPrev(rto);
        size++;
    }

    void add_start(const T &i){
        Node<T> *node = new Node<T>(i);
        if(size == 0){
            this->node_last = node;
            this->node_start = node;
            size++;
            return;
        }
        Node<T>* rto = this->getNodeStart();
        rto->setPrev(node);
        this->setNodeStart(node);
        this->getNodeStart()->setNext(rto);
        size++;
    }

    size_t getSize() const {
        return size;
    }

    Node<T>* getNodeStart() const {
        return node_start;
    }

    Node<T>* getNodeLast() {
        return node_last;
    }


    T get(const int &i){
        if (i < 0 || (size_t)i >= size)
            throw std::out_of_range("index out of range");

        Node<T>* root = getNodeStart();
        for (int j = 0; j < i; ++j)
            root = root->get_next();
        return root->getValue();
    }

    void set(const int &i,const T &t){
        Node<T>* root = getNodeStart();
        for (int j = 0; j < i; ++j)
            root = root->get_next();
        root->set(t);
    }

};

int main() {
    List<int> it;
    it.add_start(1);
    it.add_start(2);
    it.add_start(3);
    it.add_start(4);

    List<int> iii(it);

    std::cout<<it.get(0)<<"\n";
    std::cout<<iii.get(0)<<"\n";
    it.set(0,44);
    std::cout<<it.get(0)<<"\n";
    std::cout<<iii.get(0)<<"\n";

    List<int> a;
    a = iii;

    iii.set(1,1010);
    std::cout<<a.get(1)<<"\n";
    std::cout<<a.get(1)<<"\n";
    return 0;
}

