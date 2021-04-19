/**
 * BFPRT: find the kth element of an array in $ O(n) $ using Divide and Conquer method.
 * you can use std::nth_element(a, a + k, a + n) instead
**/
namespace BFPRT {
    template<typename T, typename Cmp>
    T kth_index(T* a, int l, int r, int k, Cmp cmp);

    template<typename T, typename Cmp>
    int insert_sort(T* a, int l, int r, Cmp cmp) {
        for (int i = l + 1; i <= r; ++i) {
            int tmp = a[i];
            int j = i - 1;
            while(j >= l && a[j] > tmp) {
                a[j + 1] = a[j];
                --j;
            }
            a[j + 1] = tmp;
        }
        return l + (r - l) / 2;
    }

    template<typename T, typename Cmp>
    int pivot(T* a, int l, int r, Cmp cmp) {
        if (r - l < 5) return insert_sort(a, l, r, cmp);
        int lst = l - 1;
        for (int i = l; i + 4 <= r; i += 5) {
            int p = insert_sort(a, i, i + 4, cmp);
            swap(a[++lst], a[p]);
        }
        return kth_index<T>(a, l, lst, (lst - l + 1) / 2 + 1, cmp);
    }

    template<typename T, typename Cmp>
    int partition(T* a, int l, int r, Cmp cmp) {
        int p = pivot(a, l, r, cmp);
        swap(a[p], a[r]);
        int lst = l - 1;
        for (int i = l; i < r; ++i) {
            if (cmp(a[i], a[r])) swap(a[++lst], a[i]);
        }
        swap(a[++lst], a[r]);
        return lst;
    }

    template<typename T, typename Cmp>
    T kth_index(T* a, int l, int r, int k, Cmp cmp) {
        int p = partition(a, l, r, cmp);
        int d = p - l + 1;
        if (d == k) return p;
        else if (d < k) return kth_index(a, p + 1, r, k - d, cmp);
        else return kth_index(a, l, p - 1, k, cmp);
    }

    template<typename T>
    T kth_index(T* a, int l, int r, int k) {
        return kth_index(a, l, r, k, less<T>());
    }
};