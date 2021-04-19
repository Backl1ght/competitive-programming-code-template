/**
 * BFPRT: find the kth element of an array in $ O(n) $ using Divide and Conquer method.
 * you can use std::nth_element(a, a + k, a + n) instead
**/
namespace BFPRT {
    template<typename T, typename Cmp>
    int medianOf3(T* a, int i, int j, int k, Cmp cmp) {
        if (cmp(a[i], a[j])) {
            if (cmp(a[j], a[k])) return j;
            else if (cmp(a[i], a[k])) return k; 
            else return i;
        }
        if (cmp(a[i], a[k])) return i;
        else if (cmp(a[j], a[k])) return k;
        else return j;
    }

    template<typename T, typename Cmp>
    int pivot(T* a, int l, int r, Cmp cmp) {
        int lst = l - 1;
        for (int i = l; i + 2 <= r; i += 3) {
            int p = medianOf3(a, i, i + 1, i + 2, cmp);
            swap(a[++lst], a[p]);
        }
        return l + (lst - l) / 2;
    }

    template<typename T, typename Cmp>
    int partition(T* a, int l, int r, Cmp cmp) {
        int p = pivot(a, l, r, cmp);
        swap(a[p], a[r]);
        int lst = l - 1;
        for (int i = l; i < r; ++i) {
            if (a[i] <= a[r]) swap(a[++lst], a[i]);
        }
        swap(a[++lst], a[r]);
        return lst;
    }

    template<typename T, typename Cmp>
    T kth(T* a, int l, int r, int k, Cmp cmp) {
        int p = partition(a, l, r, cmp);
        int d = p - l + 1;
        if (d == k) return a[p];
        else if (d < k) return kth(a, p + 1, r, k - d, cmp);
        else return kth(a, l, p - 1, k, cmp);
    }

    template<typename T>
    T kth(T* a, int l, int r, int k) {
        return kth(a, l, r, k, less<T>());
    }
};
