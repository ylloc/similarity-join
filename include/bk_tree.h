#pragma once

#include <string_algorithms.h>

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

/// Naive tree index, but in practice it is faster then O(size) per query
class BKTreeNode {
public:
  explicit BKTreeNode(const std::string &word);

  void Add(std::string new_word);


  std::vector<std::string_view> Search(std::string_view query, size_t radius);

private:
  void SearchImpl(std::string_view query, size_t radius,
                  std::vector<std::string_view> &results) const;

private:
  std::string word_;
  std::unordered_map<int, std::unique_ptr<BKTreeNode>> children_;
};

class BKTreeIndex {
public:
  BKTreeIndex() = default;

  void Insert(std::string string);

  std::vector<std::string_view> Search(std::string_view query, size_t radius);

private:
  std::unique_ptr<BKTreeNode> bk_tree;
  size_t size{0};
};