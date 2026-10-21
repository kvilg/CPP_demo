#include <cstddef>

template <typename T>
class List {
public:
    List() = default;

    List(const List& other);

    List& operator=(const List& other);

    ~List();

    void add_last(const T& value);

    void add_start(const T& value);

    std::size_t getSize() const;
    const T& getValue(std::size_t index);

    void setValue(std::size_t index, const T& value);

    void swap(List& other) noexcept;

private:
    class Node {
    public:
        explicit Node(const T& value);

        ~Node() = default;

        void setValue(const T& value);

        const T& getValue() const;

        void setNext(Node* node);

        void setPrev(Node* node);

        Node* getNext() const;

        Node* getPrev() const;

    private:
        T m_value;
        Node* m_next = nullptr;
        Node* m_prev = nullptr;
    };

    Node* nodeAt(std::size_t index);

    std::size_t m_size = 0;
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
};
