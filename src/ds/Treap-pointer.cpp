// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
// inline unsigned rng() {
//     static unsigned x = 7;
//     return x = x * 0xdefaced + 1;
// }

template <typename T>
struct Treap {
    struct node {
        node *l, *r;
        unsigned rnd;
        T v;
        int sz;
        node(T _v)
            : l(NULL)
            , r(NULL)
            , rnd(rng())
            , sz(1)
            , v(_v)
        {
        }
    };

    inline int get_size(node*& p)
    {
        return p ? p->sz : 0;
    }

    inline void push_up(node*& p)
    {
        if (!p)
            return;
        p->sz = get_size(p->l) + get_size(p->r) + 1;
    }

    node* root = NULL;

    node* merge(node* a, node* b)
    {
        if (!a)
            return b;
        if (!b)
            return a;
        if (a->rnd < b->rnd) {
            a->r = merge(a->r, b);
            push_up(a);
            return a;
        } else {
            b->l = merge(a, b->l);
            push_up(b);
            return b;
        }
    }

    void split_val(node* p, const T& k, node*& a, node*& b)
    {
        if (!p)
            a = b = NULL;
        else {
            if (p->v <= k) {
                a = p;
                split_val(p->r, k, a->r, b);
                push_up(a);
            } else {
                b = p;
                split_val(p->l, k, a, b->l);
                push_up(b);
            }
        }
    }

    void split_size(node* p, int k, node*& a, node*& b)
    {
        if (!p)
            a = b = NULL;
        else {
            if (get_size(p->l) <= k) {
                a = p;
                split_size(p->r, k - get_size(p->l) - 1, a->r, b);
                push_up(a);
            } else {
                b = p;
                split_size(p->l, k, a, b->l);
                push_up(b);
            }
        }
    }

    void ins(T val)
    {
        node *a, *b;
        split_val(root, val, a, b);
        a = merge(a, new node(val));
        root = merge(a, b);
    }

    void del(T val)
    {
        node *a, *b, *c, *d;
        split_val(root, val, a, b);
        split_val(a, val - 1, c, d);
        node* e = d;
        d = merge(d->l, d->r);
        delete e;
        a = merge(c, d);
        root = merge(a, b);
    }

    T getRank(T val)
    {
        node *a, *b;
        split_val(root, val - 1, a, b);
        T res = get_size(a) + 1;
        root = merge(a, b);
        return res;
    }

    T getKth(int k)
    {
        node* x = root;
        T res = numeric_limits<T>::min();
        while (x) {
            if (k <= get_size(x->l))
                x = x->l;
            else {
                if (get_size(x->l) + 1 == k) {
                    res = x->v;
                    break;
                } else {
                    k -= get_size(x->l) + 1;
                    x = x->r;
                }
            }
        }
        return res;
    }

    T getPrev(T val)
    {
        node *a, *b;
        split_val(root, val - 1, a, b);
        node* p = a;
        while (p->r)
            p = p->r;
        root = merge(a, b);
        return p->v;
    }

    T getSucc(T val)
    {
        node *a, *b;
        split_val(root, val, a, b);
        node* p = b;
        while (p->l)
            p = p->l;
        root = merge(a, b);
        return p->v;
    }
};