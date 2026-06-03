#ifndef BSTREE_HPP
#define BSTREE_HPP
#include "../common/vector.hpp"

template< class Key, class Value >
class TreeNode
{
  Key key_;
  Value value_;
  TreeNode *left_;
  TreeNode *right_;
  TreeNode *parent_;

  TreeNode();
  TreeNode(Key key, Value value, TreeNode *parent);
};

template< class Key, class Value >
class BSTIterator
{
  TreeNode< Key, Value > *current_;

public:
  explicit BSTIterator(TreeNode< Key, Value > *node);

  std::pair< const Key, Value > operator*();
  std::pair< const Key, Value > &operator->();

  BSTIterator &operator++();
  BSTIterator &operator--();
  BSTIterator operator++(int);
  BSTIterator operator--(int);

  bool operator==(const BSTIterator &other) const;
  bool operator!=(const BSTIterator &other) const;
};

template< class Key, class Value >
class BSTConstIterator
{
  TreeNode< Key, Value > *current_;

public:
  explicit BSTConstIterator(TreeNode< Key, Value > *node);

  std::pair< const Key, Value > operator*();
  std::pair< const Key, Value > &operator->();

  BSTConstIterator &operator++();
  BSTConstIterator &operator--();
  BSTConstIterator operator++(int);
  BSTConstIterator operator--(int);

  bool operator==(const BSTConstIterator &other) const;
  bool operator!=(const BSTConstIterator &other) const;
};

template< class Key, class Value, class Compare = std::less< Key > >
class BSTree
{
  TreeNode< Key, Value > *fRoot_, fLeaf_;
  size_t size_;
  Compare comp_;

public:
  using const_iterator = BSTConstIterator< Key, Value >;
  using iterator = BSTIterator< Key, Value >;

  BSTree();
  BSTree(const BSTree &other);
  BSTree(BSTree &&other) noexcept;
  BSTree &operator=(const BSTree &other);
  BSTree &operator=(BSTree &&other) noexcept;
  ~BSTree();

  void push(const Key &k, const Value &v);
  Value &get(Key k);
  const Value &get(Key k) const;
  void drop(Key k);

  size_t size() const noexcept;
  bool empty() const noexcept;
  void clear();

  size_t height(const_iterator it) const;
  size_t height() const;

  const_iterator rotateLeft(const_iterator it);
  const_iterator rotateRight(const_iterator it);
  const_iterator rotateLargeLeft(const_iterator it);
  const_iterator rotateLargeRight(const_iterator it);

  iterator begin();
  iterator end() noexcept;
  const_iterator begin() const;
  const_iterator end() const noexcept;
  const_iterator cbegin() const;
  const_iterator cend() const noexcept;

  void swap(BSTree &other);
};

template< class Key, class Value >
TreeNode< Key, Value >::TreeNode():
  key_(Key()),
  value_(Value()),
  left_(nullptr),
  right_(nullptr),
  parent_(nullptr)
{}

template< class Key, class Value >
TreeNode< Key, Value >::TreeNode(Key key, Value value, TreeNode *parent):
  key_(key),
  value_(value),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::BSTree():
  fRoot_(nullptr),
  fLeaf_(nullptr),
  size_(0),
  comp_(Compare{})
{
  fRoot_ = new TreeNode< Key, Value >();
  try {
    fLeaf_ = new TreeNode< Key, Value >();
  } catch (...) {
    delete fRoot_;
    throw;
  }
  fRoot_->left_ = fLeaf_;
  fRoot_->right_ = fLeaf_;
  fLeaf_->parent_ = fRoot_;
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::BSTree(const BSTree &other):
  fRoot_(nullptr),
  fLeaf_(nullptr),
  size_(0),
  comp_(Compare{})
{
  fRoot_ = new TreeNode< Key, Value >();
  try {
    fLeaf_ = new TreeNode< Key, Value >();
  } catch (...) {
    delete fRoot_;
    throw;
  }
  for (const std::pair< Key, Value > &v : other) {
    push(v.first, v.second);
  }
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::BSTree(BSTree &&other) noexcept:
  fRoot_(other.fRoot_),
  fLeaf_(other.fLeaf_),
  size_(other.size_),
  comp_(other.comp_)
{
  other.fRoot_ = nullptr;
  other.fLeaf_ = nullptr;
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare > &BSTree< Key, Value, Compare >::operator=(const BSTree &other)
{
  if (this != std::addressof(other)) {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare > &BSTree< Key, Value, Compare >::operator=(BSTree &&other) noexcept
{
  if (this != std::addressof(other)) {
    clear();
    root_ = other.root_;
    size_ = other.size_;
    comp_ = std::move(other.comp_);
    other.root_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::~BSTree()
{
  if (fRoot_) {
    clear();
    delete fRoot_;
    delete fLeaf_;
  }
}

#endif
