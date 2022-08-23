template <typename ValueType>
class Treap {
 private:
  typedef int32_t size_t;

  struct Node {
    ValueType value;
    uint32_t priority;
    size_t size;
    Node* left;
    Node* right;
  };
  Node* root_;
  std::mt19937 rng_;

 private:
  Node* NewNode(ValueType value) {
    Node* p = new Node;
    p->value = value;
    p->priority = rng_();
    p->size = 1;
    p->left = p->right = nullptr;
    return p;
  }

  void Free(Node*& p) {
    if (p) {
      delete p;
      p = nullptr;
    }
  }

  size_t GetSize(Node* p) { return p ? p->size : 0; }

  void PushUp(Node* p) {
    if (!p)
      return;
    p->size = GetSize(p->left) + 1 + GetSize(p->right);
  }

  std::pair<Node*, Node*> SplitValue(Node* p, const ValueType& value) {
    if (!p)
      return {nullptr, nullptr};

    std::pair<Node*, Node*> result;
    if (p->value <= value) {
      auto right_result = SplitValue(p->right, value);
      p->right = right_result.first;

      result.first = p;
      result.second = right_result.second;
    } else {
      auto left_result = SplitValue(p->left, value);
      p->left = left_result.second;

      result.first = left_result.first;
      result.second = p;
    }
    PushUp(p);

    return result;
  }

  Node* Merge(Node* a, Node* b) {
    if (a == nullptr)
      return b;
    else if (b == nullptr)
      return a;

    Node* result;
    if (a->priority < b->priority) {
      result = a;
      a->right = Merge(a->right, b);
      PushUp(a);
    } else {
      result = b;
      b->left = Merge(a, b->left);
      PushUp(b);
    }

    return result;
  }

 public:
  Treap() : root_(nullptr), rng_(std::chrono::steady_clock::now().time_since_epoch().count()) {}

  ~Treap() {
    std::function<void(Node*)> dfs = [&](Node* p) -> void {
      if (!p)
        return;
      dfs(p->left);
      dfs(p->right);
      freep(p);
    };
    dfs(root_);
  }

  void Insert(const ValueType& value) {
    auto [a, b] = SplitValue(root_, value);
    a = Merge(a, NewNode(value));
    root_ = Merge(a, b);
  }

  void Delete(const ValueType& value) {
    auto [a, b] = SplitValue(root_, value);
    auto [c, d] = SplitValue(a, value - 1);

    assert(d);

    Node* temp = d;
    d = Merge(d->left, d->right);
    Free(temp);

    a = Merge(c, d);
    root_ = Merge(a, b);
  }

  size_t GetRank(const ValueType& value) {
    auto [a, b] = SplitValue(root_, value - 1);
    size_t result = GetSize(a) + 1;
    root_ = Merge(a, b);
    return result;
  }

  ValueType GetKth(size_t k) {
    assert(k <= GetSize(root_));
    Node* p = root_;

    ValueType result;
    while (p) {
      if (k <= GetSize(p->left)) {
        p = p->left;
      } else if (k == GetSize(p->left) + 1) {
        result = p->value;
        break;
      } else {
        k -= GetSize(p->left) + 1;
        p = p->right;
      }
    }
    return result;
  }

  ValueType GetPrev(const ValueType& value) {
    auto [a, b] = SplitValue(root_, value - 1);

    assert(a != nullptr);
    Node* c = a;
    while (c->right)
      c = c->right;
    ValueType result = c->value;

    root_ = Merge(a, b);

    return result;
  }

  ValueType GetNext(const ValueType& value) {
    auto [a, b] = SplitValue(root_, value);

    assert(b != nullptr);
    Node* c = b;
    while (c->left)
      c = c->left;
    ValueType result = c->value;

    root_ = Merge(a, b);

    return result;
  }

  std::string to_string() const {
    std::stringstream ss;
    ss << "Treap: [";

    std::function<void(Node*)> dfs = [&](Node* p) {
      if (p == nullptr)
        return;

      dfs(p->left);
      ss << serialize(p->value) << ", ";
      dfs(p->right);
    };
    dfs(root_);

    ss << "]\n";

    return ss.str();
  }
};