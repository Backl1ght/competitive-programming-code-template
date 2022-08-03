#include <bits/stdc++.h>
#include "to_string.h"

template <typename T>
inline void logd_impl(const char* format, T value) {
  std::cerr << format << '=' << serialize(value) << std::endl;
}

template <typename First, typename... Rest>
inline void logd_impl(const char* format, First f, Rest... r) {
  while (*format != ',')
    std::cerr << *format++;
  std::cerr << '=' << serialize(f) << ",";
  logd_impl(format + 1, r...);
}

#define logd(...)                \
  std::cerr << __LINE__ << ": "; \
  logd_impl(#__VA_ARGS__, __VA_ARGS__);

#if defined(__linux__)
namespace LocalStackLimit {
#include <sys/resource.h>
class StackLimitHelper {
 public:
  StackLimitHelper(int limit) {
    // set min stack size to $limit MB
    const rlim_t kStackSize = limit * 1024 * 1024;
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0) {
      if (rl.rlim_cur < kStackSize) {
        rl.rlim_cur = kStackSize;
        result = setrlimit(RLIMIT_STACK, &rl);
        if (result != 0) {
          fprintf(stderr, "setrlimit returned result = %d\n", result);
        }
      }
    }
    fprintf(stderr, "set min stack size to %lu MB\n", kStackSize / 1024 / 1024);
  }
};
#endif
static StackLimitHelper _(256);
}  // namespace LocalStackLimit