inline unsigned rng() {
    static unsigned x = 7;
    return x = x * 0xdefaced + 1;
}

struct Treap {
    struct node {
        node *l, *r;
        unsigned rnd;
        int sz, v;
        node(int _v) : l(NULL), r(NULL), rnd(rng()), sz(1), v(_v) {}
    };

    inline int get_size(node* &p) {
        return p ? p->sz : 0;
    }

    inline void push_up(node* & p) {
        if (!p) return;
        p->sz = get_size(p->l) + get_size(p->r) + 1;
    }

    node* root;

    node* merge(node* a, node* b) {
        if (!a) return b;
        if (!b) return a;
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

    void split_val(node* p, const int& k, node* &a, node* &b) {
        if (!p) a = b = NULL;
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

    void split_size(node* p, int k, node*& a, node*&b){
        if(!p) a = b = NULL;
        else{
            if(get_size(p->l) <= k){
                a = p;
                split_size(p->r, k - get_size(p->l) - 1, a->r, b);
                push_up(a);
            }
            else{
                b = p;
                split_size(p->l, k, a, b->l);
                push_up(b);
            }
        }
    }

    void ins(int val) {
        node *a, *b;
        split_val(root, val, a, b);
        a = merge(a, new node(val));
        root = merge(a, b);
    }

    void del(int val) {
        node *a, *b, *c, *d;
        split_val(root, val, a, b);
        split_val(a, val - 1, c, d);
        delete d;
        root = merge(c, b);
    }

    int qry(int val) {
        node *a, *b;
        split_val(root, val, a, b);
        int res = get_size(a);
        root = merge(a, b);
        return res;
    }

    int qry(int l, int r) { return qry(r) - qry(l - 1); }
};