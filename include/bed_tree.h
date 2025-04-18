#pragma once

#include <string>
#include <vector>
#include <unordered_map>


/// 1-st step: string -> n-grams
/// n-grams -> L-bucket representation
/// L-bucket -> keys on B+-tree
/// dfs on B+-tree

std::vector<int> Compact(std::string_view str, int bound) {

  std::vector<int> size(str.size() - bound + 1);
  std::unordered_map<std::string_view, int> count;

  for (int i = 0; i < str.size() - bound; ++i) {
    count[str.substr(i, bound)] += 1;
  }

  return {};
}