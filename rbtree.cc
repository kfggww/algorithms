#include "rbtree.h"
#include <cstdlib>
#include <iostream>
#include <list>
#include <cassert>

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

  int bh = 0;
  assert(IsValid(_root, bh));
  
}

int RBTree::Get(const int &key) const { return Get(_root, key); }

void RBTree::DeleteMin() {
  if (_root != nullptr)
    _root = DeleteMin(_root);

  int bh = 0;
  assert(IsValid(_root, bh));
}

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

RBTree::Node *RBTree::DeleteMin(Node *root) {
  // outer
  if (root->_left == nullptr && root->_right == nullptr) {
    delete root;
    return nullptr;
  }

  if (IsTwoNode(root)) {
    FlipColor(root);
    root->_left = DeleteMin(root->_left);
  }

  else if (!IsTwoNode(root->_left))
    root->_left = DeleteMin(root->_left);

  else if (IsTwoNode(root->_left) && IsTwoNode(root->_right)) {
    FlipColor(root);
    root->_left = DeleteMin(root->_left);
  }

  else {
    root->_right = RotateRight(root->_right);
    root = RotateLeft(root);
    root->_right->_color = BLACK;
    root->_left->_left->_color = RED;
    root->_left = DeleteMin(root->_left);
  }

  // 局部变换保持平衡
  if (IsRed(root->_right) && !IsRed(root->_left))
    root = RotateLeft(root);
  if (IsRed(root->_left) && IsRed(root->_left->_left))
    root = RotateRight(root);
  if (IsRed(root->_left) && IsRed(root->_right))
    FlipColor(root);

  return root;
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

bool RBTree::IsValid(Node *root, int &black_height) const {

  if(root == nullptr)
    return true;

  int left_bh = 0, right_bh = 0;
  bool valid_left = IsValid(root->_left, left_bh);
  if(!valid_left)
    return false;
  if(!IsRed(root->_left))
    left_bh += 1;

  bool valid_right = IsValid(root->_right, right_bh);
  if(!valid_right)
    return false;
  if(!IsRed(root->_right))
    right_bh += 1;

  if(left_bh != right_bh)
    return false;

  black_height = left_bh;
  return true;
}

int main(int argc, char **argv) {
  RBTree rbtree;

  int size = 5;
  int delete_count = 2;
  if (argc == 3) {
    size = atoi(argv[1]);
    delete_count = atoi(argv[2]);
  }
  for (int i = 0; i < size; ++i) {
    rbtree.Put(i, i);
  }
  rbtree.LevelOrder();

  for (int i = 0; i < delete_count; ++i)
    rbtree.DeleteMin();

  rbtree.LevelOrder();

  return 0;
}
