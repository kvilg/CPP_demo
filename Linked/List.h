#pragma once

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
    // Только объявление, определение — в List.cpp. Снаружи Node не виден
    // даже как имя полей: m_head и m_tail это указатели, а указателю на
    // неполный тип полное определение не нужно.
    //
    // Для шаблона такое работает лишь потому, что единственное
    // инстанцирование List тоже лежит в List.cpp:
    //     template class List<int>;
    // Компилятору негде увидеть List<U> для другого U, а значит и Node
    // ему там не понадобится. Нужен List<std::string> — добавь рядом
    // ещё одну строку инстанцирования.
    class Node;

    Node* nodeAt(std::size_t index);

    std::size_t m_size = 0;
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
};
