#pragma once

#include <hashing.h>

#include <string>
#include <string_view>

static constexpr double kDefaultEpsilon = 0.1;
static constexpr size_t kDefaultCompactSize = 4;

struct StringCompacterConfig {
  /// the interval will be [n*(0.5-eps), n*(0.5+eps)]
  double epsilon{kDefaultEpsilon};
  /// size compacted size will be 2^compact_size-1
  size_t compact_size{kDefaultCompactSize};
};

class StringCompacter {
public:
  StringCompacter(StringCompacterConfig config, LinearFunction hasher = DefaultLinearFunction());

  std::string Compact(std::string_view view) const;

  size_t Length() const;

private:
  void CompactRecursive(std::string_view view, std::string &result, size_t level) const;

private:
  StringCompacterConfig config_;
  LinearFunction hasher_;
};