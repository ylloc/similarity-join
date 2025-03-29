#pragma once

#include <compacter.h>

#include <unordered_map>
#include <vector>
#include <memory>

static constexpr char kEmptySymbol = '_';

class TrieIndex {
public:
  TrieIndex(StringCompacter compacter);

  void Insert(std::string string);
  void InsertCompactedString(std::string sketch, std::string string);

  std::vector<std::string_view> Search(std::string_view query, size_t threshold) const;
  std::vector<std::string_view> SearchCompactedQuery(std::string_view query, size_t threshold) const;

private:
  struct TrieNode {
    explicit TrieNode(char c) : character(c) {
    }

    bool IsLeaf() const {
      return children.empty();
    }

    char character{kEmptySymbol};
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    std::vector<std::string> record_list;
  };

  void TrieSearch(std::string_view query, const TrieNode *node, size_t current_index, size_t alpha, size_t mark,
                  std::vector<std::string_view> &results) const;

  static std::vector<std::string_view>
  LengthFilter(const std::vector<std::string_view> &result, size_t length,
               double length_threshold = 0.2) {
    std::vector<std::string_view> output;
    output.reserve(result.size());
    for (const auto &candidate: result) {
      size_t candidate_size = candidate.size();
      size_t right = static_cast<size_t>((1. + length_threshold) * static_cast<double>(length));
      size_t left = static_cast<size_t>((1. - length_threshold) * static_cast<double>(length));
      if (candidate_size <= right && candidate_size >= left) {
        output.push_back(candidate);
      }
    }
    return output;
  }

  static std::vector<std::string_view>
  PositionFilter(const std::vector<std::string_view> &result,
                 double position_threshold = 0.2) {
    // todo
    return result;
  }

private:
  // used for short and fixed-size representation of strings in tree
  StringCompacter compacter_;
  // fake root of the tree
  std::unique_ptr<TrieNode> root_;
};