#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include "../common/List.hpp"
#include "../common/vector.hpp"
#include <cstddef>
#include <utility>

namespace saldaev
{
  template < class Key, class Value, class Hash, class Equal >
  struct HashTable
  {
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
      Vector< List< std::pair< Key, Value > > > *buckets_;
      size_t bucket_idx_;
      typename List< std::pair< Key, Value > >::LIter list_iter_;

      Iterator(Vector< List< std::pair< Key, Value > > > *b, size_t idx,
               typename List< std::pair< Key, Value > >::LIter it);
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
      const Vector< List< std::pair< Key, Value > > > *buckets_;
      size_t bucket_idx_;
      typename List< std::pair< Key, Value > >::LCIter list_iter_;

      ConstIterator(const Vector< List< std::pair< Key, Value > > > *b, size_t idx,
                    typename List< std::pair< Key, Value > >::LCIter it);
    };

    HashTable(const HashTable &other);
    HashTable(HashTable &&other) noexcept;
    HashTable(size_t slots, Hash hasher, Equal comparator);
    ~HashTable() = default;
    HashTable &operator=(const HashTable &other);
    HashTable &operator=(HashTable &&other) noexcept;

    void add(Key k, Value v);
    bool has(Key k) const noexcept;
    Value get(Key k) const;
    void remove(Key k);
    void rewrite(Key k, Value v);

    void rehash(size_t slots);
    void clear() noexcept;
    size_t size() const noexcept;
    size_t bucket_count() const noexcept;
    bool empty() const noexcept;
    void swap(HashTable &other) noexcept;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;

  private:
    Vector< List< std::pair< Key, Value > > > data_;
    Hash hasher_;
    Equal comparator_;
    size_t slots_ = 0;
    size_t elements_ = 0;
  };
}

#endif