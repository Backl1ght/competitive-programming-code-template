template <typename KeyType, typename ValueType, typename Comp = std::less<ValueType>>
class CartesianTree {
 private:
  struct TreeNode {
    KeyType key_;
    ValueType value_;
    TreeNode* left_;
    TreeNode* right_;
    TreeNode() {}
    TreeNode(const KeyType& key,
             const ValueType& value,
             TreeNode* left = nullptr,
             TreeNode* right = nullptr)
        : key_(key), value_(value), left_(left), right_(right) {}
  };

 private:
  TreeNode* AllocateNode(const KeyType& key, const ValueType& value) {
    TreeNode* p = new TreeNode(key, value);
    return p;
  }

  void FreeNode(TreeNode*& p) {
    if (p) {
      delete p;
      p = nullptr;
    }
  }

  void LeftRotate(TreeNode*& p) {
    assert(p->right_ != nullptr);
    TreeNode* right = p->right_;
    TreeNode* right_left = right->left_;

    p->right_ = right_left;
    right->left_ = p;
    p = right;
  }

  void RightRotate(TreeNode*& p) {
    assert(p->left_ != nullptr);
    TreeNode* left = p->left_;
    TreeNode* left_right = left->right_;

    p->left_ = left_right;
    left->right_ = p;
    p = left;
  }

  void InsertInternal(TreeNode*& p, const KeyType& key, const ValueType& value) {
    if (p == nullptr) {
      p = AllocateNode(key, value);
      return;
    }

    if (key <= p->key_) {
      InsertInternal(p->left_, key, value);
      if (comp_(p->left_->value_, p->value_))
        RightRotate(p);
    } else {
      InsertInternal(p->right_, key, value);
      if (comp_(p->right_->value_, p->value_))
        LeftRotate(p);
    }
  }

 public:
  CartesianTree() : root_(nullptr) {}

  CartesianTree(const std::pair<KeyType, ValueType>* a, int n) : root_(nullptr) {
    std::vector<TreeNode*> stack;
    for (int i = 0; i < n; ++i) {
      KeyType key = a[i].first;
      ValueType value = a[i].second;
      TreeNode* node = AllocateNode(key, value);
      while (!stack.empty() && comp_(value, stack.back()->value_)) {
        stack.pop_back();
      }
      if (!stack.empty()) {
        if (stack.back()->right_) {
          node->left_ = stack.back()->right_;
        }
        stack.back()->right_ = node;
      } else {
        node->left_ = root_;
        root_ = node;
      }
      stack.push_back(node);
    }
  }

  CartesianTree(const std::vector<std::pair<KeyType, ValueType>>& a) : root_(nullptr) {
    int n = a.size();

    std::vector<TreeNode*> stack;
    for (int i = 0; i < n; ++i) {
      KeyType key = a[i].first;
      ValueType value = a[i].second;
      TreeNode* node = AllocateNode(key, value);
      while (!stack.empty() && comp_(value, stack.back()->value_)) {
        stack.pop_back();
      }
      if (!stack.empty()) {
        if (stack.back()->right_) {
          node->left_ = stack.back()->right_;
        }
        stack.back()->right_ = node;
      } else {
        node->left_ = root_;
        root_ = node;
      }
      stack.push_back(node);
    }
  }

  ~CartesianTree() {
    std::queue<TreeNode*> q;
    q.push(root_);
    while (!q.empty()) {
      TreeNode* p = q.front();
      q.pop();

      if (p->left_)
        q.push(p->left_);
      if (p->right_)
        q.push(p->right_);

      FreeNode(p);
    }
  }

  void Insert(const KeyType& key, const ValueType& value) { InsertInternal(root_, key, value); }

  void Solve() {
    i64 lans = 0, rans = 0;

    std::queue<TreeNode*> q;
    q.push(root_);
    while (!q.empty()) {
      TreeNode* p = q.front();
      q.pop();

      lans ^= (i64(p->key_) * ((p->left_ ? p->left_->key_ : 0) + 1));
      rans ^= (i64(p->key_) * ((p->right_ ? p->right_->key_ : 0) + 1));

      if (p->left_)
        q.push(p->left_);
      if (p->right_)
        q.push(p->right_);
    }

    std::cout << lans << " " << rans << "\n";
  }

 private:
  TreeNode* root_;
  Comp comp_;
};