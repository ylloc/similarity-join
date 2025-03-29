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

  size_t pivot_index = FindPivotIndex(view);

  result.push_back(view[pivot_index]);

  CompactRecursive(view.substr(0, pivot_index), result, level - 1);
  CompactRecursive(view.substr(pivot_index + 1), result, level - 1);
}

std::vector<size_t> StringCompacter::GetPivotPositions(std::string_view view) const {
  std::vector<size_t> positions;
  positions.reserve((1 << config_.compact_size) - 1);
  GetPivotPositionsRecursive(view, 0, positions, config_.compact_size);
  return positions;
}

void StringCompacter::GetPivotPositionsRecursive(std::string_view view, size_t base_pos, std::vector<size_t> &positions,
                                                 size_t level) const {
  if (level == 0 || view.empty()) {
    return;
  }

  size_t pivot_index = FindPivotIndex(view);
  positions.push_back(base_pos + pivot_index);

  GetPivotPositionsRecursive(view.substr(0, pivot_index), base_pos, positions, level - 1);
  GetPivotPositionsRecursive(view.substr(pivot_index + 1), base_pos + pivot_index + 1, positions, level - 1);
}

size_t StringCompacter::FindPivotIndex(std::string_view view) const {
  size_t size = view.size();

  auto left = static_cast<size_t>(std::ceil((0.5 - config_.epsilon) * static_cast<double>(size)));
  auto right = static_cast<size_t>(std::floor((0.5 + config_.epsilon) * static_cast<double>(size)));

  left = std::min(left, size - 1);
  right = std::min(right, size - 1);

  auto substr = view.substr(left, right - left + 1);

  auto pivot_it = std::min_element(substr.begin(), substr.end(), [&](char first, char second) {
    return hasher_(first) < hasher_(second);
  });

  return left + std::distance(substr.begin(), pivot_it);
}
