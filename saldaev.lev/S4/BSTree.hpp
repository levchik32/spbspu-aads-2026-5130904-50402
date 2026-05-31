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
  TreeNode(Key key, Vakue value, TreeNode *parent);
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

template< class Key, class Value, class Compare >
class BSTree
{
  TreeNode *fRoot_, fLeaf_;
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
};

#endif
