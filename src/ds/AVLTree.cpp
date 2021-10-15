#include <bits/stdc++.h>
using namespace std;

#define FOR(i, l, r) for (int i = l; i <= r; ++i)
#define ROF(i, r, l) for (int i = r; i >= l; --i)
#define REP(i, l, r) for (int i = l; i < r; ++i)
#define PER(i, r, l) for (int i = r - 1; i >= l; --i)

const int N = 1e5 + 5;
using ll = long long;

template <typename T>
struct AVLTree {
  struct node {
    T v;
    int sz, h, cnt;
    node *l, *r;
    explicit node(T _v)
        : v(_v) {
      sz = h = cnt = 1;
      l = r = nullptr;
    }
  };
  node* root = nullptr;
  int get_size(node* p) {
    return p ? p->sz : 0;
  }

  int get_height(node* p) {
    return p ? p->h : 0;
  }

  void push_up(node* p) {
    if (!p)
      return;
    p->sz = get_size(p->l) + p->cnt + get_size(p->r);
    p->h = 1 + max(get_height(p->l), get_height(p->r));
  }

  void zig(node*& p) {
    node* q = p;
    q = p->r;
    p->r = q->l;
    q->l = p;
    push_up(p);
    push_up(q);
    p = q;
  }

  void zag(node*& p) {
    node* q = p->l;
    p->l = q->r;
    q->r = p;
    push_up(p);
    push_up(q);
    p = q;
  }

  void maintain(node*& p) {
    if (!p)
      return;
    if (get_height(p->l) - get_height(p->r) == -2) {
      if (p->r && get_height(p->r->l) > get_height(p->r->r)) {
        zag(p->r);
      }
      zig(p);
      return;
    }
    if (get_height(p->l) - get_height(p->r) == 2) {
      if (p->l && get_height(p->l->l) < get_height(p->l->r)) {
        zig(p->l);
      }
      zag(p);
      return;
    }
  }

  void ins(node*& p, T v) {
    if (!p) {
      p = new node(v);
      return;
    }
    if (p->v == v) {
      ++(p->cnt);
    } else {
      if (v < p->v) {
        ins(p->l, v);
      } else {
        ins(p->r, v);
      }
    }
    push_up(p);
    maintain(p);
    push_up(p);
  }

  void del(node*& p, T v) {
    if (!p)
      return;
    if (p->v == v) {
      --p->cnt;
      if (p->cnt == 0) {
        if (p->l && p->r) {
          node* q = p->r;
          while (q->l)
            q = q->l;
          p->cnt = q->cnt, p->v = q->v;
          q->cnt = 1;
          del(p->r, q->v);
        } else {
          node* q = p;
          if (p->l)
            p = p->l;
          else if (p->r)
            p = p->r;
          else
            p = nullptr;
          delete q;
          q = nullptr;
        }
      }
    } else {
      if (v < p->v)
        del(p->l, v);
      else
        del(p->r, v);
    }
    push_up(p);
    maintain(p);
    push_up(p);
  }

  void ins(T v) {
    ins(root, v);
  }

  void del(T v) {
    del(root, v);
  }

  int getRank(T v) {
    node* p = root;
    int res = 0;
    while (p) {
      if (v == p->v) {
        res += get_size(p->l);
        break;
      }
      if (v < p->v)
        p = p->l;
      else {
        res += get_size(p->l) + p->cnt;
        p = p->r;
      }
    }
    return res + 1;
  }

  T getKth(int k) {
    node* p = root;
    T res = -1;
    while (p) {
      if (k <= get_size(p->l))
        p = p->l;
      else if (k - get_size(p->l) <= p->cnt) {
        res = p->v;
        break;
      } else {
        k -= get_size(p->l) + p->cnt;
        p = p->r;
      }
    }
    return res;
  }

  T getPrev(T v) {
    T res = numeric_limits<T>::min();
    node* p = root;
    while (p) {
      if (v == p->v) {
        node* q = p->l;
        if (q) {
          while (q->r)
            q = q->r;
          res = q->v;
        }
        break;
      }

      if (v < p->v) {
        p = p->l;
      } else {
        if (p->v > res)
          res = p->v;
        p = p->r;
      }
    }
    return res;
  }

  T getSucc(T v) {
    T res = numeric_limits<T>::max();
    node* p = root;
    while (p) {
      if (v == p->v) {
        node* q = p->r;
        if (q) {
          while (q->l)
            q = q->l;
          res = q->v;
        }
        break;
      }

      if (v < p->v) {
        if (p->v < res)
          res = p->v;
        p = p->l;
      } else {
        p = p->r;
      }
    }
    return res;
  }

  void debug(node* p) {
    if (!p)
      return;
    debug(p->l);
    cerr << p->v << " ";
    debug(p->r);
  }

  void debug() {
    cerr << "INORDER: " << endl;
    debug(root);
    cerr << endl;
  }
};

void solve(int Case) {
  int n;
  scanf("%d", &n);
  int op, x;
  AVLTree<int> t;
  FOR(i, 1, n) {
    scanf("%d %d", &op, &x);
    //        cerr << op << " " << x << endl;
    switch (op) {
      case 1:
        t.ins(x);
        break;
      case 2:
        t.del(x);
        break;
      case 3:
        printf("%d\n", t.getRank(x));
        break;
      case 4:
        printf("%d\n", t.getKth(x));
        break;
      case 5:
        printf("%d\n", t.getPrev(x));
        break;
      case 6:
        printf("%d\n", t.getSucc(x));
        break;
    }
    //        t.debug();
  }
}

int main() {
#ifdef BACKLIGHT
  freopen("a.in", "r", stdin);
#endif
  int T = 1;
  //    scanf("%d", &T);
  for (int _ = 1; _ <= T; ++_)
    solve(_);
  return 0;
}