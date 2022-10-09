/**
 * Build Suffix Array in O(n \log n) with binary lifting and radix sort.
 *
 * Idea
 *
 *   Assume that the rank of all substrings with lenght w is known, let call it
 * rank_{w}, then let rank_w(i) be the first key, and rank_w(i + w) be the
 * second key. rank_{2w} can be calculated by using radix sort.
 *
 * Reference
 *
 *   https://oi-wiki.org/string/sa/
 */
template <int alpha = 26, typename String>
std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> SuffixArray(
    const String& s) {
  int n = s.size(), m = alpha;

  /**
   * suffix(i) means s_i...s_{n-1}
   * sa[i] is start position fo the i-th smallest prefix
   * rank[i] is rank of the suffix(i)
   * lcp[i] means longest common prefix between suffix(sa[i]) and suffix(sa[i -
   * 1])
   */
  std::vector<int> sa(n), rank(n), lcp(n);
  std::vector<int> count(std::max(m, n), 0), old_rank(n), temp(n);

  // build sa with binary lifting and raidx sort.
  for (int i = 0; i < n; ++i) {
    rank[i] = s[i];
    ++count[rank[i]];
  }
  for (int i = 1; i < m; ++i) {
    count[i] += count[i - 1];
  }
  for (int i = n - 1; i >= 0; --i) {
    sa[--count[rank[i]]] = i;
  }

  for (int length = 1; length < n; length = length * 2) {
    // second key.
    int p = 0;
    for (int i = n - length; i < n; ++i)
      temp[p++] = i;
    for (int i = 0; i < n; ++i)
      if (sa[i] >= length)
        temp[p++] = sa[i] - length;

    // first key.
    std::fill(count.begin(), count.end(), 0);
    for (int i = 0; i < n; ++i)
      ++count[rank[temp[i]]];
    for (int i = 1; i < m; ++i)
      count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; --i) {
      sa[--count[rank[temp[i]]]] = temp[i];
    }

    old_rank = rank;
    m = 0;
    rank[sa[0]] = m++;
    for (int i = 1; i < n; ++i) {
      if (old_rank[sa[i]] == old_rank[sa[i - 1]] &&
          ((sa[i] + length < n ? old_rank[sa[i] + length] : -1) ==
           (sa[i - 1] + length < n ? old_rank[sa[i - 1] + length] : -1))) {
        rank[sa[i]] = m - 1;
      } else {
        rank[sa[i]] = m++;
      }
    }

    if (m == n)
      break;
  }

  // longest common prefix
  for (int i = 0, k = 0; i < n; ++i) {
    if (rank[i] == 0)
      continue;
    if (k)
      --k;
    while (i + k < n && sa[rank[i] - 1] + k < n &&
           s[i + k] == s[sa[rank[i] - 1] + k])
      ++k;
    lcp[rank[i]] = k;
  }

  return {sa, rank, lcp};
}