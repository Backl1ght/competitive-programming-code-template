template <typename Data, typename Tag>
class SegmentTree {
 public:
  struct Node {
    Node* left_child_;
    Node* right_child_;

    int left_bound_;
    int right_bound_;

    Data data_;
    Tag tag_;

    void ApplayUpdate(const Tag& tag) {
      data_.Apply(left_bound_, right_bound_, tag);
      tag_.Apply(left_bound_, right_bound_, tag);
    }

    void MaintainInfomation() {
      ASSERT(left_child_ && right_child_);

      data_ = left_child_->data_ + right_child_->data_;
    }

    void Propagation() {
      if (tag_.NeedPropagation()) {
        right_child_->ApplayUpdate(tag_);
        left_child_->ApplayUpdate(tag_);
        tag_.Reset();
      }
    }

    Node() : left_child_(nullptr), right_child_(nullptr), left_bound_(-1), right_bound_(-1) {}
  };

  /*
   * Used for binary search on segment tree.
   *
   * If it should go to the desire(or optimal) direction, then return ture. Otherwise return false.
   * For example, if you want to find the leftmost position satisfying some condition, then return
   * true to go left.
   */
  using Judger = std::function<bool(const Data&, const Data&)>;

 private:
  void UpdateInternal(Node* p, int left, int right, const Tag& tag) {
    ASSERT(p);
    
    if (p->left_bound_ >= left && p->right_bound_ <= right) {
      p->ApplayUpdate(tag);
      return;
    }

    p->Propagation();

    if (p->left_child_->right_bound_ >= left)
      UpdateInternal(p->left_child_, left, right, tag);
    if (p->right_child_->left_bound_ <= right)
      UpdateInternal(p->right_child_, left, right, tag);

    p->MaintainInfomation();
  }

  const Data QueryInternal(Node* p, int left, int right) {
    ASSERT(p);

    if (p->left_bound_ >= left && p->right_bound_ <= right)
      return p->data_;

    p->Propagation();

    Data result;
    if (p->left_child_->right_bound_ >= left)
      result = result + QueryInternal(p->left_child_, left, right);
    if (p->right_child_->left_bound_ <= right)
      result = result + QueryInternal(p->right_child_, left, right);

    return result;
  }

  std::pair<int, const Data> FindLeftmostIfInternal(Node* p, const Judger& judger) {
    ASSERT(p);

    if (p->left_bound_ == p->right_bound_)
      return {p->left_bound_, p->data_};

    p->Propagation();

    if (judger(p->left_child_->data_, p->right_child_->data_))
      return FindLeftmostIfInternal(p->left_child_, judger);
    return FindLeftmostIfInternal(p->right_child_, judger);
  }

  std::pair<int, const Data> FindRightmostIfInternal(Node* p, const Judger& judger) {
    ASSERT(p);

    if (p->left_bound_ == p->right_bound_)
      return {p->left_bound_, p->data_};

    p->Propagation();

    if (judger(p->left_child_->data_, p->right_child_->data_))
      return FindRightmostIfInternal(p->right_child_, judger);
    return FindRightmostIfInternal(p->left_child_, judger);
  }

 public:
  SegmentTree(const std::vector<Data>& array) : n_(array.size()) {
    std::function<Node*(int, int)> build = [&](int left, int right) -> Node* {
      Node* p = new Node();
      p->left_bound_ = left;
      p->right_bound_ = right;

      if (left == right) {
        p->data_ = array[left];
      } else {
        int middle = (left + right) >> 1;
        p->left_child_ = build(left, middle);
        p->right_child_ = build(middle + 1, right);
        p->MaintainInfomation();
      }

      return p;
    };

    root_ = build(0, n_ - 1);
  }

  ~SegmentTree() {
    std::function<void(Node*)> dfs = [&](Node* p) {
      if (!p)
        return;
      dfs(p->left_child_);
      dfs(p->right_child_);
      delete p;
    };

    dfs(root_);
  }

  void Update(int left, int right, const Tag& tag) {
    ASSERT(left >= 0 && right < n_);

    UpdateInternal(root_, left, right, tag);
  }

  const Data Query(int left, int right) {
    ASSERT(left >= 0 && right < n_);

    return QueryInternal(root_, left, right);
  }

  std::pair<int, const Data> FindLeftmostIf(const Judger& judger) {
    return FindLeftmostIfInternal(root_, judger);
  }

  std::pair<int, const Data> FindRightmostIf(const Judger& judger) {
    return FindRightmostIfInternal(root_, judger);
  }

  std::string to_string() const {
    std::stringstream ss;
    ss << "SegmentTree [\n";
    std::function<void(Node*)> dfs = [&](Node* p) {
      if (p->left_bound_ == p->right_bound_) {
        ss << "  [" << p->left_bound_ << "]: {" << p->data_.to_string() << "}, {"
           << p->tag_.to_string() << "}\n";
        return;
      }
      dfs(p->left_child_);
      dfs(p->right_child_);
    };
    dfs(root_);
    ss << "]\n\n";

    return ss.str();
  }

 private:
  int n_;
  Node* root_;
};

struct Tag {
 public:
  int range_add_;

 public:
  Tag(int range_add = 0) : range_add_(range_add) {}

  bool NeedPropagation() { return range_add_ != 0; }

  void Apply(int left, int right, const Tag& tag) { range_add_ = range_add_ + tag.range_add_; }

  void Reset() { range_add_ = 0; }

  std::string to_string() const { return std::to_string(range_add_); }
};

struct Data {
 public:
  int mn1_;
  int mn2_;

 public:
  Data(int mn1 = 0x3f3f3f3f, int mn2 = 0x3f3f3f3f) : mn1_(mn1), mn2_(mn2) {}

  void Apply(int left, int right, const Tag& tag) {
    int length = right - left + 1;

    mn1_ = mn1_ + tag.range_add_;
    mn2_ = mn2_ + tag.range_add_;
  }

  friend Data operator+(const Data& lhs, const Data& rhs) {
    Data result;

    result.mn1_ = std::min(lhs.mn1_, rhs.mn1_);
    result.mn2_ = std::min(lhs.mn2_, rhs.mn2_);

    return result;
  }

  std::string to_string() const { return std::to_string(mn1_) + ", " + std::to_string(mn2_); }
};