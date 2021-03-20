#ifndef RBTREE_H_
#define RBTREE_H_

/* 红黑树 */
class RBTree {

public:
  void Put(const int &key, const int &val);

  int Get(const int &key) const;

  void LevelOrder() const;

private:
  static const bool RED;
  static const bool BLACK;

  /* 红黑内部节点 */
  struct Node {
    int _key;
    int _val;
    bool _color;
    Node *_left;
    Node *_right;

    Node(const int &key, const int &val, const bool &color = RED,
         Node *left = nullptr, Node *right = nullptr);
  };

  Node *Put(Node *root, const int &key, const int &val);

  int Get(Node *root, const int &key) const;

  // 局部变换操作
  Node *RotateLeft(Node *root);
  Node *RotateRight(Node *root);
  void FlipColor(Node *root);

  // 判断节点颜色
  inline bool IsRed(const Node *root) const {
    if (nullptr == root)
      return false;
    return root->_color == RED;
  }

private:
  Node *_root = nullptr;
};

#endif
