#include <iostream>
#include <unordered_map>

class Node {
public:
  int key, value;
  Node *pre, *next;

  Node(int k = 0, int v = 0) : key(k), value(v) {}
};

class LRUCache {
private:
  std::unordered_map<int, Node *> maps;
  Node *dummy;
  int capacity;

public:
  LRUCache(int capacity) {
    capacity = capacity;
    dummy = new Node{};
    dummy->pre = dummy;
    dummy->next = dummy;
  }

  void remove(Node *node) {
    node->next->pre = node->pre;
    node->pre->next = node->next;
  }

  void pushToHead(Node *node) {
    node->pre = dummy;
    node->next = dummy->next;
    node->next->pre = node;
    node->pre->next = node;
  }

  Node *getNode(int key) {
    auto it = maps.find(key);
    if (it == maps.end()) {
      return nullptr;
    } else {
      auto n = it->second;
      remove(n);
      pushToHead(n);
      return n;
    }
  }

  void put(int key, int value) {
    Node *node = new Node(key, value);
    maps[key] = node;
    pushToHead(node);

    if (maps.size() > capacity) {
      auto deleteNode = dummy->pre;
      maps.erase(key);
      remove(deleteNode);
      delete deleteNode;
    }
  }

  int get(int key) {
    Node *node = getNode(key);
    if (node == nullptr) {
      return -1;
    } else {
      return node->value;
    }
  }
};

