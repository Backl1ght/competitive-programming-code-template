// Problem: P8306 【模板】字典树
// Contest: Luogu
// URL: https://www.luogu.com.cn/problem/P8306
// Memory Limit: 1024 MB
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
  std::cin >> T;
  for (int t = 1; t <= T; ++t) {
    SolveCase(t);
  }
  return 0;
}

void Initialize() {}

template <int alpha = 26>
class Trie {
 public:
  struct Node {
   public:
    Node() : child_({nullptr}) { pass_ = 0; }

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

   public:
    int pass_;
  };

 public:
  void Insert(const std::vector<int>& s) {
    Node* p = root_;
    for (int i = 0; i < s.size(); ++i) {
      int c = s[i];
      if (p->child_[c] == nullptr) {
        p->child_[c] = new Node();
      }
      p = p->child_[c];

      ++p->pass_;
    }
  }

  int Query(const std::vector<int>& s) {
    Node* p = root_;
    int n = s.size();
    for (int i = 0; i < n; ++i) {
      int c = s[i];

      if (p->child_[c] == nullptr)
        return 0;

      p = p->child_[c];
    }

    return p->pass_;
  }

 public:
  Trie() : root_(new Node()) {}

  ~Trie() { delete root_; }

 private:
  Node* root_;
};

std::vector<int> ToVec(const std::string& s) {
  int n = s.size();
  std::vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    int c;
    if (isdigit(s[i]))
      c = s[i] - '0';
    else if (islower(s[i]))
      c = 10 + s[i] - 'a';
    else
      c = 36 + s[i] - 'A';
    a[i] = c;
  }
  return a;
}

void SolveCase(int Case) {
  int n, q;
  std::cin >> n >> q;

  Trie<62> trie;
  std::string s;
  for (int i = 0; i < n; ++i) {
    std::cin >> s;
    trie.Insert(ToVec(s));
  }

  for (int i = 0; i < q; ++i) {
    std::cin >> s;
    std::cout << trie.Query(ToVec(s)) << "\n";
  }
}
