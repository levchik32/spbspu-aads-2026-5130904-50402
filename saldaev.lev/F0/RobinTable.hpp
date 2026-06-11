#ifndef ROBINTABLE_HPP
#define ROBINTABLE_HPP
#include "../common/vector.hpp"

namespace saldaev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    struct Node
    {
      bool taken_;
      Key key_;
      Value value_;
      size_t dist_;
    };

    Vector< Node > data_;
    Hash hasher_;
    Equal key_eq_;
    size_t slots_;
    size_t elements_;
    float max_load_factor_;

  public:
    struct Iterator
    {
      friend class HashTable;

    public:
      std::pair< Key, Value > &operator*();
      std::pair< Key, Value > *operator->();

      Iterator &operator++();
      Iterator operator++(int);

      bool operator==(const Iterator &other) const;
      bool operator!=(const Iterator &other) const;

    private:
      typename Vector< Node >::iterator it;

      Iterator(Vector< Node >::iterator it);
    };

    struct ConstIterator
    {
      friend class HashTable;

    public:
      const std::pair< Key, Value > &operator*() const;
      const std::pair< Key, Value > *operator->() const;

      ConstIterator &operator++();
      ConstIterator operator++(int);

      bool operator==(const ConstIterator &other) const;
      bool operator!=(const ConstIterator &other) const;

    private:
      typename Vector< Node >::iterator it;

      ConstIterator(Vector< Node >::iterator it);
    };

    HashTable(Hash hasher, Equal key_eq, size_t slots = 11, float load_factor = 0.75f);

    void add(Key k, Value v);
    bool has(Key k) const noexcept;
    Value get(Key k) const;
    Value &at(Key k);
    void remove(Key k);
    void rewrite(Key k, Value v);

    void rehash(size_t slots);
    void clear() noexcept;
    size_t size() const noexcept;
    float load_factor() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable &other) noexcept;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;
  };
}

#endif
