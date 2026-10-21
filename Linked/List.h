#pragma once

#include <compare>
#include <cstddef>
#include <initializer_list>

template <typename T>
class List {
public:
    List() = default;

    List(const List& other);

    List& operator=(const List& other);

    ~List();

    void addLast(const T& value);

    void addStart(const T& value);

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

    // Чтобы писать List<int> list{1, 2, 3} — и в тестах, и в main.
    // Обрати внимание: при пустых скобках List<int>{} вызовется всё-таки
    // конструктор по умолчанию, такое правило у списков инициализации.
    List(std::initializer_list<T> values);

    bool isEmpty() const;

    // Освободить все узлы; список остаётся пригодным для вставки
    void clear();

    void removeStart();
    void removeLast();

    // Вставить value перед элементом index; index == getSize() —
    // то же, что addLast
    void insert(std::size_t index, const T& value);

    void removeAt(std::size_t index);

    // В отличие от getValue проверяет границы и бросает
    // std::out_of_range. И, в отличие от него же, const.
    const T& at(std::size_t index) const;

    bool operator==(const List& other) const;

    // Первый и последний элементы. Обязаны работать за O(1).
    // По неконстантной версии можно и присваивать:
    //     list.getStart() = 5;
    T& getStart();
    const T& getStart() const;

    T& getLast();
    const T& getLast() const;

    // Удалить ВСЕ элементы, равные value. Пустой список и отсутствие
    // если удаляется первый или последний.
    void removeValue(const T& value);

    // Развернуть список, перецепив узлы. Значения при этом не копируются
    // и не меняются — именно поэтому у списка своя операция, а не
    // свободный std::reverse, который переставляет значения обменом.
    void reverse();

    // Отсортировать по возрастанию. std::sort сюда не подходит:
    // ему нужны итераторы произвольного доступа, которых у связного
    // списка нет и быть не может.
    void sort();

    // C++20: описываешь одно сравнение, а <, >, <=, >= компилятор
    // выводит из него сам. Порядок лексикографический, как у строк.
    std::strong_ordering operator<=>(const List& other) const;

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
