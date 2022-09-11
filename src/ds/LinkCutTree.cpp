/**
 * LinkCutTree(aka Sleator-Tarjan Tree).
 *
 * @reference: A Data Structure for Dynamic Trees. by Sleator and Tarjan.
 *             https://www.cs.cmu.edu/~sleator/papers/dynamic-trees.pdf
 */
template <typename Data, typename SubtreeData, typename Tag>
class LinkCutTree {
 public:
  /**
   * Node definition of auxiliary tree(Splay).
   */
  struct Node {
    int vertex_id_;

    Node* left_;
    Node* right_;
    Node* parent_;

    int reverse_;

    Data node_data_;
    Data path_data_;
    Tag tag_;

    SubtreeData node_subtree_data_;
    SubtreeData real_subtree_data_;
    SubtreeData virtual_subtree_data_;

    Node(int vertex_id)
        : vertex_id_(vertex_id),
          left_(nullptr),
          right_(nullptr),
          parent_(nullptr),
          reverse_(0),
          node_data_(),
          path_data_(),
          tag_(),
          node_subtree_data_(),
          real_subtree_data_(),
          virtual_subtree_data_() {}

    void Reverse() {
      std::swap(left_, right_);
      reverse_ ^= 1;
    }

    void Apply(const Tag& tag) {
      node_data_.Apply(tag);
      path_data_.Apply(tag);
      tag_.Apply(tag);
    }

    void Propagation() {
      if (reverse_) {
        if (left_)
          left_->Reverse();
        if (right_)
          right_->Reverse();
        reverse_ = 0;
      }

      if (tag_.NeedPropagation()) {
        if (left_)
          left_->Apply(tag_);
        if (right_)
          right_->Apply(tag_);
        tag_.Reset();
      }
    }

    void Maintain() {
      path_data_ = node_data_;
      if (left_)
        path_data_ = path_data_ + left_->path_data_;
      if (right_)
        path_data_ = path_data_ + right_->path_data_;

      real_subtree_data_ = node_subtree_data_ + virtual_subtree_data_;
      if (left_)
        real_subtree_data_ = real_subtree_data_ + left_->real_subtree_data_;
      if (right_)
        real_subtree_data_ = real_subtree_data_ + right_->real_subtree_data_;
    }

    bool IsLeftChild() {
      if (parent_ == nullptr)
        return false;
      return parent_->left_ == this;
    }

    bool IsRootOfAuxiliaryTree() {
      if (parent_ == nullptr)
        return true;
      return (parent_->left_ != this) && (parent_->right_ != this);
    }

    bool IsRootOfOriginalTree() { return parent_ == nullptr; }

    std::string to_string() const { return std::to_string(vertex_id_); }
  };

 private:
  void LeftRotate(Node* p) {
    ASSERT(p->parent_);

    Node* father = p->parent_;
    Node* grandfather = father->parent_;
    Node* left_child_of_p = p->left_;

    p->parent_ = grandfather;
    if (!father->IsRootOfAuxiliaryTree()) {
      if (father->IsLeftChild())
        grandfather->left_ = p;
      else
        grandfather->right_ = p;
    }

    p->left_ = father;
    father->parent_ = p;

    father->right_ = left_child_of_p;
    if (left_child_of_p)
      left_child_of_p->parent_ = father;

    father->Maintain();
    p->Maintain();
  }

  void RightRotate(Node* p) {
    ASSERT(p->parent_);

    Node* father = p->parent_;
    Node* grandfather = father->parent_;
    Node* right_child_of_p = p->right_;

    p->parent_ = grandfather;
    if (!father->IsRootOfAuxiliaryTree()) {
      if (father->IsLeftChild())
        grandfather->left_ = p;
      else
        grandfather->right_ = p;
    }

    p->right_ = father;
    father->parent_ = p;

    father->left_ = right_child_of_p;
    if (right_child_of_p)
      right_child_of_p->parent_ = father;

    father->Maintain();
    p->Maintain();
  }

  void Rotate(Node* p) {
    Node* father = p->parent_;

    if (father->IsRootOfAuxiliaryTree()) {
      if (p->IsLeftChild()) {
        RightRotate(p);
      } else {
        LeftRotate(p);
      }
      return;
    }

    if (p->IsLeftChild() == father->IsLeftChild()) {
      if (p->IsLeftChild()) {
        RightRotate(father);
        RightRotate(p);
      } else {
        LeftRotate(father);
        LeftRotate(p);
      }
      return;
    }

    if (p->IsLeftChild()) {
      RightRotate(p);
      LeftRotate(p);
    } else {
      LeftRotate(p);
      RightRotate(p);
    }
  }

  /**
   * Splay p until p is root of an auxiliary tree.
   */
  void Splay(Node* p) {
    Node* temp = p;

    // Update path to root. That is, make one up to down propagation starting from root.
    std::stack<Node*> stack;
    while (true) {
      stack.push(p);
      if (p->IsRootOfAuxiliaryTree())
        break;
      else
        p = p->parent_;
    }
    while (!stack.empty()) {
      stack.top()->Propagation();
      stack.pop();
    }

    p = temp;
    while (!p->IsRootOfAuxiliaryTree()) {
      Rotate(p);
    }
  }

  void Splay(int u) { Splay(vertices_[u]); }

  void Access(int u) {
    // Jump from u to root of original tree.
    Node* p = vertices_[u];
    Node* last = nullptr;
    while (!last || !last->IsRootOfOriginalTree()) {
      Splay(p);

      // Append last path to the back of current path.
      if (p->right_)
        p->virtual_subtree_data_ = p->virtual_subtree_data_ + p->right_->real_subtree_data_;
      if (last)
        p->virtual_subtree_data_ = p->virtual_subtree_data_ - last->real_subtree_data_;
      p->right_ = last;
      p->Maintain();

      last = p;
      p = p->parent_;
    }
  }

  void MakeRoot(int u) {
    // Make u part of root path firstly.
    Access(u);

    // Since during Access(vertex(u)), we set the right child of node(u) to nil, after
    // Access(vertex(u)), vertex(u) will be the deepest vertex on root path. In the other word,
    // node(u) will be the last node being visited in one in-order traverse of auxiliary tree
    // containing it.
    //
    // To make vertex(u) the root, we need to make it the lowest vertex on root path, or the first
    // node being visited during in-order traverse.
    //
    // That is, we need to reverse the in-order traverse of tree, or in the other word, reverse
    // the root path. Simply reverse the auxiliary tree containing node(u) can achieve this.
    Splay(u);
    vertices_[u]->Reverse();
  }

  Node* FindRoot(int u) {
    // Make vertex(u) part of root path firstly. This process will not change the root of the
    // original tree.
    Access(u);

    // The root will be the first node visited during one in-order traverse, that is, the
    // leftmost(or smallest) node in auxiliary tree is the represent of the root in original tree.
    // Make node(u) the root of auxiliary tree, and then search for the leftmost node.
    Splay(u);
    Node* p = vertices_[u];
    while (p->left_) {
      p->Propagation();
      p = p->left_;
    }
    return p;
  }

  /**
   * Make path u...v to be the root path.
   *
   * After this process, the root of auxiliary tree representing root path will be node(v). Further
   * operation on path u...v can be made by using lazy propagation tech on splay tree.
   */
  void Expose(int u, int v) {
    MakeRoot(u);
    Access(v);
    Splay(v);
  }

 public:
  LinkCutTree(int n) : n_(n), vertices_(n_, nullptr) {
    for (int i = 0; i < n; ++i) {
      vertices_[i] = new Node(i);
    }
  }

  ~LinkCutTree() {
    for (int i = 0; i < n_; ++i) {
      delete vertices_[i];
    }
  }

  std::string to_string() const {
    std::stringstream ss;
    ss << "Splay Forest: [\n";
    std::function<void(Node*)> dfs = [&](Node* p) {
      if (!p)
        return;
      p->Propagation();

      dfs(p->left_);
      ss << p->to_string() << ", ";
      dfs(p->right_);
    };
    for (int i = 0, j = 0; i < n_; ++i) {
      if (vertices_[i]->IsRootOfAuxiliaryTree()) {
        ss << "  [" << j++ << "] Path: ";
        ss << (vertices_[i]->IsRootOfOriginalTree()
                   ? "NULL"
                   : std::to_string(vertices_[i]->parent_->vertex_id_))
           << "[";
        dfs(vertices_[i]);
        ss << "]\n";
      }
    }
    ss << "]\n\n";

    return ss.str();
  }

  void Link(int u, int v) {
    ASSERT(u != v);

    MakeRoot(u);

    // If the root of tree containing vertex(v) is vertex(u), after make vertex(u) the root of tree
    // containing it, then vertex(v) and vertex(u) are already in the same component, there is
    // nothing to do.
    if (FindRoot(v) == vertices_[u])
      return;

    // If the root of tree containing vertex(v) is not vertex(u), then FindRoot(v) will not effct
    // the structure of original tree containing vertex(u). That is, vertex(u) is still the root of
    // tree containing it.
    //
    // Since vertex(u) and vertex(v) are both root of trees containnig themselves, to link these two
    // trees, just set the parent of node(u) to node(v). Setting parent of node(v) to node(u) will
    // work, too.
    vertices_[u]->parent_ = vertices_[v];

    vertices_[v]->virtual_subtree_data_ =
        vertices_[v]->virtual_subtree_data_ + vertices_[u]->real_subtree_data_;
    vertices_[v]->Maintain();
  }

  void Cut(int u, int v) {
    ASSERT(u != v);

    MakeRoot(u);

    // In such case, u and v belong to two different components, ther is nothing to do.
    if (FindRoot(v) != vertices_[u])
      return;

    // The root of tree containing vertex(u) and vertex(v) will still be vertex(u). That is,
    // vertex(u) is the lowest vertex on the root path.
    //
    // After FindRoot, vertex(v) will be the deepest vertex on the root path. And node(v) will be
    // the root of auxiliary tree. And root path contain only vertex(u) and vertex(v).
    //
    // That is, is there is an edge between vertex(u) and vertex(v), node(u) will be the left child
    // of node(v), and node(u) has no child, node(v) has no right child.
    if (vertices_[u]->parent_ == vertices_[v] && vertices_[v]->left_ == vertices_[u] &&
        vertices_[v]->right_ == nullptr) {
      vertices_[u]->parent_ = nullptr;
      vertices_[v]->left_ = nullptr;
      vertices_[v]->Maintain();
    }
  }

  bool IsConnected(int u, int v) {
    // After MakeRoot(u) and Access(v), vertex(u) and vertex(v) are both part of root path, and
    // vertex(u) will be the lowest vertex, that is, node(u) will be the leftmost node in auxiliary
    // tree. Splay(node(v)) to make node(v) the root of auxiliary tree, and then search for the
    // leftmost node on the auxiliary tree.
    MakeRoot(u);
    return FindRoot(v) == vertices_[u];
  }

  int Lca(int u, int v) {
    // TODO(backlight): finish the code.
    return -1;
  }

  void UpdatePath(int u, int v, const Tag& tag) {
    Expose(u, v);
    vertices_[v]->Apply(tag);
  }

  const Data QueryPath(int u, int v) {
    Expose(u, v);
    return vertices_[v]->path_data_;
  }

  void UpdateSubtreeNode(int u, const SubtreeData& node_subtree_data) {
    MakeRoot(u);
    vertices_[u]->node_subtree_data_ = node_subtree_data;
    vertices_[u]->Maintain();
  }

  // TODO(backlight): support subtree data path update.

  const SubtreeData QuerySubtree(int u) {
    MakeRoot(u);
    return vertices_[u]->real_subtree_data_;
  }

 private:
  int n_;
  std::vector<Node*> vertices_;
};

struct Tag {
 public:
  Tag() {}

  bool NeedPropagation() { return false; }

  void Apply(const Tag& tag) {}

  void Reset() {}

  std::string to_string() const { return "Tag"; }

 public:
};

struct Data {
 public:
  Data() {}

  void Apply(int size, const Tag& tag) {}

  friend Data operator+(const Data& lhs, const Data& rhs) {
    Data result;

    return result;
  }

  std::string to_string() const { return "Data"; }

 public:
};

struct SubtreeData {
 public:
  friend SubtreeData operator+(const SubtreeData& lhs, const SubtreeData& rhs) {
    SubtreeData result;

    return result;
  }

  friend SubtreeData operator-(const SubtreeData& lhs, const SubtreeData& rhs) {
    SubtreeData result;

    return result;
  }

 public:
};