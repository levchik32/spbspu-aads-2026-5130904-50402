#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>

namespace saldaev
{
  template < class T >
  struct List
  {
  private:
    struct Node;

  public:
    struct LCIter
    {
      friend class List;

      LCIter();

      LCIter &operator++() noexcept;
      LCIter operator++(int) noexcept;
      LCIter &operator--() noexcept;
      LCIter operator--(int) noexcept;
      bool operator==(const LCIter &other) const noexcept;
      bool operator!=(const LCIter &other) const noexcept;

      const T &operator*() const;
      const T *operator->() const;

    private:
      typename List::Node *curr;

      explicit LCIter(typename List::Node *node);
    };

    struct LIter
    {
      friend class List;

      LIter();

      LIter &operator++() noexcept;
      LIter operator++(int) noexcept;
      LIter &operator--() noexcept;
      LIter operator--(int) noexcept;

      bool operator==(const LIter &other) const noexcept;
      bool operator!=(const LIter &other) const noexcept;

      T &operator*();
      T *operator->();
      const T &operator*() const;
      const T *operator->() const;

    private:
      typename List::Node *curr;

      explicit LIter(typename List::Node *node);
    };

    List();
    ~List();
    List(const List &other);
    List &operator=(const List &other);
    List(List &&other);
    List &operator=(List &&other);

    void pushFront(const T &d);
    void pushBack(const T &d);
    T popFront();
    T popBack();
    void clear() noexcept;
    void swap(List &other) noexcept;

    void insertBefore(LIter it, const T &d);
    void insertAfter(LIter it, const T &d);
    LIter erase(LIter it);

    LIter begin();
    LIter end();
    LCIter begin() const;
    LCIter end() const;

    size_t size() const noexcept;

  private:
    struct Node
    {
      T data;
      Node *next;
      Node *prev;

      Node(const T &d, Node *n, Node *p);
    };

    Node *head;
    Node *tail;
    size_t length;
  };
}

template < class T >
saldaev::List< T >::Node::Node(const T &d, Node *n, Node *p):
  data(d),
  next(n),
  prev(p)
{}

template < class T >
saldaev::List< T >::List():
  head(nullptr),
  tail(nullptr),
  length(0)
{
  head = static_cast< Node * >(::operator new(sizeof(Node)));
  try {
    tail = static_cast< Node * >(::operator new(sizeof(Node)));
  } catch (...) {
    ::operator delete(head);
    throw;
  }
  head->next = tail;
  head->prev = nullptr;
  tail->prev = head;
  tail->next = nullptr;
}

template < class T >
saldaev::List< T >::~List()
{
  clear();
  ::operator delete(head);
  ::operator delete(tail);
}

template < class T >
saldaev::List< T >::List(const List &other):
  List()
{
  try {
    for (auto it = other.begin(); it != other.end(); ++it) {
      pushBack(*it);
    }
  } catch (...) {
    clear();
    throw;
  }
}

template < class T >
saldaev::List< T > &saldaev::List< T >::operator=(const List< T > &other)
{
  if (this != &other) {
    List< T > tmp(other);
    swap(tmp);
  }
  return *this;
}

template < class T >
saldaev::List< T >::List(List &&other):
  List()
{
  swap(other);
}

template < class T >
saldaev::List< T > &saldaev::List< T >::operator=(List< T > &&other)
{
  if (this != &other) {
    swap(other);
  }
  return *this;
}

template < class T >
void saldaev::List< T >::pushFront(const T &d)
{
  Node *n = new Node(d, head->next, head);
  head->next->prev = n;
  head->next = n;
  ++length;
}

template < class T >
void saldaev::List< T >::pushBack(const T &d)
{
  Node *n = new Node(d, tail, tail->prev);
  tail->prev->next = n;
  tail->prev = n;
  ++length;
}

template < class T >
T saldaev::List< T >::popFront()
{
  if (length != 0) {
    T ret = head->next->data;
    Node *tmp = head->next->next;
    delete head->next;
    head->next = tmp;
    tmp->prev = head;
    --length;
    return ret;
  }
  throw std::out_of_range("pop from empty list");
}

template < class T >
T saldaev::List< T >::popBack()
{
  if (length != 0) {
    T ret = tail->prev->data;
    Node *tmp = tail->prev->prev;
    delete tail->prev;
    tail->prev = tmp;
    tmp->next = tail;
    --length;
    return ret;
  }
  throw std::out_of_range("pop from empty list");
}

template < class T >
void saldaev::List< T >::clear() noexcept
{
  LIter it = begin();
  while (it != end()) {
    it = erase(it);
  }
}

template < class T >
void saldaev::List< T >::swap(List &other) noexcept
{
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  std::swap(length, other.length);
}

template < class T >
void saldaev::List< T >::insertBefore(LIter it, const T &d)
{
  typename List< T >::Node *n = new typename List< T >::Node(d, it.curr, it.curr->prev);
  it.curr->prev->next = n;
  it.curr->prev = n;
  ++length;
}

template < class T >
void saldaev::List< T >::insertAfter(LIter it, const T &d)
{
  if (it == end()) {
    throw std::out_of_range("cannot insert after end()");
  }
  typename List< T >::Node *n = new typename List< T >::Node(d, it.curr->next, it.curr);
  it.curr->next->prev = n;
  it.curr->next = n;
  ++length;
}

template < class T >
typename saldaev::List< T >::LIter saldaev::List< T >::erase(LIter it)
{
  if (it == end()) {
    throw std::out_of_range("cannot erase end() iterator");
  }
  typename List< T >::Node *curr = it.curr;
  typename List< T >::Node *next = curr->next;
  curr->next->prev = curr->prev;
  curr->prev->next = curr->next;
  delete curr;
  --length;
  return LIter(next);
}

template < class T >
typename saldaev::List< T >::LIter saldaev::List< T >::begin()
{
  return LIter(head->next);
}

template < class T >
typename saldaev::List< T >::LIter saldaev::List< T >::end()
{
  return LIter(tail);
}

template < class T >
typename saldaev::List< T >::LCIter saldaev::List< T >::begin() const
{
  return LCIter(head->next);
}

template < class T >
typename saldaev::List< T >::LCIter saldaev::List< T >::end() const
{
  return LCIter(tail);
}

template < class T >
size_t saldaev::List< T >::size() const noexcept
{
  return length;
}

// ___ LCIter ___

template < class T >
saldaev::List< T >::LCIter::LCIter():
  curr(nullptr)
{}

template < class T >
saldaev::List< T >::LCIter::LCIter(typename List< T >::Node *node):
  curr(node)
{}

template < class T >
typename saldaev::List< T >::LCIter &saldaev::List< T >::LCIter::operator++() noexcept
{
  if (curr->next) {
    curr = curr->next;
  }
  return *this;
}

template < class T >
typename saldaev::List< T >::LCIter saldaev::List< T >::LCIter::operator++(int) noexcept
{
  LCIter ret = *this;
  if (curr->next) {
    curr = curr->next;
  }
  return ret;
}

template < class T >
typename saldaev::List< T >::LCIter &saldaev::List< T >::LCIter::operator--() noexcept
{
  curr = curr->prev;
  return *this;
}

template < class T >
typename saldaev::List< T >::LCIter saldaev::List< T >::LCIter::operator--(int) noexcept
{
  LCIter ret = *this;
  curr = curr->prev;
  return ret;
}

template < class T >
bool saldaev::List< T >::LCIter::operator==(const LCIter &other) const noexcept
{
  return curr == other.curr;
}

template < class T >
bool saldaev::List< T >::LCIter::operator!=(const LCIter &other) const noexcept
{
  return !(*this == other);
}

template < class T >
const T &saldaev::List< T >::LCIter::operator*() const
{
  return curr->data;
}

template < class T >
const T *saldaev::List< T >::LCIter::operator->() const
{
  return &(curr->data);
}

// ___ LIter ___

template < class T >
saldaev::List< T >::LIter::LIter():
  curr(nullptr)
{}

template < class T >
saldaev::List< T >::LIter::LIter(typename List::Node *node):
  curr(node)
{}

template < class T >
typename saldaev::List< T >::LIter &saldaev::List< T >::LIter::operator++() noexcept
{
  if (curr->next) {
    curr = curr->next;
  }
  return *this;
}

template < class T >
typename saldaev::List< T >::LIter saldaev::List< T >::LIter::operator++(int) noexcept
{
  LIter ret = *this;
  if (curr->next) {
    curr = curr->next;
  }
  return ret;
}

template < class T >
typename saldaev::List< T >::LIter &saldaev::List< T >::LIter::operator--() noexcept
{
  curr = curr->prev;
  return *this;
}

template < class T >
typename saldaev::List< T >::LIter saldaev::List< T >::LIter::operator--(int) noexcept
{
  LIter ret = *this;
  curr = curr->prev;
  return ret;
}

template < class T >
bool saldaev::List< T >::LIter::operator==(const LIter &other) const noexcept
{
  return curr == other.curr;
}

template < class T >
bool saldaev::List< T >::LIter::operator!=(const LIter &other) const noexcept
{
  return !(*this == other);
}

template < class T >
T &saldaev::List< T >::LIter::operator*()
{
  return curr->data;
}

template < class T >
T *saldaev::List< T >::LIter::operator->()
{
  return &(curr->data);
}

template < class T >
const T &saldaev::List< T >::LIter::operator*() const
{
  return curr->data;
}

template < class T >
const T *saldaev::List< T >::LIter::operator->() const
{
  return &(curr->data);
}

#endif
