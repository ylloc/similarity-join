#include <trie_index.h>

TrieIndex::TrieIndex(StringCompacter compacter)
        : compacter_(std::move(compacter)),
          root_(std::make_unique<TrieNode>(kEmptySymbol)) {
}

void TrieIndex::Insert(std::string str) {
  std::string sketch_repr = compacter_.Compact(str);
  InsertCompactedString(std::move(sketch_repr), std::move(str));
}

void TrieIndex::InsertCompactedString(std::string sketch, std::string str) {
  TrieNode *node = root_.get();

  for (char c: sketch) {
    if (!node->children.contains(c)) {
      node->children[c] = std::make_unique<TrieNode>(c);
    }
    node = node->children[c].get();
  }

  node->record_list.push_back(std::move(str));
}

std::vector<std::string_view> TrieIndex::Search(std::string_view query, size_t threshold) const {
  std::string sketch = compacter_.Compact(query);
  auto search_result = SearchCompactedQuery(std::move(sketch), threshold);
  auto length_filtered_result = LengthFilter(search_result, query.size(), 0.2);
  auto position_filtered_result = PositionFilter(length_filtered_result, query.size());
  return position_filtered_result;
}

std::vector<std::string_view> TrieIndex::SearchCompactedQuery(std::string_view sketch, size_t threshold) const {
  std::vector<std::string_view> results;
  TrieSearch(sketch, root_.get(), 0, threshold, 0, results);
  return results;
}

void TrieIndex::TrieSearch(
        std::string_view query, const TrieNode *node, size_t current_index, size_t alpha, size_t mark,
        std::vector<std::string_view> &results) const {
  if (!node || mark > alpha) {
    return;
  }

  if (current_index == query.size()) {
    for (const auto &view: node->record_list) {
      results.push_back(view);
    }
    return;
  }

  for (const auto &[_, child]: node->children) {
    if (query[current_index] == child->character) {
      TrieSearch(query, child.get(), current_index + 1, alpha, mark, results);
    } else {
      TrieSearch(query, child.get(), current_index + 1, alpha, mark + 1, results);
    }
  }
}
