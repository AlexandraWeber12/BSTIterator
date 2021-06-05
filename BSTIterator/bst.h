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

    struct node_t; // ?

    class iterator final {
    public:
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
        iterator copie = *this;
        if (lastActionPop) {
          lastActionPop = false;
          //buildUpStack(p_node->p_right);
          return copie;
        }
        if (p_node->p_right != nullptr)
        {
          m_iteratorStack.push(p_node);
          copie = buildUpStack(p_node->p_right);
        }
        else {
          p_node = m_iteratorStack.top();
          m_iteratorStack.pop();
          lastActionPop = true;
        }
        

        return copie;
      }

      // post-increment
      iterator operator ++(int) {



     

        return *this;
      }

      bool operator ==(iterator const& rhs) const {
        return p_node == rhs.p_node;
      }

    private:
      node_t* p_node{};
      std::stack<node_t*> m_iteratorStack;
      bool lastActionPop = false;

      iterator buildUpStack(node_t* node) {
        p_node = node;
        if (node->p_left)
        {
          m_iteratorStack.push(node);
          buildUpStack(node->p_left);
        }
        return *this;
      }
    };

    using value_type = T;
    using size_type = size_t;
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

    tree_iterator begin() {
      std::stack<node_t*> iteratorStack;

      node_t* currentNode = p_root;
      while (currentNode->p_left)
      {
        iteratorStack.push(currentNode);
        currentNode = currentNode->p_left;
      }

      return { currentNode, iteratorStack };
    }

    /*tree_iterator end() {
      return { p_data + n };
    }*/

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

    size_type size() {
      return t_size;
    }

    bool contains() {

    }

  private:
    struct node_t final {
      value_type value{};
      node_t* p_left{ nullptr };
      node_t* p_right{ nullptr };
      size_type count{ 1 };
    };

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

    node_t* findLexiVorfahre(node_t* const p) {
      if (!p->p_right)
      {
        findLexiVorfahre(p->p_right);
      }
      return p;
    }

    static void insert(node_t*& p_root, value_type const& value, size_type& size) { //static da sonst auf inneres p_root zugegriffen wird
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
        //insert(value < p_root->value ? p_root->p_left : p_root->p_right, value, size);
      }
    }

    node_t* p_root{ nullptr };
    size_type t_size{};
  };

} // namespace swe2
