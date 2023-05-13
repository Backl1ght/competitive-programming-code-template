namespace SelfAdjustingTopTree {

/**
 * A simplified version of self-adjusting top tree by ternarizing both
 * compress tree and rake tree.
 *
 * Ternarization
 *
 *   1. As for the original version, it keep the circular order of outer
 * neighbors for a vertex on the root path, and these outer neighbors will be
 * divided into at most two parts by the root path, and each part will be
 * transformed into one outer cluster with rake operation. For simplification,
 * we just transform all outer neighbors into one outer cluster with rake
 * operation and lose the infomation about the circular order of outer
 * neighbors. Let this outer cluster be (u, v) with v on the root path, attach
 * this cluster to compress(v) as a middle child. Now, compress tree become a
 * 3-arc tree.
 *
 *   2. As for the original version, each leaf of a rake tree represent an
 * unit tree, and each internal node of a rake tree represent raking it's left
 * child onto it's right child. For simplification, attach the unit tree(or
 * compress tree) to a rake node as a middle child, and then a rake node
 * represent raking it's at most three chilren into one cluster. This gonna to
 * make the top tree easier to implement, but with worse efficiency(still
 * amortized O(\log n) but with larger constant number). Now, rake tree become
 * a 3-arc tree.
 *
 *     Since a rake node without middle child is useless, in the other word,
 * such node can be deleted while all data still well maintained, add a rule
 * that each node on rake tree should contain middle child.
 *
 * Data on Vertices
 *
 *   The original version of top trees can easily maintain data on edges, but
 * need extra discussion for maintaining data on vertices since top tree
 * clusters correspond to edges. Here, to maintain data on vertices, we gonna
 * to attach each vertex a dummy edge to store vertex-related data, which is
 * easier to implement but with larger constant number. And after this, data
 * on vertices will stored in compress tree as well as data on edges.
 *
 * Reference
 *
 *   1. Self-adjusting top trees by Robert E. Tarjan et al.
 *
 *   2. https://negiizhao.blog.uoj.ac/blog/4912 by negiizhao.
 *
 *   3. https://www.cnblogs.com/HolyK/p/14332568.html by HolyK.
 *
 */

/**
 * TODO(backlight)
 *   - serialize.
 *   - ugly memory management.
 *   - support non local search.
 */

template <typename Tag,
          typename BaseData,
          typename RakeData,
          typename CompressData>
class TernarizedTopTree {
 public:
  class TernarizedSplay {
   public:
    virtual void PushUp() = 0;
    virtual void PushDown() = 0;
    virtual std::string to_string() = 0;
    virtual ~TernarizedSplay(){};

   public:
    TernarizedSplay* left() { return child_[0]; }

    TernarizedSplay* right() { return child_[1]; }

    TernarizedSplay* middle() { return child_[2]; }

    TernarizedSplay* parent() { return parent_; }

    int IsRight() {
      ASSERT(parent_);
      return parent_->right() == this;
    }

    int Which() {
      ASSERT(parent_);
      for (int i = 0; i < 3; ++i) {
        if (parent_->child_[i] == this) {
          return i;
        }
      }
      ASSERT(false);
    }

    bool IsRoot() const {
      if (!parent_)
        return true;
      return (parent_->left() != this && parent_->right() != this);
    }

    void SetChild(int which, TernarizedSplay* child) {
      child_[which] = child;
      if (child)
        child->parent_ = this;
    }

   public:
    void Rotate() {
      TernarizedSplay* parent = parent_;
      ASSERT(parent->is_compress_ == is_compress_);

      TernarizedSplay* grand_parent = parent->parent();
      int x = IsRight();

      parent_ = grand_parent;
      if (grand_parent != nullptr)
        grand_parent->SetChild(parent->Which(), this);

      parent->SetChild(x, child_[x ^ 1]);

      SetChild(x ^ 1, parent);

      parent->PushUp();
      PushUp();
    }

    void PushDownFromRoot() {
      std::stack<TernarizedSplay*> stack;
      TernarizedSplay* p = this;
      stack.push(p);
      while (!p->IsRoot()) {
        p = p->parent_;
        stack.push(p);
      }
      while (!stack.empty()) {
        stack.top()->PushDown();
        stack.pop();
      }
    }

    void Splay() {
      PushDownFromRoot();

      while (!IsRoot()) {
        if (!parent_->IsRoot()) {
          TernarizedSplay* x =
              (IsRight() == parent_->IsRight() ? parent_ : this);
          x->Rotate();
        }
        Rotate();
      }
    }

    void Splice() {
      ASSERT(IsRoot());
      ASSERT(parent()->middle() == this);
      ASSERT(is_compress_);
      ASSERT(parent()->is_rake_);

      // Local Splay
      parent_->Splay();

      TernarizedSplay* parent = parent_;
      TernarizedSplay* grand_parent = parent->parent();
      ASSERT(grand_parent->is_compress_);
      ASSERT(grand_parent->middle() == parent);

      grand_parent->Splay();
      parent->PushDown();

      // Splice
      parent->SetChild(2, grand_parent->right());
      grand_parent->SetChild(1, this);
      parent->PushUp();
      grand_parent->PushUp();

      if (parent->middle() == nullptr) {
        // Grandparent might has no right child, under such case, parent will
        // contain no middle child after splice. Since rake tree got a rule that
        // each node in a rake tree should contain middle child, parent should
        // be deleted.

        TernarizedSplay* replacement;
        if (parent->left() == nullptr) {
          replacement = parent->right();
        } else if (parent->right() == nullptr) {
          replacement = parent->left();
        } else {
          replacement = parent->left();
          replacement->PushDown();
          while (replacement->right()) {
            replacement = replacement->right();
            replacement->PushDown();
          }

          // Replacement will be the largest node after the cutting and
          // splaying, that is, replacement must has no right child.
          parent->left()->parent_ = nullptr;
          replacement->Splay();
          replacement->SetChild(1, parent->right());
          replacement->PushUp();
        }

        // Data on the rake tree will not change, so there is no need to
        // change data on grand_parent.
        grand_parent->SetChild(2, replacement);
        grand_parent->PushUp();

        delete parent;
      }
    }

   public:
    TernarizedSplay(bool is_compress, bool is_rake)
        : is_compress_(is_compress),
          is_rake_(is_rake),
          child_({nullptr, nullptr, nullptr}),
          parent_(nullptr) {}

   public:
    bool is_compress_;
    bool is_rake_;

    std::array<TernarizedSplay*, 3> child_;
    TernarizedSplay* parent_;
  };

  class RakeTree : public TernarizedSplay {
   public:
    void PushDown() override {
      RakeTree* left_child = dynamic_cast<RakeTree*>(TernarizedSplay::left());
      RakeTree* right_child = dynamic_cast<RakeTree*>(TernarizedSplay::right());
      CompressTree* middle_child =
          dynamic_cast<CompressTree*>(TernarizedSplay::middle());

      if (tag_.NeedPropagation()) {
        if (left_child)
          left_child->Apply(tag_);
        if (right_child)
          right_child->Apply(tag_);
        if (middle_child)
          middle_child->Apply(tag_);
        tag_.Reset();
      }
    }

    void PushUp() override {
      RakeData left_rake =
          TernarizedSplay::left()
              ? dynamic_cast<RakeTree*>(TernarizedSplay::left())->rake_data_
              : RakeData();

      RakeData right_rake =
          TernarizedSplay::right()
              ? dynamic_cast<RakeTree*>(TernarizedSplay::right())->rake_data_
              : RakeData();

      CompressData middle_compress =
          TernarizedSplay::middle()
              ? dynamic_cast<CompressTree*>(TernarizedSplay::middle())
                    ->compress_data_
              : CompressData();

      rake_data_ = middle_compress.Rake(left_rake.Rake(right_rake));
    }

    void Apply(const Tag& tag) {}

    RakeTree() : TernarizedSplay(false, true), rake_data_() {}

    std::string to_string() override { return "Rake Tree"; }

    ~RakeTree() override {}

   public:
    RakeData rake_data_;
    Tag tag_;
  };

  class CompressTree : public TernarizedSplay {
   public:
    void Reverse() {
      reverse = !reverse;
      std::swap(TernarizedSplay::child_[0], TernarizedSplay::child_[1]);
      compress_data_.Reverse();
    }

    void PushDown() override {
      CompressTree* left_child =
          dynamic_cast<CompressTree*>(TernarizedSplay::left());
      CompressTree* right_child =
          dynamic_cast<CompressTree*>(TernarizedSplay::right());
      RakeTree* middle_child =
          dynamic_cast<RakeTree*>(TernarizedSplay::middle());
      if (reverse) {
        if (left_child)
          left_child->Reverse();
        if (right_child)
          right_child->Reverse();
        reverse = false;
      }

      if (tag_.NeedPropagation()) {
        if (left_child)
          left_child->Apply(tag_);
        if (middle_child)
          middle_child->Apply(tag_);
        if (right_child)
          right_child->Apply(tag_);
        tag_.Reset();
      }
    }

    void PushUp() override {
      CompressData left_compress =
          TernarizedSplay::left()
              ? dynamic_cast<CompressTree*>(TernarizedSplay::left())
                    ->compress_data_
              : CompressData();
      CompressData right_compress =
          TernarizedSplay::right()
              ? dynamic_cast<CompressTree*>(TernarizedSplay::right())
                    ->compress_data_
              : CompressData();
      RakeData middle_rake =
          TernarizedSplay::middle()
              ? dynamic_cast<RakeTree*>(TernarizedSplay::middle())->rake_data_
              : RakeData();

      compress_data_ = CompressData::Compress(left_compress, right_compress,
                                              middle_rake, base_data_);
    }

    void Apply(const Tag& tag) {
      tag_.Apply(tag);
      compress_data_.Apply(tag);
      base_data_.Apply(tag);
    }

   public:
    void Expose() {
      ASSERT(is_vertex_);

      // Make this node an endpoint(Discard path below).
      TernarizedSplay::Splay();
      ASSERT(TernarizedSplay::IsRoot());
      ASSERT((!TernarizedSplay::parent() || TernarizedSplay::Which() == 2));

      if (TernarizedSplay::right()) {
        auto r = new RakeTree;
        r->SetChild(0, TernarizedSplay::middle());
        r->SetChild(2, TernarizedSplay::right());
        r->PushUp();

        TernarizedSplay::SetChild(1, nullptr);
        TernarizedSplay::SetChild(2, r);
        PushUp();
      }
      ASSERT(!TernarizedSplay::right());

      // Local Splay and Splice.
      TernarizedSplay* p = this;
      while (p->parent_ != nullptr) {
        ASSERT(p->is_compress_);
        p->Splice();
        p = p->parent_;
      }

      // Global Splay
      TernarizedSplay::Splay();
    }

    void Evert() {
      Expose();
      Reverse();
      PushDown();
    }

    CompressTree* Root() {
      Expose();

      CompressTree* p = this;
      p->PushDown();
      while (p->left() != nullptr) {
        p = dynamic_cast<CompressTree*>(p->left());
        p->PushDown();
      }
      p->Splay();
      return p;
    }

   public:
    CompressTree(bool is_vertex)
        : TernarizedSplay(true, false),
          is_vertex_(is_vertex),
          is_edge_(!is_vertex_),
          reverse(false),
          tag_(),
          compress_data_(),
          base_data_() {}

    ~CompressTree() override {}

    std::string to_string() override {
      std::stringstream ss;
      ss << "CompressTree: [";
      std::function<void(TernarizedSplay*)> dfs =
          [&](TernarizedSplay* p) -> void {
        if (!p)
          return;

        p->PushDown();
        dfs(p->left());
        ss << ((CompressTree*)p)->base_data_.to_string() << ", ";
        dfs(p->right());
      };
      TernarizedSplay* p = this;
      dfs(p);
      ss << "]";
      return ss.str();
    }

   public:
    bool is_vertex_;
    bool is_edge_;
    bool reverse;

    Tag tag_;
    CompressData compress_data_;
    BaseData base_data_;
  };

 public:
  static CompressTree* AllocateNode(bool is_vertex) {
    return new CompressTree(is_vertex);
  }

  static void FreeNode(TernarizedSplay*& p) {
    if (p) {
      delete p;
      p = nullptr;
    }
  }

  static void FreeComponent(TernarizedSplay* p) {
    std::function<void(TernarizedSplay*&)> dfs = [&](TernarizedSplay*& p) {
      if (!p)
        return;
      for (int i = 0; i < 3; ++i) {
        dfs(p->child_[i]);
      }
      FreeNode(p);
    };

    if (p->parent_ == nullptr) {
      dfs(p);
    }
  }

 public:
  static void Expose(CompressTree* vertex_u, CompressTree* vertex_v) {
    ASSERT(vertex_u && vertex_u->is_vertex_);
    ASSERT(vertex_v && vertex_v->is_vertex_);

    vertex_u->Evert();
    vertex_v->Expose();
  }

  static void Link(CompressTree* vertex_u, CompressTree* vertex_v) {
    ASSERT(vertex_u && vertex_u->is_vertex_);
    ASSERT(vertex_v && vertex_v->is_vertex_);

    vertex_u->Evert();
    if (vertex_v->Root() == vertex_u)
      return;

    vertex_u->Expose();
    ASSERT(!vertex_u->right());

    vertex_v->Evert();
    ASSERT(!vertex_v->left() && !vertex_v->parent());

    vertex_u->SetChild(1, vertex_v);
    vertex_v->PushUp();
    vertex_u->PushUp();
  }

  static void Cut(CompressTree* vertex_v, CompressTree* vertex_u) {
    ASSERT(vertex_u && vertex_u->is_vertex_);
    ASSERT(vertex_v && vertex_v->is_vertex_);

    Expose(vertex_u, vertex_v);
    if (vertex_v->left() != vertex_u || vertex_u->right())
      return;

    ASSERT(vertex_u == vertex_v->left());

    vertex_u->parent_ = nullptr;
    vertex_v->SetChild(0, nullptr);
    vertex_v->PushUp();
  }

  /**
   * Return compress tree representing path from u to v.
   */
  static CompressTree* GetPath(CompressTree* vertex_u, CompressTree* vertex_v) {
    Expose(vertex_u, vertex_v);
    return vertex_v;
  }

  /**
   * Return compress tree representing subtree whose root is target while the
   * root of tree is root.
   */
  static CompressTree* GetSubtree(CompressTree* root, CompressTree* target) {
    Expose(root, target);
    return target;
  }
};

}  // namespace SelfAdjustingTopTree

class Tag {
 public:
  Tag() {}

  bool NeedPropagation() { return false; }

  void Reset() {}

  void Apply(const Tag& tag) {}

 public:
};

class BaseData {
 public:
  BaseData(int id = -1, int size = 0) : id_(id), size_(size) {}

  void Apply(const Tag& tag) {}

  std::string to_string() const {
    std::stringstream ss;
    ss << "(" << id_ << "," << size_ << ")";
    return ss.str();
  }

 public:
  int id_;
  int size_;
};

class RakeData {
 public:
  RakeData(int size = 0) : size_(size) {}

  void Apply(const Tag& tag) {}

  RakeData Rake(const RakeData& other) {
    RakeData result;

    result.size_ = size_ + other.size_;

    return result;
  }

 public:
  int size_;
};

class CompressData {
 public:
  CompressData(int size = 0) : size_(size) {}

  void Reverse() {}

  void Apply(const Tag& tag) {}

  RakeData Rake(const RakeData& other) {
    RakeData result;

    result.size_ = size_ + other.size_;

    return result;
  }

  static CompressData Compress(const CompressData& l,
                               const CompressData& r,
                               const RakeData& m,
                               const BaseData& b) {
    CompressData result;

    result.size_ = l.size_ + r.size_ + m.size_ + b.size_;

    return result;
  }

 public:
  int size_;
};

using TopTree = SelfAdjustingTopTree::
    TernarizedTopTree<Tag, BaseData, RakeData, CompressData>;