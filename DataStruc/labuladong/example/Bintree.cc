#include <iostream>
#include <vector>

using namespace std;

template <typename T> 
struct Stack {
public:
  Stack() = default;

  void push(const T& val) { 
    data_.push_back(val);
  }

  void pop() {
    data_.pop_back();
  }

  const T& top() const {
    return data_.back();
  }

  bool empty() const {
    return data_.empty();
  }

  size_t size() const {
    return data_.size();
  }


private:
  vector<T> data_;
};

template <typename T> 
struct BinTreeNode {
  T data;
  BinTreeNode *LeftChild;
  BinTreeNode *RightChild;
  BinTreeNode *Parent;
};

// 二叉树先序遍历 递归版
template <typename T>
void travPre_R(BinTreeNode<T> *root) {
  if(!root) {
    cout << "panic" << endl;
    return;
  }
  cout << root->data;
  travPre_R(root->LeftChild);
  travPre_R(root->RightChild);
}

// 二叉树先序遍历 迭代版
template <typename T>
void travPre_v1(BinTreeNode<T> *root) {
  Stack<BinTreeNode<T> *> s; // stack for iteration
  if(root)
    s.push(root);
  while (!s.empty()) {
    root = s.pop();
    cout << root->data;

    if(root->RightChild)
      s.push(root->RightChild); // right child first in last out
    if(root->LeftChild) 
      s.push(root->LeftChild);  // left child last in first out
  }
}

// 二叉树先序遍历 迭代版 Plus
template <typename T>
static void VisitAlongLeftBranch(BinTreeNode<T> *x, Stack<BinTreeNode<T> *> &S) {
  while(x) {
    cout << x->data;
    if(x->RightChild)
      S.push(x -> RightChild);
    x = x->LeftChild;
  }
}

template <typename T>
void travPre_v2(BinTreeNode<T>* root) {
  Stack<BinTreeNode<T>* > S;
  while(true) {
    VisitAlongLeftBranch(root, S);
    if(S.empty())
      break;
    root = S.pop();
  }
}

// 二叉树中序遍历 递归实现
template <typename T>
void travIn_R(BinTreeNode<T>* root) {
  if(!root) 
    return;
  travIn_R(root->LeftChild);
  cout << root->data;
  travIn_R(root->RightChild);
}


int main() {

  return 0;
}