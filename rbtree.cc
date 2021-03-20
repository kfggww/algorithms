#include "rbtree.h"
#include <iostream>
#include <list>

using std::cout;
using std::endl;
using std::list;

const bool RBTree::RED = true;
const bool RBTree::BLACK = false;


RBTree::Node::Node(const int &key, const int &val, const bool &color,
                   Node *left, Node *right)
    : _key(key), _val(val), _color(color), _left(left), _right(right) {}

void RBTree::Put(const int &key, const int &val) {

  _root = Put(_root, key, val);
  _root->_color = BLACK;
}

int RBTree::Get(const int &key) const { return Get(_root, key); }

void RBTree::LevelOrder() const {

  list<Node *> queue;
  if (nullptr != _root)
    queue.push_back(_root);

  while (!queue.empty()) {
    auto size = queue.size();
    for (auto i = 0; i < size; ++i) {
      Node *cur = queue.front();
      queue.pop_front();
      cout << cur->_key << ", ";

      if (nullptr != cur->_left)
        queue.push_back(cur->_left);
      if (nullptr != cur->_right)
        queue.push_back(cur->_right);
    }
    cout << endl;
  }
}

RBTree::Node *RBTree::Put(Node *root, const int &key, const int &val) {

  if (nullptr == root)
    return new Node(key, val);

  if (key == root->_key)
    root->_val = val;
  else if (key < root->_key)
    root->_left = Put(root->_left, key, val);
  else
    root->_right = Put(root->_right, key, val);

  // 局部变换保持平衡
  if (IsRed(root->_right) && !IsRed(root->_left))
    root = RotateLeft(root);
  if (IsRed(root->_left) && IsRed(root->_left->_left))
    root = RotateRight(root);
  if (IsRed(root->_left) && IsRed(root->_right))
    FlipColor(root);

  return root;
}

int RBTree::Get(Node *root, const int &key) const {

  if (nullptr == root)
    return -1;

  if (key == root->_key)
    return root->_val;
  else if (key < root->_key)
    return Get(root->_left, key);
  else
    return Get(root->_right, key);
}

RBTree::Node *RBTree::RotateLeft(Node *root) {

  bool rootColor = root->_color;
  Node *x = root->_right;
  root->_right = x->_left;
  root->_color = x->_color;
  x->_left = root;
  x->_color = rootColor;

  return x;
}

RBTree::Node *RBTree::RotateRight(Node *root) {

  bool rootColor = root->_color;
  Node *x = root->_left;
  root->_left = x->_right;
  root->_color = x->_color;
  x->_right = root;
  x->_color = rootColor;

  return x;
}

void RBTree::FlipColor(Node *root) {

  root->_color = !root->_color;
  if (nullptr != root->_left)
    root->_left->_color = !root->_left->_color;
  if (nullptr != root->_right)
    root->_right->_color = !root->_right->_color;
}

int main() {
  RBTree rbtree;

  for (int i = 0; i < 5; ++i) {
    rbtree.Put(i, i);
  }

  rbtree.LevelOrder();

  return 0;
}
