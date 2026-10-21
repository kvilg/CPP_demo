#pragma once

#include <compare>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>

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

public:
    List() = default;

    List(const List& other);

    List& operator=(const List& other);

    ~List();

    void addLast(const T& value);

    void addStart(const T& value);

    std::size_t getSize() const;

    // Быстрый доступ без проверок: индекс за границей — UB, как у
    // operator[] у вектора. Проверяющий вариант — at() ниже.
    //
    // Метод неконстантный, хотя возвращает const T&, и это расходится с
    // остальным классом: getSize, at, getStart и getLast — константные.
    // Так и задумано: константную версию нельзя написать, не заведя
    // const-версию nodeAt, а это часть упражнения с at(). Сделаешь at —
    // заодно пометь const и здесь.
    const T& getValue(std::size_t index);

    void setValue(std::size_t index, const T& value);

    void swap(List& other) noexcept;

    // Свободный swap, чтобы работал идиом
    //
    //     using std::swap;
    //     swap(a, b);
    //
    // Он называется std::swap two-step, «двухшаговый swap»: сначала
    // using втаскивает в область видимости std::swap как запасной
    // вариант, потом безквалификационный вызов даёт ADL выбрать более
    // подходящий, если он есть. Именно так свой swap зовут все
    // generic-функции в стандартной библиотеке.
    //
    // Здесь std::swap виден по using, но компилятор всё равно выберет
    // этот — его находит ADL, поиск по типам аргументов. Объявлен прямо
    // в классе (так называемый hidden friend): снаружи он виден только
    // через ADL и не засоряет пространство имён.
    // Почитать: https://en.cppreference.com/w/cpp/language/adl
    //
    // Заглушка не бросает, в отличие от остальных: функция noexcept, и
    // исключение из неё означало бы std::terminate — вместо красного
    // теста упал бы весь прогон.
    friend void swap(List& a, List& b) noexcept {
        // TODO: реализовать
        (void)a;
        (void)b;
    }

    // --- упражнения ---
    //
    // Объявлено, но не реализовано: тела в List.cpp либо бросают
    // std::logic_error, либо ничего не делают. Поэтому тесты с именами
    // "exercise: ..." падают — это и есть список задач.
    //
    // Прогнать только нерешённое:   ctest --preset debug -R exercise
    // Прогнать только то, что есть: ctest --preset debug -E exercise
    //
    // ОБЩЕЕ ПРАВИЛО ПРО ГРАНИЦЫ. Всё, что ниже, при выходе за границы и
    // на пустом списке бросает std::out_of_range. Это отличается от
    // std::list, где такие вызовы — UB: стандарт выбирает скорость, а
    // мы выбираем проверяемость, потому что на брошенное исключение
    // можно написать тест, а на UB нельзя. Исключения из правила два и
    // оба выше: getValue и setValue, они намеренно без проверок.

    // Правило пяти: деструктор, копирование и перемещение.
    // Забрать у other содержимое и оставить его пустым, но валидным.
    List(List&& other) noexcept;
    List& operator=(List&& other) noexcept;

    // Чтобы писать List<int> list{1, 2, 3} — и в тестах, и в main.
    // Обрати внимание: при пустых скобках List<int>{} вызовется всё-таки
    // конструктор по умолчанию, такое правило у списков инициализации.
    List(std::initializer_list<T> values);

    // Пара к конструктору выше, чтобы работало и list = {1, 2, 3}.
    // У std::list пара полная; держать только конструктор — значит
    // оставить API наполовину сделанным.
    List& operator=(std::initializer_list<T> values);

    bool isEmpty() const;

    // Освободить все узлы; список остаётся пригодным для вставки
    void clear();

    // На пустом списке — std::out_of_range
    void removeStart();
    void removeLast();

    // Вставить value перед элементом index; index == getSize() —
    // то же, что addLast. Больше getSize() — std::out_of_range.
    void insert(std::size_t index, const T& value);

    // index >= getSize() — std::out_of_range
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
    // На пустом списке — std::out_of_range.
    T& getStart();
    const T& getStart() const;

    T& getLast();
    const T& getLast() const;

    // Удалить ВСЕ элементы, равные value. Пустой список и отсутствие
    // совпадений — не ошибка, просто ничего не произойдёт.
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

    // Заменить всё содержимое. Старые узлы освободить.
    void assign(std::size_t count, const T& value);
    void assign(std::initializer_list<T> values);

    // Изменить длину: короче — лишнее удалить с конца, длиннее —
    // добить в конец. Версия без value добивает T{}, поэтому требует от
    // типа конструктор по умолчанию — ещё один повод для requires
    // именно на методе, а не на классе.
    void resize(std::size_t count)
        requires std::default_initializable<T>;
    void resize(std::size_t count, const T& value);

    // Удалить подряд идущие дубликаты, оставив по одному. Именно
    // подряд идущие: чтобы убрать все, список сначала сортируют.
    void unique()
        requires std::equality_comparable<T>;

    // Слить в себя другой отсортированный список, перецепив его узлы.
    // Копирования нет вообще, other остаётся пустым. Оба списка должны
    // быть отсортированы — иначе результат бессмысленный.
    void merge(List& other)
        requires std::totally_ordered<T>;

    // Сконструировать элемент прямо в узле, без промежуточной копии.
    // Отличие от addLast(T(args...)) в том, что временный объект не
    // создаётся: аргументы прокидываются в конструктор T как есть,
    // сохраняя константность и rvalue-ность. Приём называется
    // perfect forwarding.
    template <typename... Args>
        requires std::constructible_from<T, Args...>
    T& emplaceLast(Args&&... args);

    template <typename... Args>
        requires std::constructible_from<T, Args...>
    T& emplaceStart(Args&&... args);

    // --- итератор ---
    //
    // Нужен, чтобы заработало for (int x : list) и алгоритмы из
    // <algorithm>. Range-based for разворачивается примерно в
    //
    //     for (auto it = list.begin(); it != list.end(); ++it) {
    //         int x = *it;
    //         ...
    //     }
    //
    // то есть минимум от итератора нужно: operator*, operator++ и
    // сравнение. Остальное — чтобы он считался полноценным
    // двунаправленным итератором и его принимали алгоритмы.
    //
    // Категорию заявляем двунаправленной: список двусвязный, значит
    // умеет и назад. Отсюда и operator--.
    //
    // Константной версии здесь нет: по const List& пройтись не выйдет.
    // Это следующий шаг и почти дословная копия с заменой T& на
    // const T&, поэтому интереснее сначала сделать эту.
    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator() = default;

        // Конструктор публичный, но снаружи недоступен: Node — приватный
        // тип List, и указатель на него вне класса просто неоткуда взять.
        explicit Iterator(Node* node);

        reference operator*() const;
        pointer operator->() const;

        // Префиксный возвращает ссылку на себя, постфиксный — копию до
        // инкремента. Поэтому префиксный и дешевле: привычка писать ++it
        // вместо it++ в циклах растёт отсюда.
        Iterator& operator++();
        Iterator operator++(int);

        Iterator& operator--();
        Iterator operator--(int);

        // operator!= писать не нужно: в C++20 компилятор выводит его
        // из operator== сам.
        bool operator==(const Iterator& other) const;

    private:
        Node* m_node = nullptr;
    };

    // end() — это итератор ЗА последним элементом, а не на него.
    Iterator begin();
    Iterator end();

    // --- что доделать в самом классе ---
    //
    // Это не упражнения: наблюдаемого поведения у них нет, тестом такое
    // не поймать. Замечания к коду, который уже написан. (Свободный swap
    // сначала лежал здесь же — и зря: он как раз проверяем, поэтому
    // уехал в упражнения, к методу swap.)
    //
    // 1. Нигде нет [[nodiscard]]. Восемнадцать методов возвращают
    //    значение, и ни один не помечен. Из-за этого строка
    //
    //        list.isEmpty();
    //
    //    молча ничего не делает — типичная опечатка вместо if. Стоит
    //    пройтись по getSize, isEmpty, at, getValue, getStart, getLast,
    //    begin, end, operator== и расставить. В стандартной библиотеке
    //    так и сделано: у std::list помечены и front(), и back() —
    //    отбросишь их результат, компилятор предупредит.
    //    Почитать:
    //    https://en.cppreference.com/w/cpp/language/attributes/nodiscard
    //
    // 2. В List.cpp у Node объявлен ~Node() = default. Он ничего не
    //    меняет, зато из-за него clang-tidy требует у Node и остальные
    //    методы правила пяти. Удалить — предупреждение уйдёт само.
    //
    // 3. У Node шесть аксессоров вокруг трёх полей. Node приватный и
    //    спрятан в .cpp, снаружи его не видно по построению, так что
    //    защищать нечего. Обычный struct с полями value, next и prev
    //    сделал бы код List короче: node->next вместо node->getNext().

private:
    Node* nodeAt(std::size_t index);

    std::size_t m_size = 0;
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
};
