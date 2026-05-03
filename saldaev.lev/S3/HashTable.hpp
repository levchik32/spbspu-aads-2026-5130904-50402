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

  private:
    Vector< List< std::pair< Key, Value > > > data_;
    Hash hasher_;
    Equal comparator_;
    size_t slots_ = 0;
    size_t elements_ = 0;
  };
}

#endif