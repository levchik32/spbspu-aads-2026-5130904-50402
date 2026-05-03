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
{
  for (; i < slots; ++i) {
    data_.pushBack(List< std::pair< Key, Value > >());
  }
}

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

template < class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  if (has(k)) {
    throw std::invalid_argument("Key already exists");
  }

  data_[hasher_(k) % slots_].newTail({k, v});
  ++elements_;
}

template < class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::has(Key k) const noexcept
{
  auto it = data_[hasher_(k) % slots_].begin();
  while (it.isValid()) {
    if (comparator_(it.getData().first, k)) {
      return true;
    }
    ++it;
  }
  return false;
}

template < class Key, class Value, class Hash, class Equal >
Value saldaev::HashTable< Key, Value, Hash, Equal >::get(Key k) const
{
  auto it = data_[hasher_(k) % slots_].begin();
  while (it.isValid()) {
    if (comparator_(it.getData().first, k)) {
      return it.getData().second;
    }
    ++it;
  }
  throw std::invalid_argument("Key does not exist");
}

template < class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::remove(Key k)
{
  size_t idx = hasher_(k) % slots_;
  auto it = data_[idx].begin();
  while (it.isValid()) {
    if (comparator_(it.getData().first, k)) {
      data_[idx].cutCurrent(it);
      --elements_;
      return;
    }
    ++it;
  }
  throw std::invalid_argument("Key does not exist");
}

template < class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::rewrite(Key k, Value v)
{
  size_t idx = hasher_(k) % slots_;
  auto it = data_[idx].begin();
  while (it.isValid()) {
    if (comparator_(it.getData().first, k)) {
      data_[idx].setData(it, v);
      return;
    }
    ++it;
  }
  throw std::invalid_argument("Key does not exist");
}

template < class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{}

template < class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::clear() noexcept
{
  auto it = data_.begin();
  while (it != data_.end()) {
    it->clear();
    ++it;
  }
  elements_ = 0;
}

template < class Key, class Value, class Hash, class Equal >
size_t saldaev::HashTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return elements_;
}

template < class Key, class Value, class Hash, class Equal >
size_t saldaev::HashTable< Key, Value, Hash, Equal >::bucket_count() const noexcept
{
  return slots_;
}

template < class Key, class Value, class Hash, class Equal >
bool saldaev::HashTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return !elements_;
}

template < class Key, class Value, class Hash, class Equal >
void saldaev::HashTable< Key, Value, Hash, Equal >::swap(HashTable &other) noexcept
{
  data_.swap(other.data_);
  std::swap(hasher_, other.hasher_);
  std::swap(comparator_, other.comparator_);
  std::swap(slots_, other.slots);
  std::swap(elements_, other.elements_);
}