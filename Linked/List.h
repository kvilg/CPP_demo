#pragma once

#include <compare>
#include <concepts>
#include <cstddef>
#include <initializer_list>

// Что List требует от элемента всегда: уметь копироваться и
// присваиваться. Больше ничего — ни сравнения, ни порядка.
//
// У std::list такого нет: стандартная библиотека описывает требования к
// элементу словами (Cpp17CopyInsertable и прочие), а в коде их не
// проверяет — концепты в C++20 получили ranges и алгоритмы, но не
// контейнеры, иначе сломалась бы совместимость. Нам же концепт полезен
// ровно тем, что превращает простыню ошибок из глубины шаблона в одну
// внятную строку.
template <typename T>
concept ListElement = std::copy_constructible<T> && std::assignable_from<T&, const T&>;

template <ListElement T>
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

    // Сравнение требует от T равенства — но только оно, а не весь класс.
    // Тип без operator== должен спокойно жить в списке, пока этот метод
    // не позвали; поэтому requires висит на методе, а не на классе.
    bool operator==(const List& other) const
        requires std::equality_comparable<T>;

    // Первый и последний элементы. Обязаны работать за O(1).
    // По неконстантной версии можно и присваивать:
    //     list.getStart() = 5;
    T& getStart();
    const T& getStart() const;

    T& getLast();
    const T& getLast() const;

    // Удалить ВСЕ элементы, равные value. Пустой список и отсутствие
    // если удаляется первый или последний.
    void removeValue(const T& value)
        requires std::equality_comparable<T>;

    // Развернуть список, перецепив узлы. Значения при этом не копируются
    // и не меняются — именно поэтому у списка своя операция, а не
    // свободный std::reverse, который переставляет значения обменом.
    void reverse();

    // Отсортировать по возрастанию. std::sort сюда не подходит:
    // ему нужны итераторы произвольного доступа, которых у связного
    // списка нет и быть не может.
    void sort()
        requires std::totally_ordered<T>;

    // C++20: описываешь одно сравнение, а <, >, <=, >= компилятор
    // выводит из него сам. Порядок лексикографический, как у строк.
    //
    // Категория не прибита к strong_ordering, а выводится из T:
    // у int сравнение даёт строгий порядок, а у double — частичный,
    // потому что NaN не равен ничему, включая себя. Прибей мы здесь
    // strong_ordering — и List<double> остался бы без сравнения вовсе.
    //
    // std::list идёт ещё дальше: его synth-three-way умеет собрать
    // категорию даже из одного operator<, если <=> у типа нет. Нам
    // столько не нужно, достаточно потребовать <=>.
    std::compare_three_way_result_t<T> operator<=>(const List& other) const
        requires std::three_way_comparable<T>;

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
