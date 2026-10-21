#include "List.h"
#include <stdexcept>
#include <utility>

#include <doctest/doctest.h>

// Определение Node. В List.h от него осталось только `class Node;` —
// наружу утекает лишь имя, а устройство узла (значение и две ссылки)
// видно только здесь.
template <ListElement T>
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

template <ListElement T>
void List<T>::Node::setValue(const T& value) {
    m_value = value;
}

template <ListElement T>
const T& List<T>::Node::getValue() const {
    return m_value;
}

template <ListElement T>
void List<T>::Node::setNext(Node* node) {
    m_next = node;
}

template <ListElement T>
void List<T>::Node::setPrev(Node* node) {
    m_prev = node;
}

template <ListElement T>
typename List<T>::Node* List<T>::Node::getNext() const {
    return m_next;
}

template <ListElement T>
typename List<T>::Node* List<T>::Node::getPrev() const {
    return m_prev;
}

template <ListElement T>
List<T>::Node::Node(const T& value) : m_value(value) {}

template <ListElement T>
typename List<T>::Node* List<T>::nodeAt(std::size_t index) {
    Node* cur = m_head;
    for (std::size_t i = 0; i < index && cur != nullptr; ++i) {
        cur = cur->getNext();
    }
    return cur;
}

template <ListElement T>
List<T>::List(const List& other) {
    List tmp;
    for (Node* n = other.m_head; n != nullptr; n = n->getNext()) {
        tmp.addLast(n->getValue());
    }
    swap(tmp);
}

template <ListElement T>
List<T>& List<T>::operator=(const List& other) {
    if (this != &other) {
        List tmp(other);
        swap(tmp);
    }
    return *this;
}

template <ListElement T>
List<T>::~List() {
    Node* cur = m_head;
    while (cur != nullptr) {
        Node* next = cur->getNext();
        delete cur;
        cur = next;
    }
}

template <ListElement T>
void List<T>::addLast(const T& value) {
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

template <ListElement T>
void List<T>::addStart(const T& value) {
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

template <ListElement T>
std::size_t List<T>::getSize() const {
    return m_size;
}

template <ListElement T>
const T& List<T>::getValue(std::size_t index) {
    return nodeAt(index)->getValue();
}

template <ListElement T>
void List<T>::setValue(std::size_t index, const T& value) {
    nodeAt(index)->setValue(value);
}

template <ListElement T>
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

template <ListElement T>
List<T>::List(List&& other) noexcept {
    // TODO: реализовать
    (void)other;
}

template <ListElement T>
List<T>& List<T>::operator=(List&& other) noexcept {
    // TODO: реализовать
    (void)other;
    return *this;
}

template <ListElement T>
List<T>::List(std::initializer_list<T> values) {
    (void)values;
    throw std::logic_error("List::List(initializer_list) не реализован");
}

template <ListElement T>
bool List<T>::isEmpty() const {
    throw std::logic_error("List::isEmpty не реализован");
}

template <ListElement T>
void List<T>::clear() {
    throw std::logic_error("List::clear не реализован");
}

template <ListElement T>
void List<T>::removeStart() {
    throw std::logic_error("List::removeStart не реализован");
}

template <ListElement T>
void List<T>::removeLast() {
    throw std::logic_error("List::removeLast не реализован");
}

template <ListElement T>
void List<T>::insert(std::size_t index, const T& value) {
    (void)index;
    (void)value;
    throw std::logic_error("List::insert не реализован");
}

template <ListElement T>
void List<T>::removeAt(std::size_t index) {
    (void)index;
    throw std::logic_error("List::removeAt не реализован");
}

template <ListElement T>
const T& List<T>::at(std::size_t index) const {
    (void)index;
    throw std::logic_error("List::at не реализован");
}

template <ListElement T>
bool List<T>::operator==(const List& other) const
    requires std::equality_comparable<T>
{
    (void)other;
    throw std::logic_error("List::operator== не реализован");
}

template <ListElement T>
T& List<T>::getStart() {
    throw std::logic_error("List::getStart не реализован");
}

template <ListElement T>
const T& List<T>::getStart() const {
    throw std::logic_error("List::getStart (const) не реализован");
}

template <ListElement T>
T& List<T>::getLast() {
    throw std::logic_error("List::getLast не реализован");
}

template <ListElement T>
const T& List<T>::getLast() const {
    throw std::logic_error("List::getLast (const) не реализован");
}

template <ListElement T>
void List<T>::removeValue(const T& value)
    requires std::equality_comparable<T>
{
    (void)value;
    throw std::logic_error("List::removeValue не реализован");
}

template <ListElement T>
void List<T>::reverse() {
    throw std::logic_error("List::reverse не реализован");
}

template <ListElement T>
void List<T>::sort()
    requires std::totally_ordered<T>
{
    throw std::logic_error("List::sort не реализован");
}

template <ListElement T>
std::compare_three_way_result_t<T> List<T>::operator<=>(const List& other) const
    requires std::three_way_comparable<T>
{
    (void)other;
    throw std::logic_error("List::operator<=> не реализован");
}

template <ListElement T>
void List<T>::assign(std::size_t count, const T& value) {
    (void)count;
    (void)value;
    throw std::logic_error("List::assign (count, value) не реализован");
}

template <ListElement T>
void List<T>::assign(std::initializer_list<T> values) {
    (void)values;
    throw std::logic_error("List::assign (initializer_list) не реализован");
}

template <ListElement T>
void List<T>::resize(std::size_t count)
    requires std::default_initializable<T>
{
    (void)count;
    throw std::logic_error("List::resize (count) не реализован");
}

template <ListElement T>
void List<T>::resize(std::size_t count, const T& value) {
    (void)count;
    (void)value;
    throw std::logic_error("List::resize (count, value) не реализован");
}

template <ListElement T>
void List<T>::unique()
    requires std::equality_comparable<T>
{
    throw std::logic_error("List::unique не реализован");
}

template <ListElement T>
void List<T>::merge(List& other)
    requires std::totally_ordered<T>
{
    (void)other;
    throw std::logic_error("List::merge не реализован");
}

template <ListElement T>
template <typename... Args>
    requires std::constructible_from<T, Args...>
T& List<T>::emplaceLast(Args&&... args) {
    // Свёртка по запятой: раскрывается в ((void) a1, (void) a2, ...) и
    // упоминает каждый аргумент. sizeof...(args) здесь не годится — он
    // спрашивает только размер пачки, а сами параметры остаются
    // неиспользованными, и MSVC выдаёт на них C4100.
    ((void)args, ...);
    throw std::logic_error("List::emplaceLast не реализован");
}

template <ListElement T>
template <typename... Args>
    requires std::constructible_from<T, Args...>
T& List<T>::emplaceStart(Args&&... args) {
    ((void)args, ...);
    throw std::logic_error("List::emplaceStart не реализован");
}

// --- итератор ---
//
// Конструктор настоящий, он тут ничего не решает. Всё остальное —
// заглушки, и это и есть упражнение.

template <ListElement T>
List<T>::Iterator::Iterator(Node* node) : m_node(node) {}

template <ListElement T>
T& List<T>::Iterator::operator*() const {
    throw std::logic_error("List::Iterator::operator* не реализован");
}

template <ListElement T>
T* List<T>::Iterator::operator->() const {
    throw std::logic_error("List::Iterator::operator-> не реализован");
}

template <ListElement T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
    throw std::logic_error("List::Iterator::operator++ не реализован");
}

template <ListElement T>
typename List<T>::Iterator List<T>::Iterator::operator++(int) {
    throw std::logic_error("List::Iterator::operator++(int) не реализован");
}

template <ListElement T>
typename List<T>::Iterator& List<T>::Iterator::operator--() {
    throw std::logic_error("List::Iterator::operator-- не реализован");
}

template <ListElement T>
typename List<T>::Iterator List<T>::Iterator::operator--(int) {
    throw std::logic_error("List::Iterator::operator--(int) не реализован");
}

template <ListElement T>
bool List<T>::Iterator::operator==(const Iterator& other) const {
    (void)other;
    throw std::logic_error("List::Iterator::operator== не реализован");
}

template <ListElement T>
typename List<T>::Iterator List<T>::begin() {
    throw std::logic_error("List::begin не реализован");
}

template <ListElement T>
typename List<T>::Iterator List<T>::end() {
    throw std::logic_error("List::end не реализован");
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

TEST_CASE("addLast добавляет в конец") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("addStart добавляет в начало") {
    List<int> list;
    list.addStart(1);
    list.addStart(2);
    list.addStart(3);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 3);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 1);
}

TEST_CASE("addStart и addLast работают вместе") {
    List<int> list;
    list.addLast(2);
    list.addStart(1);
    list.addLast(3);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("addLast в пустой список задаёт и голову, и хвост") {
    List<int> list;
    list.addLast(42);

    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 42);

    // если хвост не выставлен, второй addLast потеряет элемент
    list.addLast(43);
    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(1) == 43);
}

TEST_CASE("addStart в пустой список задаёт и голову, и хвост") {
    List<int> list;
    list.addStart(42);

    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 42);

    list.addLast(43);
    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(1) == 43);
}

TEST_CASE("setValue меняет элемент на месте") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    list.setValue(1, 20);

    CHECK(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 20);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("копирующий конструктор сохраняет порядок") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    List<int> copy(list);

    REQUIRE(copy.getSize() == 3);
    CHECK(copy.getValue(0) == 1);
    CHECK(copy.getValue(1) == 2);
    CHECK(copy.getValue(2) == 3);
}

TEST_CASE("копия независима от оригинала") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

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
    copy.addLast(1);
    REQUIRE(copy.getSize() == 1);
    CHECK(copy.getValue(0) == 1);
    CHECK(empty.getSize() == 0);
}

TEST_CASE("присваивание заменяет содержимое") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    List<int> other;
    other.addLast(9);

    other = list;

    REQUIRE(other.getSize() == 3);
    CHECK(other.getValue(0) == 1);
    CHECK(other.getValue(1) == 2);
    CHECK(other.getValue(2) == 3);
}

TEST_CASE("после присваивания списки независимы") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    List<int> other;
    other = list;

    other.setValue(0, 99);

    CHECK(other.getValue(0) == 99);
    CHECK(list.getValue(0) == 1);
}

TEST_CASE("присваивание пустого списка очищает получателя") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    List<int> empty;
    list = empty;

    CHECK(list.getSize() == 0);
}

TEST_CASE("присваивание самому себе не портит список") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    // через ссылку, чтобы не ловить -Wself-assign-overloaded
    List<int>& alias = list;
    list = alias;

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("swap обменивает содержимое и размеры") {
    List<int> a;
    a.addLast(1);
    a.addLast(2);

    List<int> b;
    b.addLast(7);

    a.swap(b);

    REQUIRE(a.getSize() == 1);
    CHECK(a.getValue(0) == 7);

    REQUIRE(b.getSize() == 2);
    CHECK(b.getValue(0) == 1);
    CHECK(b.getValue(1) == 2);
}

TEST_CASE("swap с пустым списком") {
    List<int> filled;
    filled.addLast(1);

    List<int> empty;

    filled.swap(empty);

    CHECK(filled.getSize() == 0);
    REQUIRE(empty.getSize() == 1);
    CHECK(empty.getValue(0) == 1);

    // оба списка после swap пригодны для вставки
    filled.addLast(5);
    empty.addLast(6);
    CHECK(filled.getValue(0) == 5);
    CHECK(empty.getValue(1) == 6);
}

TEST_CASE("swap сам с собой ничего не меняет") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    list.swap(list);

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("список держит много элементов в правильном порядке") {
    List<int> list;
    for (int i = 0; i < 100; ++i) {
        list.addLast(i);
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

    list.addLast(1);
    CHECK_FALSE(list.isEmpty());
}

TEST_CASE("exercise: clear освобождает список") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    list.clear();

    CHECK(list.getSize() == 0);

    // после clear список пригоден для вставки
    list.addLast(3);
    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 3);
}

TEST_CASE("exercise: removeStart убирает первый элемент") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    list.removeStart();

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 2);
    CHECK(list.getValue(1) == 3);
}

TEST_CASE("exercise: removeStart на списке из одного элемента") {
    List<int> list;
    list.addLast(1);

    list.removeStart();

    REQUIRE(list.getSize() == 0);

    // хвост тоже должен обнулиться, иначе addLast потеряет элемент
    list.addLast(2);
    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 2);
}

TEST_CASE("exercise: removeLast убирает последний элемент") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    list.removeLast();

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("exercise: removeLast на списке из одного элемента") {
    List<int> list;
    list.addLast(1);

    list.removeLast();

    REQUIRE(list.getSize() == 0);

    // голова тоже должна обнулиться
    list.addStart(2);
    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 2);
}

TEST_CASE("exercise: insert в начало") {
    List<int> list;
    list.addLast(2);
    list.addLast(3);

    list.insert(0, 1);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("exercise: insert в середину") {
    List<int> list;
    list.addLast(1);
    list.addLast(3);

    list.insert(1, 2);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("exercise: insert по индексу getSize равносилен addLast") {
    List<int> list;
    list.addLast(1);

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
    list.addLast(2);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("exercise: removeAt убирает элемент по индексу") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    list.removeAt(1);

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 3);
}

TEST_CASE("exercise: at возвращает значение") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    CHECK(list.at(0) == 1);
    CHECK(list.at(1) == 2);
}

TEST_CASE("exercise: at работает на константном списке") {
    List<int> list;
    list.addLast(42);

    const List<int>& ref = list;
    CHECK(ref.at(0) == 42);
}

TEST_CASE("exercise: at за границей бросает out_of_range") {
    List<int> list;
    list.addLast(1);

    CHECK_THROWS_AS(list.at(1), std::out_of_range);
    CHECK_THROWS_AS(list.at(100), std::out_of_range);

    const List<int> empty;
    CHECK_THROWS_AS(empty.at(0), std::out_of_range);
}

TEST_CASE("exercise: move-конструктор забирает содержимое") {
    List<int> source;
    source.addLast(1);
    source.addLast(2);

    List<int> moved(std::move(source));

    REQUIRE(moved.getSize() == 2);
    CHECK(moved.getValue(0) == 1);
    CHECK(moved.getValue(1) == 2);

    // источник остаётся пустым, но валидным
    CHECK(source.getSize() == 0);
    source.addLast(3);
    CHECK(source.getValue(0) == 3);
}

TEST_CASE("exercise: move-присваивание забирает содержимое") {
    List<int> source;
    source.addLast(1);
    source.addLast(2);

    List<int> target;
    target.addLast(9);

    target = std::move(source);

    REQUIRE(target.getSize() == 2);
    CHECK(target.getValue(0) == 1);
    CHECK(target.getValue(1) == 2);
    CHECK(source.getSize() == 0);
}

TEST_CASE("exercise: operator== сравнивает содержимое") {
    List<int> a;
    a.addLast(1);
    a.addLast(2);

    List<int> b;
    b.addLast(1);
    b.addLast(2);

    CHECK(a == b);

    b.setValue(1, 99);
    CHECK_FALSE(a == b);

    List<int> shorter;
    shorter.addLast(1);
    CHECK_FALSE(a == shorter);

    List<int> empty1;
    List<int> empty2;
    CHECK(empty1 == empty2);
}

TEST_CASE("exercise: конструктор из списка инициализации") {
    List<int> list{1, 2, 3};

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 3);

    // хвост выставлен, список можно дополнять
    list.addLast(4);
    REQUIRE(list.getSize() == 4);
    CHECK(list.getValue(3) == 4);
}

TEST_CASE("exercise: getStart и getLast") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    CHECK(list.getStart() == 1);
    CHECK(list.getLast() == 3);

    // по неконстантной версии можно присваивать
    list.getStart() = 10;
    list.getLast() = 30;
    CHECK(list.getValue(0) == 10);
    CHECK(list.getValue(2) == 30);
}

TEST_CASE("exercise: getStart и getLast на списке из одного элемента") {
    List<int> list;
    list.addLast(42);

    CHECK(list.getStart() == 42);
    CHECK(list.getLast() == 42);
}

TEST_CASE("exercise: getStart и getLast на константном списке") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    const List<int>& ref = list;
    CHECK(ref.getStart() == 1);
    CHECK(ref.getLast() == 2);
}

TEST_CASE("exercise: removeValue убирает все совпадения") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(1);
    list.addLast(3);
    list.addLast(1);

    list.removeValue(1);

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 2);
    CHECK(list.getValue(1) == 3);
}

TEST_CASE("exercise: removeValue с краёв правит голову и хвост") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(1);

    list.removeValue(1);

    REQUIRE(list.getSize() == 1);
    CHECK(list.getValue(0) == 2);

    // голова и хвост должны указывать на оставшийся узел
    list.addLast(3);
    list.addStart(0);
    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 0);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("exercise: removeValue когда совпадают все элементы") {
    List<int> list;
    list.addLast(7);
    list.addLast(7);

    list.removeValue(7);

    REQUIRE(list.getSize() == 0);

    // список остаётся рабочим
    list.addLast(1);
    CHECK(list.getValue(0) == 1);
}

TEST_CASE("exercise: reverse разворачивает порядок") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    list.reverse();

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 3);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 1);
}

TEST_CASE("exercise: reverse переставляет и голову, и хвост") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    list.reverse();

    // если хвост не обновлён, addLast припишет не туда
    list.addLast(0);
    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 2);
    CHECK(list.getValue(1) == 1);
    CHECK(list.getValue(2) == 0);

    // а если не обновлена голова — промахнётся addStart
    list.addStart(9);
    CHECK(list.getValue(0) == 9);
}

TEST_CASE("exercise: reverse на пустом и на одном элементе") {
    List<int> empty;
    empty.reverse();
    CHECK(empty.getSize() == 0);

    List<int> single;
    single.addLast(1);
    single.reverse();
    REQUIRE(single.getSize() == 1);
    CHECK(single.getValue(0) == 1);
}

TEST_CASE("exercise: sort упорядочивает элементы") {
    List<int> list;
    list.addLast(3);
    list.addLast(1);
    list.addLast(2);

    list.sort();

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("exercise: sort оставляет голову и хвост согласованными") {
    List<int> list;
    list.addLast(2);
    list.addLast(3);
    list.addLast(1);

    list.sort();

    list.addLast(4);
    list.addStart(0);

    REQUIRE(list.getSize() == 5);
    CHECK(list.getValue(0) == 0);
    CHECK(list.getValue(4) == 4);
}

TEST_CASE("exercise: sort на пустом, одном и уже отсортированном") {
    List<int> empty;
    empty.sort();
    CHECK(empty.getSize() == 0);

    List<int> single;
    single.addLast(1);
    single.sort();
    CHECK(single.getValue(0) == 1);

    List<int> sorted;
    sorted.addLast(1);
    sorted.addLast(2);
    sorted.sort();
    CHECK(sorted.getValue(0) == 1);
    CHECK(sorted.getValue(1) == 2);
}

TEST_CASE("exercise: трёхстороннее сравнение") {
    List<int> a;
    a.addLast(1);
    a.addLast(2);

    List<int> b;
    b.addLast(1);
    b.addLast(3);

    // Результат <=> выносим в переменную: doctest не умеет разбирать
    // выражения с std::strong_ordering внутри CHECK.
    const bool a_before_b = (a <=> b) < 0;
    const bool b_after_a = (b <=> a) > 0;
    CHECK(a_before_b);
    CHECK(b_after_a);

    List<int> c;
    c.addLast(1);
    c.addLast(2);

    const bool a_same_as_c = (a <=> c) == 0;
    CHECK(a_same_as_c);
}

TEST_CASE("exercise: из <=> выводятся < > <= >=") {
    List<int> shorter;
    shorter.addLast(1);

    List<int> longer;
    longer.addLast(1);
    longer.addLast(2);

    // короче и совпадает началом — значит меньше, как у строк
    CHECK(shorter < longer);
    CHECK(longer > shorter);
    CHECK(shorter <= longer);
    CHECK_FALSE(shorter >= longer);
}

// --- упражнения: контракты на границы ---

TEST_CASE("exercise: getStart и getLast на пустом бросают out_of_range") {
    List<int> empty;
    CHECK_THROWS_AS(empty.getStart(), std::out_of_range);
    CHECK_THROWS_AS(empty.getLast(), std::out_of_range);

    const List<int> constEmpty;
    CHECK_THROWS_AS(constEmpty.getStart(), std::out_of_range);
    CHECK_THROWS_AS(constEmpty.getLast(), std::out_of_range);
}

TEST_CASE("exercise: removeStart и removeLast на пустом бросают out_of_range") {
    List<int> empty;
    CHECK_THROWS_AS(empty.removeStart(), std::out_of_range);
    CHECK_THROWS_AS(empty.removeLast(), std::out_of_range);
}

TEST_CASE("exercise: insert за границей бросает out_of_range") {
    List<int> list;
    list.addLast(1);

    // допустимы 0 и 1 (== getSize), всё остальное — ошибка
    CHECK_THROWS_AS(list.insert(2, 5), std::out_of_range);
    CHECK_THROWS_AS(list.insert(100, 5), std::out_of_range);
}

TEST_CASE("exercise: removeAt за границей бросает out_of_range") {
    List<int> list;
    list.addLast(1);

    CHECK_THROWS_AS(list.removeAt(1), std::out_of_range);
    CHECK_THROWS_AS(list.removeAt(100), std::out_of_range);
}

// --- упражнения: массовые операции ---

TEST_CASE("exercise: assign из count и value") {
    List<int> list;
    list.addLast(9);
    list.addLast(9);
    list.addLast(9);

    list.assign(2, 7);

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 7);
    CHECK(list.getValue(1) == 7);
}

TEST_CASE("exercise: assign из списка инициализации") {
    List<int> list;
    list.addLast(9);

    list.assign({1, 2, 3});

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(2) == 3);
}

TEST_CASE("exercise: assign нулевой длины очищает список") {
    List<int> list;
    list.addLast(1);

    list.assign(0, 5);

    CHECK(list.getSize() == 0);

    // список остаётся рабочим
    list.addLast(2);
    CHECK(list.getValue(0) == 2);
}

TEST_CASE("exercise: resize укорачивает с конца") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    list.resize(2);

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
}

TEST_CASE("exercise: resize удлиняет значением по умолчанию") {
    List<int> list;
    list.addLast(1);

    list.resize(3);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 0);
    CHECK(list.getValue(2) == 0);
}

TEST_CASE("exercise: resize удлиняет заданным значением") {
    List<int> list;
    list.addLast(1);

    list.resize(3, 8);

    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(1) == 8);
    CHECK(list.getValue(2) == 8);
}

TEST_CASE("exercise: resize до нуля и обратно") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    list.resize(0);
    REQUIRE(list.getSize() == 0);

    list.resize(2, 5);
    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 5);
}

TEST_CASE("exercise: unique убирает подряд идущие дубликаты") {
    List<int> list;
    list.addLast(1);
    list.addLast(1);
    list.addLast(2);
    list.addLast(2);
    list.addLast(2);
    list.addLast(1);

    list.unique();

    // последняя единица остаётся: она не подряд с первыми
    REQUIRE(list.getSize() == 3);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);
    CHECK(list.getValue(2) == 1);
}

TEST_CASE("exercise: unique на пустом и без дубликатов") {
    List<int> empty;
    empty.unique();
    CHECK(empty.getSize() == 0);

    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.unique();
    CHECK(list.getSize() == 2);
}

TEST_CASE("exercise: merge сливает два отсортированных списка") {
    List<int> a;
    a.addLast(1);
    a.addLast(3);
    a.addLast(5);

    List<int> b;
    b.addLast(2);
    b.addLast(4);

    a.merge(b);

    REQUIRE(a.getSize() == 5);
    CHECK(a.getValue(0) == 1);
    CHECK(a.getValue(1) == 2);
    CHECK(a.getValue(2) == 3);
    CHECK(a.getValue(3) == 4);
    CHECK(a.getValue(4) == 5);

    // источник опустошается: узлы перецеплены, а не скопированы
    CHECK(b.getSize() == 0);
}

TEST_CASE("exercise: merge с пустым списком") {
    List<int> a;
    a.addLast(1);

    List<int> empty;
    a.merge(empty);

    REQUIRE(a.getSize() == 1);
    CHECK(a.getValue(0) == 1);

    List<int> b;
    b.addLast(2);
    empty.merge(b);

    REQUIRE(empty.getSize() == 1);
    CHECK(empty.getValue(0) == 2);
}

TEST_CASE("exercise: emplaceLast и emplaceStart") {
    List<int> list;

    int& last = list.emplaceLast(2);
    CHECK(last == 2);

    int& first = list.emplaceStart(1);
    CHECK(first == 1);

    REQUIRE(list.getSize() == 2);
    CHECK(list.getValue(0) == 1);
    CHECK(list.getValue(1) == 2);

    // возвращается ссылка на сам элемент, а не копия
    last = 20;
    CHECK(list.getValue(1) == 20);
}

// --- упражнения: итератор ---

TEST_CASE("exercise: range-based for по списку") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    int sum = 0;
    for (int x : list) {
        sum += x;
    }

    CHECK(sum == 6);
}

TEST_CASE("exercise: итератор идёт вперёд до end") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    auto it = list.begin();
    CHECK(*it == 1);

    ++it;
    CHECK(*it == 2);

    ++it;
    const bool atEnd = (it == list.end());
    CHECK(atEnd);
}

TEST_CASE("exercise: у пустого списка begin равен end") {
    List<int> empty;

    const bool same = (empty.begin() == empty.end());
    CHECK(same);
}

TEST_CASE("exercise: постфиксный инкремент возвращает значение до сдвига") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);

    auto it = list.begin();
    const int before = *(it++);

    CHECK(before == 1);
    CHECK(*it == 2);
}

TEST_CASE("exercise: итератор ходит назад") {
    List<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);

    auto it = list.begin();
    ++it;
    ++it;
    CHECK(*it == 3);

    --it;
    CHECK(*it == 2);

    const int before = *(it--);
    CHECK(before == 2);
    CHECK(*it == 1);
}

TEST_CASE("exercise: через итератор можно менять элемент") {
    List<int> list;
    list.addLast(1);

    auto it = list.begin();
    *it = 42;

    CHECK(list.getValue(0) == 42);
}

TEST_CASE("exercise: operator-> у итератора") {
    List<int> list;
    list.addLast(7);

    auto it = list.begin();

    // у int нет полей, поэтому зовём стрелку явно: она отдаёт T*
    CHECK(*it.operator->() == 7);
}
