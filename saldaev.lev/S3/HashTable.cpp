#include "HashTable.hpp"

template < class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable &other):
  data_(other.data_),
  hasher_(other.hasher),
  comparator_(other.comparator),
  slots_(other.slots),
  elements_(other.elements_)
{}

template < class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable &&other) noexcept:
  data_(std::move(other.data_)),
  hasher_(other.hasher),
  comparator_(other.comparator),
  slots_(other.slots),
  elements_(other.elements_)
{}

template < class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots, Hash hasher, Equal comparator):
  data_(Vector(slots)),
  hasher_(hasher),
  comparator_(comparator),
  slots_(slots),
  elements_(0)
{}

template < class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal > &
saldaev::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable &other)
{
  if (this != std::addressof(other)) {
    Vector< List< std::pair< Key, Value > > > cpy(other.data_);
    data_.swap(cpy);

    hasher_ = other.hasher;
    comparator_ = other.comparator;
    slots_ = other.slots;
    elements_ = other.elements_;
  }

  return *this;
}

template < class Key, class Value, class Hash, class Equal >
saldaev::HashTable< Key, Value, Hash, Equal > &
saldaev::HashTable< Key, Value, Hash, Equal >::operator=(HashTable &&other) noexcept
{
  Vector< List< std::pair< Key, Value > > > cpy(std::move(other.data_));
  data_.swap(cpy);

  hasher_ = other.hasher;
  comparator_ = other.comparator;
  slots_ = other.slots;
  elements_ = other.elements_;

  return *this;
}
