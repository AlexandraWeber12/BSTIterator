// Creator: Ulrich Mitterhuber - S2010828005
// Creation date: 05.06.2021
// Course: Robotics Systems Engineering
// Exercise: UE4, BSP Binary Search Tree

#pragma once

#include <concepts>
#include <functional>
#include <tuple>
#include <stack>

namespace swe2 {

  template<typename T> concept RegularLessThanComparable =
    std::regular<T> &&
    requires(T const& lhs, T const& rhs) {
      {lhs < rhs} -> std::convertible_to<bool>;
  };

  template <RegularLessThanComparable T> class bst final {
  public:
    using value_type = T;
    using size_type = size_t;

  private:
    struct node_t final {
      value_type value{};
      node_t* p_left{ nullptr };
      node_t* p_right{ nullptr };
      size_type count{ 1 };
    };

  public:
    class iterator final {
    public:
      iterator() = default;
      iterator(node_t* node, std::stack<node_t*> iteratorStack) :
        p_node{ node },
        m_iteratorStack{ iteratorStack }
      {
      }

      const T& operator *() const {
        return p_node->value;
      }

      // pre-increment
      const iterator& operator ++() {
        if (m_iteratorStack.empty())
        {
          p_node = nullptr;
          return *this;
        }
        p_node = m_iteratorStack.top();
        m_iteratorStack.pop();
        buildUpStack(p_node->p_right);
        return *this;
      }

      // post-increment
      iterator operator ++(int) {
        iterator temp{ *this };
        ++(*this);
        return temp;
      }

      bool operator ==(iterator const& rhs) const {
        return p_node == rhs.p_node;
      }

      const size_type& getNodeCount() const {
        return p_node->count;
      }

    private:
      node_t* p_node{ nullptr };

      std::stack<node_t*> m_iteratorStack;

      void buildUpStack(node_t* node) {
        if (node != nullptr)
        {
          m_iteratorStack.push(node);
          buildUpStack(node->p_left);
        }
      }
    };

    using tree_iterator = iterator;
    bst() = default;

    bst(std::initializer_list<value_type> const& init) {
      for (auto const& e : init) {
        insert(e);
      }
    }

    bst(bst const& input) {
      copyTree(input.p_root);
    }

    bst& operator=(bst const& rhs) {
      clear();
      copyTree(rhs.p_root);
      return *this;
    }

    ~bst() {
      clear();
    }

    tree_iterator begin() const {
      std::stack<node_t*> iteratorStack;

      node_t* currentNode = p_root;
      while (currentNode->p_left)
      {
        iteratorStack.push(currentNode);
        currentNode = currentNode->p_left;
      }

      return { currentNode, iteratorStack };
    }

    tree_iterator end() const {
      return {};
    }

    void apply(std::function <void(value_type)> const& fun) {
      apply(p_root, fun);
    }

    void clear() {
      clear(p_root);
      p_root = nullptr;
      t_size = 0;
    }

    bool erase(value_type const& value) {
      return erase(p_root, value);
    }

    void insert(value_type const& value) {
      insert(p_root, value, t_size);
    }

    void insert(std::initializer_list <value_type> const& init) {
      for (auto const& e : init) {
        insert(e);
      }
    }

    bool empty() {
      return p_root == nullptr;
    }

    size_type size() const {
      return t_size;
    }

    bool contains(const T& item) {
      for (auto it = begin(); it != end(); ++it)
      {
        if (*it == item)
        {
          return true;
        }
      }
      return false;
    }

    tree_iterator find(const T& item) {
      for (auto it = begin(); it != end(); ++it)
      {
        if (*it == item)
        {
          return it;
        }
      }
      return end();
    }

    const size_t& count(const T& item) {
      return find(item).getNodeCount();
    }

    void swap(bst& other) {
      node_t* temp_root{ p_root };
      size_type temp_size{ t_size };
      p_root = other.p_root;
      t_size = other.t_size;
      other.p_root = temp_root;
      other.t_size = temp_size;
    }

    friend std::strong_ordering operator <=> (const bst<T>& lhs, const bst<T>& rhs) {
      if (&lhs == &rhs)
      {
        return std::strong_ordering::equivalent;
      }

      switch (compare(lhs, rhs))
      {
      case -1:
        return std::strong_ordering::less;
      case 1:
        return std::strong_ordering::greater;
      default:
        return std::strong_ordering::equal;
      }
    }

    friend std::ostream& operator << (std::ostream& lhs, const bst<T>& rhs) {
      for (auto it = rhs.begin(); it != rhs.end(); ++it)
      {
        lhs << *it << " ";
      }
      return lhs;
    }

  private:
     static int compare(const bst<T>& lhs, const bst<T>& rhs) {
      tree_iterator lhsIterator = lhs.begin();
      tree_iterator rhsIterator = rhs.begin();

      while (lhsIterator != lhs.end() && rhsIterator != rhs.end())
      {
        if (*lhsIterator > *rhsIterator)
        {
          return 1;
        }

        if (*lhsIterator < *rhsIterator)
        {
          return -1;
        }

        ++lhsIterator;
        ++rhsIterator;
      }

      size_type lhsLength = lhs.size();
      size_type rhsLength = rhs.size();

      if (lhsLength > rhsLength)
      {
        return 1;
      }
      else if (lhsLength < rhsLength)
      {
        return -1;
      }
      return 0;
    }

    static void apply(node_t* const p, std::function <void(value_type)> const& fun) {
      if (p)
      {
        apply(p->p_left, fun);	//Links
        fun(p->value);			//Wurzel
        apply(p->p_right, fun);	//Rechts

        //LWR... inorder
        //WLR... preorder
        //LRW... postorder
      }
    }

    void copyTree(node_t* const p) {
      if (p)
      {
        insert(p->value);
        copyTree(p->p_left);
        copyTree(p->p_right);
      }
    }

    static void clear(node_t* const p) {
      if (p)
      {
        clear(p->p_left);
        clear(p->p_right);

        delete p;
      }
    }

    static std::tuple <node_t*, node_t*> find_symmetric_predecessor(node_t* const p) {
      node_t* p_prv{ nullptr };
      node_t* p_sym{ p->p_left };

      while (p_sym->p_right)
      {
        p_prv = p_sym;
        p_sym = p_sym->p_right;
      }
      return { p_sym, p_prv };
    }

    static bool erase(node_t*& p, value_type const& value) {
      if (p)
      {
        if (value < p->value)
        {
          return erase(p->p_left, value);
        }
        if (value > p->value)
        {
          return erase(p->p_right, value);
        }

        node_t* p_del{ p };

        if (!p_del->p_right)
        {
          p = p_del->p_left;
        }
        else if (!p_del->p_left)
        {
          p = p_del->p_right;
        }
        else
        {
          auto [p_sym, p_prev] { find_symmetric_predecessor(p_del) };

          p_del->value = std::move(p_sym->value);

          (p_prev ? p_prev->p_right : p_del->p_left) = p_sym->p_left;
          p_del = p_sym;
        }
        delete p_del; p_del = nullptr;
        return true;
      }
      return false;
    }

    static void insert(node_t*& p_root, value_type const& value, size_type& size) {
      if (!p_root)
      {
        p_root = new node_t{ value };
        size++;
      }
      else
      {
        if (value < p_root->value)
        {
          insert(p_root->p_left, value, size);
        }
        else if (value > p_root->value)
        {
          insert(p_root->p_right, value, size);
        }
        else
        {
          p_root->count++;
        }
      }
    }

    node_t* p_root{ nullptr };
    size_type t_size{};
  };

} // namespace swe2
