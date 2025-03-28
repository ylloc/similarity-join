#include <compacter.h>

#include <functional>
#include <cmath>

StringCompacter::StringCompacter(StringCompacterConfig config, LinearFunction hasher)
        : config_(config), hasher_(std::move(hasher)) {}

std::string StringCompacter::Compact(std::string_view view) const {
  std::string result;
  size_t final_size = (1 << config_.compact_size) - 1;
  result.reserve(final_size);
  CompactRecursive(view, result, config_.compact_size);

  // padding
  while (result.size() < final_size) {
    result.push_back('_');
  }

  return result;
}

size_t StringCompacter::Length() const {
  return (1 << config_.compact_size) - 1;
}

void StringCompacter::CompactRecursive(std::string_view view, std::string &result, size_t level) const {
  if (level == 0 || view.empty()) {
    return;
  }

  size_t size = view.size();

  auto left = static_cast<size_t>(std::ceil((0.5 - config_.epsilon) * static_cast<double>(size)));
  auto right = static_cast<size_t>(std::floor((0.5 + config_.epsilon) * static_cast<double>(size)));

  left = std::min(left, size - 1);
  right = std::min(right, size - 1);

  auto substr = view.substr(left, right - left + 1);

  auto pivot_it = std::min_element(substr.begin(), substr.end(), [&](char first, char second) {
    return hasher_(first) < hasher_(second);
  });

  size_t pivot_index = left + std::distance(substr.begin(), pivot_it);

  result.push_back(*pivot_it);

  CompactRecursive(view.substr(0, pivot_index), result, level - 1);
  CompactRecursive(view.substr(pivot_index + 1), result, level - 1);
}