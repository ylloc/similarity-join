#include <algorithms/string_algorithms.h>

#include <vector>
#include <mdspan>

size_t EditDistance(std::string_view s1, std::string_view s2) {
  const size_t n = s1.size();
  const size_t m = s2.size();

  std::vector<size_t> buffer((n + 1) * (m + 1), 0);

  std::mdspan dp(buffer.data(), (n + 1), (m + 1));

  for (std::size_t i = 0; i <= n; ++i) {
    dp[i, 0] = i;
  }
  for (std::size_t j = 0; j <= m; ++j) {
    dp[0, j] = j;
  }

  for (size_t i = 1; i <= n; ++i) {
    for (size_t j = 1; j <= m; ++j) {
      if (s1[i - 1] == s2[j - 1]) {
        dp[i, j] = dp[i - 1, j - 1];
      } else {
        dp[i, j] = std::min({dp[i - 1, j] + 1, dp[i, j - 1] + 1, dp[i - 1, j - 1] + 1});
      }
    }
  }

  return dp[n, m];
}