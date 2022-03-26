template <typename KeyType,
          typename ValueType,
          typename Comp = std::less<ValueType>>
class CartesianTree {
 private:
  struct TreeNode {
    KeyType key_;
    ValueType value_;
    TreeNode* left_;
    TreeNode* right_;
    TreeNode() {}
    TreeNode(KeyType key,
             ValueType value,
             TreeNode* left = nullptr,
             TreeNode* right = nullptr)
        : key_(key), value_(value), left_(left), right_(right) {}
    ~TreeNode() {
      if (left_ != nullptr) {
        delete left_;
        left_ = nullptr;
      }
      if (right_ != nullptr) {
        delete right_;
        right_ = nullptr;
      }
    }
  };
  TreeNode* root_;
  Comp comp_;

 public:
  CartesianTree() : root_(nullptr) {}
  ~CartesianTree() {
    if (root_ != nullptr) {
      delete root_;
      root_ = nullptr;
    }
  }
  void Insert(KeyType key, ValueType value) {
    if (root_ == nullptr) {
      root_ = new TreeNode(key, value);
      return;
    }
    auto LeftRotate = [](TreeNode*& p) -> void {
      assert(p->right_ != nullptr);
      TreeNode* right = p->right_;
      TreeNode* right_left = p->right_->left_;
      p->right_ = right_left;
      right->left_ = p;
      p = right;
    };
    auto RightRotate = [](TreeNode*& p) -> void {
      assert(p->left_ != nullptr);
      TreeNode* left = p->left_;
      TreeNode* left_right = p->left_->right_;
      p->left_ = left_right;
      left->right_ = p;
      p = left;
    };
    std::function<void(TreeNode*&)> InsertInternal =
        [&, this, key, value](TreeNode*& p) -> void {
      if (p == nullptr) {
        p = new TreeNode(key, value);
        return;
      }
      if (key <= p->key_) {
        InsertInternal(p->left_);
        if (comp_(p->left_->value_, p->value_))
          RightRotate(p);
      } else {
        InsertInternal(p->right_);
        if (comp_(p->right_->value_, p->value_))
          LeftRotate(p);
      }
    };
    InsertInternal(root_);
  }
  void Build(const std::vector<ValueType>& a) {
    assert(root_ == nullptr);
    size_t n = a.size();
    std::stack<TreeNode*> stack;
    for (size_t i = 0; i < n; ++i) {
      KeyType key = i;
      ValueType value = a[i];
      TreeNode* node = new TreeNode(key, value);
      while (!stack.empty() && comp_(value, stack.top()->value_)) {
        stack.pop();
      }
      if (!stack.empty()) {
        if (stack.top()->right_) {
          node->left_ = stack.top()->right_;
        }
        stack.top()->right_ = node;
      } else {
        node->left_ = root_;
        root_ = node;
      }
      stack.push(node);
    }
  }
};