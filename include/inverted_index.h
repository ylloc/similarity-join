#pragma once

#include <compacter.h>
#include <index.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <fstream>

class InvertedIndex {
  using RecordList = std::vector<std::string>;

public:
  explicit InvertedIndex(StringCompacter compacter);

  void BuildFromFile(std::ifstream &fin);

  void Build(std::vector<std::string> strings);

  std::vector<std::string_view> Search(std::string_view query_sketch, size_t threshold) const;

private:
  StringCompacter compacter_;
  std::vector<std::unordered_map<char, RecordList>> index_levels_;
};