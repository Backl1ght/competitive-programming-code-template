// Problem: P5357 【模板】AC 自动机（二次加强版）
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P5357
// Memory Limit: 256 MB
// Time Limit: 1000 ms
//
// Powered by CP Editor (https://cpeditor.org)

#include <bits/stdc++.h>

#define CPPIO \
  std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

#ifdef BACKLIGHT
#include "debug.h"
#else
#define logd(...) ;
#define ASSERT(x) ;
#define serialize() std::string("")
#endif

using i64 = int64_t;
using u64 = uint64_t;

void Initialize();
void SolveCase(int Case);

int main(int argc, char* argv[]) {
  CPPIO;
  int T = 1;
  // std::cin >> T;
  for (int t = 1; t <= T; ++t) {
    SolveCase(t);
  }
  return 0;
}

void Initialize() {}

template <int alpha = 26>
class AcAutomaton {
 public:
  struct Node {
   public:
    Node() : child_({nullptr}), trans_({nullptr}), fail_(nullptr) {
      cnt_ = 0;
      indeg_ = 0;
    }

    ~Node() {
      for (int i = 0; i < alpha; ++i) {
        if (child_[i]) {
          delete child_[i];
          child_[i] = nullptr;
        }
      }
    }

   public:
    std::array<Node*, alpha> child_;
    std::array<Node*, alpha> trans_;
    Node* fail_;

   public:
    std::vector<int> ids_;
    int cnt_;
    int indeg_;
  };

 private:
  void traverse(const std::function<void(Node* p)>& f) {
    std::queue<Node*> q;
    q.push(root_);
    while (!q.empty()) {
      Node* p = q.front();
      q.pop();

      f(p);

      for (int c = 0; c < alpha; ++c) {
        if (p->child_[c]) {
          q.push(p->child_[c]);
        }
      }
    }
  }

 public:
  void Insert(const std::vector<int>& s, int id) {
    Node* p = root_;
    for (int i = 0; i < s.size(); ++i) {
      int c = s[i];
      if (p->child_[c] == nullptr) {
        p->child_[c] = new Node();
      }
      p = p->child_[c];
    }
    p->ids_.push_back(id);
  }

  void Build() {
    std::queue<Node*> q;
    q.push(root_);

    while (!q.empty()) {
      Node* p = q.front();
      q.pop();

      for (int c = 0; c < alpha; ++c) {
        if (p->child_[c]) {
          p->trans_[c] = p->child_[c];

          if (p == root_)
            p->trans_[c]->fail_ = root_;
          else
            p->trans_[c]->fail_ = p->fail_->trans_[c];

          q.push(p->child_[c]);
        } else {
          if (p == root_)
            p->trans_[c] = root_;
          else
            p->trans_[c] = p->fail_->trans_[c];
        }
      }
    }
  }

  std::vector<int> Query(int n, const std::vector<int>& s) {
    std::vector<int> count(n, 0);

    Node* p = root_;
    int m = s.size();
    for (int i = 0; i < m; ++i) {
      int c = s[i];
      p = p->trans_[c];

      ++p->cnt_;
    }

    traverse([](Node* p) {
      if (p->fail_)
        ++p->fail_->indeg_;
    });

    std::queue<Node*> q;
    traverse([&q](Node* p) {
      if (p->indeg_ == 0) {
        q.push(p);
      }
    });

    while (!q.empty()) {
      Node* p = q.front();
      q.pop();

      if (!p->ids_.empty()) {
        for (int id : p->ids_)
          count[id] = p->cnt_;
      }

      if (p->fail_) {
        --p->fail_->indeg_;
        if (p->fail_->indeg_ == 0)
          q.push(p->fail_);

        p->fail_->cnt_ += p->cnt_;
      }
    }

    return count;
  }

 public:
  AcAutomaton() : root_(new Node()) {}

  ~AcAutomaton() { delete root_; }

 private:
  Node* root_;
};
using AcAM = AcAutomaton<26>;

void SolveCase(int Case) {
  auto ToVec = [](const std::string& s) {
    int n = s.size();
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i)
      a[i] = s[i] - 'a';
    return a;
  };

  int n;
  std::cin >> n;

  AcAM ac;
  std::string s;
  for (int i = 0; i < n; ++i) {
    std::cin >> s;
    ac.Insert(ToVec(s), i);
  }
  ac.Build();

  std::cin >> s;
  std::vector<int> count = ac.Query(n, ToVec(s));

  for (int i = 0; i < n; ++i)
    std::cout << count[i] << "\n";
}
