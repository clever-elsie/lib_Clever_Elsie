#ifndef ELSIE_TREAP
#define ELSIE_TREAP
#include <random>
#include <cstddef>
#include <cstdint>
#include <concepts>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <utility>
#include <math/random.hpp>

namespace elsie{

template<class val_t, class allocator = std::allocator<void>>
class implicit_treap {
private:
  struct node {
    val_t val;
    uint64_t priority;
    node *par;
    node *ch[2];
    template<class... Args>
    requires (std::constructible_from<val_t, Args...>)
    node(size_t pri, Args&&... args)
    :val(std::forward<Args>(args)...), priority(pri),
     par(nullptr), ch{nullptr, nullptr} {}
  };
  node *root;
  size_t size_;
  xor_shift<64> generator;
public:
  implicit_treap() : root(nullptr), size_(0), generator(std::random_device{}()) {}

  size_t size() const { return size_; }

  bool empty() const { return size_ == 0; }

  void insert(const val_t& val) {
    root = insert_node(root, val);
    size++;
  }

  void erase(const val_t& val) {
    root = erase_node(root, val);
    size--;
  }

  val_t& operator[](size_t index) {
    if (index >= size) {
      throw std::out_of_range("Index out of range");
    }
    auto it = find_by_order(index);
    return it->val;
  }

  std::shared_ptr<node> find_by_order(size_t order) {
    if (order >= size) {
      throw std::out_of_range("Order out of range");
    }
    return find_by_order_node(root, order);
  }

private:
  std::shared_ptr<node> insert_node(std::shared_ptr<node> node, const val_t& val) {
    if (!node) {
      return std::make_shared<node>(val);
    }

    if (val < node->val) {
      node->left = insert_node(node->left, val);
      if (node->left->priority > node->priority) {
        node = rotate_right(node);
      }
    } else {
      node->right = insert_node(node->right, val);
      if (node->right->priority > node->priority) {
        node = rotate_left(node);
      }
    }
    return node;
  }

  std::shared_ptr<node> erase_node(std::shared_ptr<node> node, const val_t& val) {
    if (!node) {
      return nullptr;
    }

    if (val < node->val) {
      node->left = erase_node(node->left, val);
    } else if (val > node->val) {
      node->right = erase_node(node->right, val);
    } else {
      if (!node->left) {
        return node->right;
      } else if (!node->right) {
        return node->left;
      } else {
        if (node->left->priority > node->right->priority) {
          node = rotate_right(node);
          node->right = erase_node(node->right, val);
        } else {
          node = rotate_left(node);
          node->left = erase_node(node->left, val);
        }
      }
    }
    return node;
  }

  std::shared_ptr<node> find_by_order_node(std::shared_ptr<node> node, size_t order) {
    if (!node) {
      return nullptr;
    }

    size_t left_size = 0;
    if (node->left) {
      left_size = get_size(node->left);
    }

    if (order == left_size) {
      return node;
    } else if (order < left_size) {
      return find_by_order_node(node->left, order);
    } else {
      return find_by_order_node(node->right, order - left_size - 1);
    }
  }

  size_t get_size(std::shared_ptr<node> node) const {
    if (!node) {
      return 0;
    }
    return 1 + get_size(node->left) + get_size(node->right);
  }

  std::shared_ptr<node> rotate_right(std::shared_ptr<node> node) {
    std::shared_ptr<node> left_child = node->left;
    node->left = left_child->right;
    left_child->right = node;
    return left_child;
  }

  std::shared_ptr<node> rotate_left(std::shared_ptr<node> node) {
    std::shared_ptr<node> right_child = node->right;
    node->right = right_child->left;
    right_child->left = node;
    return right_child;
  }
};

} // namespace elsie
#endif