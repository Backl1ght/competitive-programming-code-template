namespace STLWrapper {

template <typename T, typename... Args>
auto n_vector(size_t n, Args&&... args) {
  if constexpr (sizeof...(args) == 1) {
    return std::vector<T>(n, args...);
  } else {
    return std::vector(n, n_vector<T>(args...));
  }
}

template <typename T>
using min_heap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

template <typename T>
using max_heap = std::priority_queue<T, std::vector<T>, std::less<T>>;

}  // namespace STLWrapper