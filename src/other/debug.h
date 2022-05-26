#include <bits/stdc++.h>
#include "to_string.h"

template <typename T>
inline void _logd(const char* format, T value) {
  std::cerr << format << '=' << to_string(value) << std::endl;
}

template <typename First, typename... Rest>
inline void _logd(const char* format, First f, Rest... r) {
  while (*format != ',')
    std::cerr << *format++;
  std::cerr << '=' << to_string(f) << ",";
  _logd(format + 1, r...);
}

#define logd(...)                \
  std::cerr << __LINE__ << ": "; \
  _logd(#__VA_ARGS__, __VA_ARGS__);

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
static StackLimitHelper _(256);
}  // namespace LocalStackLimit