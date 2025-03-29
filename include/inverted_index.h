#pragma once

#include <compacter.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <fstream>

static constexpr double kDefaultIndexEpsilon = 0.1;

struct InvertedIndexConfig {
  /// the interval will be [(1-epsilon)*|q|, (1+epsilon)*|q|]
  double epsilon{kDefaultIndexEpsilon};
};

struct Record {
  std::string original_str;
  size_t length;
  std::vector<size_t> pivot_positions;
};

using RecordList = std::vector<Record>;

class InvertedIndex {
public:
  explicit InvertedIndex(StringCompacter compacter, InvertedIndexConfig config = {});

  void BuildFromFile(std::ifstream &fin);

  void Build(std::vector<std::string> strings);

  std::vector<std::string_view> Search(std::string_view query, size_t threshold) const;

private:
  StringCompacter compacter_;
  InvertedIndexConfig config_;
  std::vector<std::unordered_map<char, RecordList>> index_levels_;
};
