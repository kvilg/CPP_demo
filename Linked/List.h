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

    // --- упражнения ---
    //
    // Объявлено, но не реализовано: тела в List.cpp либо бросают
    // std::logic_error, либо ничего не делают. Поэтому тесты с именами
    // "exercise: ..." падают — это и есть список задач.
    //
    // Прогнать только нерешённое:   ctest --preset debug -R exercise
    // Прогнать только то, что есть: ctest --preset debug -E exercise

    // Правило пяти: деструктор, копирование и перемещение.
    // Забрать у other содержимое и оставить его пустым, но валидным.
    List(List&& other) noexcept;
    List& operator=(List&& other) noexcept;

    bool isEmpty() const;

    // Освободить все узлы; список остаётся пригодным для вставки
    void clear();

    void remove_start();
    void remove_last();

    // Вставить value перед элементом index; index == getSize() —
    // то же, что add_last
    void insert(std::size_t index, const T& value);

    void removeAt(std::size_t index);

    // В отличие от getValue проверяет границы и бросает
    // std::out_of_range. И, в отличие от него же, const.
    const T& at(std::size_t index) const;

    bool operator==(const List& other) const;

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
