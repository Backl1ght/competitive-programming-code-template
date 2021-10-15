template <typename T>
struct rbtree {
  struct node {
    T val;
    int sz, cnt;
    node *l, *r, *p;
    bool color;
  };
  node buf[N << 3], *s = buf;
  node* nil = ++s;
  node* root = nil;
  node* find_min(node* x) {
    while (x->l != nil)
      x = x->l;
    return x;
  }
  node* find_max(node* x) {
    while (x->r != nil)
      x = x->r;
    return x;
  }
  node* find_node(const T& val) {
    node* x = root;
    while (x != nil) {
      if (x->val == val)
        return x;
      if (x->val < val)
        x = x->r;
      else
        x = x->l;
    }
    return NULL;
  }
  void zig(node* x) {
    node* y = x->r;
    x->r = y->l;
    if (y->l != nil)
      y->l->p = x;
    y->p = x->p;
    if (x->p == nil)
      root = y;
    else if (x == x->p->r)
      x->p->r = y;
    else
      x->p->l = y;
    y->l = x;
    x->p = y;
    y->sz = x->sz;
    x->sz = x->l->sz + x->r->sz + x->cnt;
    return;
  }
  void zag(node* x) {
    node* y = x->l;
    x->l = y->r;
    if (y->r != nil)
      y->r->p = x;
    y->p = x->p;
    if (x->p == nil)
      root = y;
    else if (x == x->p->l)
      x->p->l = y;
    else
      x->p->r = y;
    y->r = x;
    x->p = y;
    y->sz = x->sz;
    x->sz = x->l->sz + x->r->sz + x->cnt;
    return;
  }
  void insert_fixup(node* z) {
    while (z->p->color == 1) {
      if (z->p == z->p->p->l) {
        node* y = z->p->p->r;
        if (y->color == 1) {
          y->color = z->p->color = 0;
          z->p->p->color = 1;
          z = z->p->p;
        } else {
          if (z == z->p->r) {
            z = z->p;
            zig(z);
          }
          z->p->color = 0;
          z->p->p->color = 1;
          zag(z->p->p);
        }
      } else {
        node* y = z->p->p->l;
        if (y->color == 1) {
          y->color = z->p->color = 0;
          z->p->p->color = 1;
          z = z->p->p;
        } else {
          if (z == z->p->l) {
            z = z->p;
            zag(z);
          }
          z->p->color = 0;
          z->p->p->color = 1;
          zig(z->p->p);
        }
      }
    }
    root->color = 0;
    return;
  }
  void transplant(node* x, node* y) {
    y->p = x->p;
    if (x->p == nil)
      root = y;
    else if (x == x->p->l)
      x->p->l = y;
    else
      x->p->r = y;
    return;
  }
  void delete_fixup(node* x) {
    while (x != root && x->color == 0) {
      if (x == x->p->l) {
        node* w = x->p->r;
        if (w->color == 1) {
          x->p->color = 1;
          w->color = 0;
          zig(x->p);
          w = x->p->r;
        }
        if (w->l->color == 0 && w->r->color == 0) {
          w->color = 1;
          x = x->p;
        } else {
          if (w->r->color == 0) {
            w->color = 1;
            w->l->color = 0;
            zag(w);
            w = x->p->r;
          }
          w->color = x->p->color;
          x->p->color = 0;
          w->r->color = 0;
          zig(w->p);
          x = root;
        }
      } else {
        node* w = x->p->l;
        if (w->color == 1) {
          x->p->color = 1;
          w->color = 0;
          zag(x->p);
          w = x->p->l;
        }
        if (w->r->color == 0 && w->l->color == 0) {
          w->color = 1;
          x = x->p;
        } else {
          if (w->l->color == 0) {
            w->color = 1;
            w->r->color = 0;
            zig(w);
            w = x->p->l;
          }
          w->color = x->p->color;
          x->p->color = 0;
          w->l->color = 0;
          zag(w->p);
          x = root;
        }
      }
    }
    x->color = 0;
    return;
  }
  void ins(const T& val) {
    node* x = root;
    node* y = nil;
    while (x != nil) {
      y = x;
      ++y->sz;
      if (x->val == val) {
        ++x->cnt;
        return;
      }
      if (x->val < val)
        x = x->r;
      else
        x = x->l;
    }
    node* z = ++s;
    *z = (node){val, 1, 1, nil, nil, y, 1};
    if (y == nil)
      root = z;
    else {
      if (y->val < val)
        y->r = z;
      else
        y->l = z;
    }
    insert_fixup(z);
    return;
  }
  void del(const T& val) {
    node* z = root;
    node* w = nil;
    while (z != nil) {
      w = z;
      --w->sz;
      if (z->val == val)
        break;
      if (z->val < val)
        z = z->r;
      else
        z = z->l;
    }
    if (z != nil) {
      // delete only one node
      if (z->cnt > 1) {
        --z->cnt;
        return;
      }

      node* y = z;
      node* x;
      bool history = y->color;
      if (z->l == nil) {
        x = z->r;
        transplant(z, z->r);
      } else if (z->r == nil) {
        x = z->l;
        transplant(z, z->l);
      } else {
        y = find_min(z->r);
        history = y->color;
        x = y->r;
        if (y->p == z)
          x->p = y;
        else {
          node* w = y;
          while (w != z) {
            w->sz -= y->cnt;
            w = w->p;
          }
          transplant(y, y->r);
          y->r = z->r;
          y->r->p = y;
        }
        transplant(z, y);
        y->l = z->l;
        y->l->p = y;
        y->color = z->color;
        y->sz = y->l->sz + y->r->sz + y->cnt;
      }
      if (history == 0)
        delete_fixup(x);
    } else
      while (w != nil) {
        ++w->sz;
        w = w->p;
      }
    return;
  }
  T getKth(int k) {
    T res = 0;
    node* x = root;
    while (x != nil) {
      if (x->l->sz + 1 <= k && x->l->sz + x->cnt >= k) {
        res = x->val;
        break;
      } else if (x->l->sz + x->cnt < k) {
        k -= x->l->sz + x->cnt;
        x = x->r;
      } else {
        x = x->l;
      }
    }
    return res;
  }
  int getRank(const T& val) {
    int rk = 0;
    node* x = root;
    while (x != nil) {
      if (x->val < val) {
        rk += x->l->sz + x->cnt;
        x = x->r;
      } else {
        if (x->val == val)
          ++rk;
        x = x->l;
      }
    }
    return rk;
  }
  T getSucc(const T& val) {
    ins(val);
    T res = INT_MAX;
    node* x = find_node(val);
    if (x->r != nil) {
      res = find_min(x->r)->val;
    } else {
      while (x->p->r == x)
        x = x->p;
      if (x->p != nil)
        res = x->p->val;
    }
    del(val);
    return res;
  }
  T getPrev(const T& val) {
    ins(val);
    T res = INT_MIN;
    node* x = find_node(val);
    if (x->l != nil)
      res = find_max(x->l)->val;
    else {
      while (x->p->l == x)
        x = x->p;
      if (x->p != nil)
        res = x->p->val;
    }
    del(val);
    return res;
  }
};