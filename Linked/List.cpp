#include "List.h"
#include <stdexcept>
#include <utility>

#include <doctest/doctest.h>

// Определение Node. В List.h от него осталось только `class Node;` —
// наружу утекает лишь имя, а устройство узла (значение и две ссылки)
// видно только здесь.
template <typename T>
class List<T>::Node {
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

template <typename T>
List<T>::Node::Node(const T& value) {
    m_value = value;
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
    Node* node = new Node(value);
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
    } else {
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
const T& List<T>::getValue(std::size_t index) {
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

// --- упражнения: заглушки ---
//
// Реализовать и довести до зелёного тесты "exercise: ..." ниже.
//
// (void) перед throw — чтобы -Wunused-parameter с -Werror не завалил
// сборку на неиспользованном аргументе. Когда напишешь тело, эти строки
// удаляются.

template <typename T>
List<T>::List(List&& other) noexcept {
    // TODO: реализовать
    (void)other;
}

template <typename T>
List<T>& List<T>::operator=(List&& other) noexcept {
    // TODO: реализовать
    (void)other;
    return *this;
}

template <typename T>
bool List<T>::isEmpty() const {
    throw std::logic_error("List::isEmpty не реализован");
}

template <typename T>
void List<T>::clear() {
    throw std::logic_error("List::clear не реализован");
}

template <typename T>
void List<T>::remove_start() {
    throw std::logic_error("List::remove_start не реализован");
}

template <typename T>
void List<T>::remove_last() {
    throw std::logic_error("List::remove_last не реализован");
}

template <typename T>
void List<T>::insert(std::size_t index, const T& value) {
    (void)index;
    (void)value;
    throw std::logic_error("List::insert не реализован");
}

template <typename T>
void List<T>::removeAt(std::size_t index) {
    (void)index;
    throw std::logic_error("List::removeAt не реализован");
}

template <typename T>
const T& List<T>::at(std::size_t index) const {
    (void)index;
    throw std::logic_error("List::at не реализован");
}

template <typename T>
bool List<T>::operator==(const List& other) const {
    (void)other;
    throw std::logic_error("List::operator== не реализован");
}

template class List<int>;

// --- тесты ---
//
// Лежат рядом с реализацией: так задуман doctest. В цели Linked они
// вырезаются препроцессором (DOCTEST_CONFIG_DISABLE), в бинарник не
// попадают; цель Linked_tests собирает этот же файл с тестами.
//
// Проверяется только List<int>: выше явно инстанцирован лишь он.
// Доступ к приватному Node у TEST_CASE нет — это свободные функции,
// а не члены класса; для тестов на внутренности нужен friend в List.h.

TEST_CASE("новый список пуст") {
    List<int> list;

    CHECK(list.getSize() == 0);
}

TEST_CASE("add_last добавляет в конец") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);
    list.add_last(3);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("add_start добавляет в начало") {
    List<int> list;
    list.add_start(1);
    list.add_start(2);
    list.add_start(3);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 3);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 1);
}

TEST_CASE("add_start и add_last работают вместе") {
    List<int> list;
    list.add_last(2);
    list.add_start(1);
    list.add_last(3);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("add_last в пустой список задаёт и голову, и хвост") {
    List<int> list;
    list.add_last(42);

    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 42);

    // если хвост не выставлен, второй add_last потеряет элемент
    list.add_last(43);
    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(1) == 43);
}

TEST_CASE("add_start в пустой список задаёт и голову, и хвост") {
    List<int> list;
    list.add_start(42);

    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 42);

    list.add_last(43);
    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(1) == 43);
}

TEST_CASE("setValue меняет элемент на месте") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);
    list.add_last(3);

    list.setValue(1, 20);

    CHECK(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 20);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("копирующий конструктор сохраняет порядок") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);
    list.add_last(3);

    List<int> copy(list);

    REQUIRE(copy.getSize() == 3);
    CHECK(copy.getValue(0) == 1);
    CHECK(copy.getValue(1) == 2);
    CHECK(copy.getValue(2) == 3);
}

TEST_CASE("копия независима от оригинала") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);

    List<int> copy(list);

    list.setValue(0, 99);

    CHECK(list.getValue(0) == 99);
    CHECK(copy.getValue(0) == 1);

    // и в обратную сторону
    copy.setValue(1, 77);
    CHECK(copy.getValue(1) == 77);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("копия пустого списка пуста") {
    List<int> empty;
    List<int> copy(empty);

    CHECK(copy.getSize() == 0);

    // копия пустого списка остаётся пригодной для вставки
    copy.add_last(1);
    REQUIRE(copy.getSize() == 1);
    CHECK(copy.getValue(0) == 1);
    CHECK(empty.getSize() == 0);
}

TEST_CASE("присваивание заменяет содержимое") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);
    list.add_last(3);

    List<int> other;
    other.add_last(9);

    other = list;

    REQUIRE(other.getSize() == 3);
    CHECK(other.getValue(0) == 1);
    CHECK(other.getValue(1) == 2);
    CHECK(other.getValue(2) == 3);
}

TEST_CASE("после присваивания списки независимы") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);

    List<int> other;
    other = list;

    other.setValue(0, 99);

    CHECK(other.getValue(0) == 99);
    CHECK(list.getValue(0) == 1);
}

TEST_CASE("присваивание пустого списка очищает получателя") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);

    List<int> empty;
    list = empty;

    CHECK(list.getSize() == 0);
}

TEST_CASE("присваивание самому себе не портит список") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);

    // через ссылку, чтобы не ловить -Wself-assign-overloaded
    List<int>& alias = list;
    list = alias;

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("swap обменивает содержимое и размеры") {
    List<int> a;
    a.add_last(1);
    a.add_last(2);

    List<int> b;
    b.add_last(7);

    a.swap(b);

    REQUIRE(a.getSize() == 1);
    CHECK(a.getValue(0) == 7);

    REQUIRE(b.getSize() == 2);
    CHECK(b.getValue(0) == 1);
    CHECK(b.getValue(1) == 2);
}

TEST_CASE("swap с пустым списком") {
    List<int> filled;
    filled.add_last(1);

    List<int> empty;

    filled.swap(empty);

    CHECK(filled.getSize() == 0);
    REQUIRE(empty.getSize() == 1);
    CHECK(empty.getValue(0) == 1);

    // оба списка после swap пригодны для вставки
    filled.add_last(5);
    empty.add_last(6);
    CHECK(filled.getValue(0) == 5);
    CHECK(empty.getValue(1) == 6);
}

TEST_CASE("swap сам с собой ничего не меняет") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);

    list.swap(list);

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("список держит много элементов в правильном порядке") {
    List<int> list;
    for (int i = 0; i < 100; ++i) {
        list.add_last(i);
    }

    REQUIRE(list.getSize() == 100);
    CHECK(list.getValue(0) == 0);
    CHECK(list.getValue(50) == 50);
    CHECK(list.getValue(99) == 99);
}

// --- упражнения: тесты, которые должны стать зелёными ---
//
// Сейчас они падают: методы объявлены, но не реализованы. Это и есть
// задание — реализовать и добиться зелёного.
//
// Только эти:    ctest --preset debug -R exercise
// Только готовые: ctest --preset debug -E exercise

TEST_CASE("exercise: isEmpty") {
    List<int> list;
    CHECK(list.isEmpty());

    list.add_last(1);
    CHECK_FALSE(list.isEmpty());
}

TEST_CASE("exercise: clear освобождает список") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);

    list.clear();

    CHECK(list.getSize() == 0);

    // после clear список пригоден для вставки
    list.add_last(3);
    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 3);
}

TEST_CASE("exercise: remove_start убирает первый элемент") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);
    list.add_last(3);

    list.remove_start();

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 2);
    CHECK(list.getValue(1) == 3);
}

TEST_CASE("exercise: remove_start на списке из одного элемента") {
    List<int> list;
    list.add_last(1);

    list.remove_start();

    REQUIRE(list.getSize() == 0);

    // хвост тоже должен обнулиться, иначе add_last потеряет элемент
    list.add_last(2);
    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 2);
}

TEST_CASE("exercise: remove_last убирает последний элемент") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);
    list.add_last(3);

    list.remove_last();

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("exercise: remove_last на списке из одного элемента") {
    List<int> list;
    list.add_last(1);

    list.remove_last();

    REQUIRE(list.getSize() == 0);

    // голова тоже должна обнулиться
    list.add_start(2);
    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 2);
}

TEST_CASE("exercise: insert в начало") {
    List<int> list;
    list.add_last(2);
    list.add_last(3);

    list.insert(0, 1);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("exercise: insert в середину") {
    List<int> list;
    list.add_last(1);
    list.add_last(3);

    list.insert(1, 2);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("exercise: insert по индексу getSize равносилен add_last") {
    List<int> list;
    list.add_last(1);

    list.insert(list.getSize(), 2);

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("exercise: insert в пустой список") {
    List<int> list;

    list.insert(0, 1);

    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 1);

    // и голова, и хвост должны смотреть на новый узел
    list.add_last(2);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("exercise: removeAt убирает элемент по индексу") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);
    list.add_last(3);

    list.removeAt(1);

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 3);
}

TEST_CASE("exercise: at возвращает значение") {
    List<int> list;
    list.add_last(1);
    list.add_last(2);

    CHECK(list.at(0) == 1);
    CHECK(list.at(1) == 2);
}

TEST_CASE("exercise: at работает на константном списке") {
    List<int> list;
    list.add_last(42);

    const List<int>& ref = list;
    CHECK(ref.at(0) == 42);
}

TEST_CASE("exercise: at за границей бросает out_of_range") {
    List<int> list;
    list.add_last(1);

    CHECK_THROWS_AS(list.at(1), std::out_of_range);
    CHECK_THROWS_AS(list.at(100), std::out_of_range);

    const List<int> empty;
    CHECK_THROWS_AS(empty.at(0), std::out_of_range);
}

TEST_CASE("exercise: move-конструктор забирает содержимое") {
    List<int> source;
    source.add_last(1);
    source.add_last(2);

    List<int> moved(std::move(source));

    REQUIRE(moved.getSize() == 2);
    CHECK(moved.getValue(0) == 1);
    CHECK(moved.getValue(1) == 2);

    // источник остаётся пустым, но валидным
    CHECK(source.getSize() == 0);
    source.add_last(3);
    CHECK(source.getValue(0) == 3);
}

TEST_CASE("exercise: move-присваивание забирает содержимое") {
    List<int> source;
    source.add_last(1);
    source.add_last(2);

    List<int> target;
    target.add_last(9);

    target = std::move(source);

    REQUIRE(target.getSize() == 2);
    CHECK(target.getValue(0) == 1);
    CHECK(target.getValue(1) == 2);
    CHECK(source.getSize() == 0);
}

TEST_CASE("exercise: operator== сравнивает содержимое") {
    List<int> a;
    a.add_last(1);
    a.add_last(2);

    List<int> b;
    b.add_last(1);
    b.add_last(2);

    CHECK(a == b);

    b.setValue(1, 99);
    CHECK_FALSE(a == b);

    List<int> shorter;
    shorter.add_last(1);
    CHECK_FALSE(a == shorter);

    List<int> empty1;
    List<int> empty2;
    CHECK(empty1 == empty2);
}
