#include <bk_tree.h>

BKTreeNode::BKTreeNode(const std::string &word) : word_(word) {}

void BKTreeNode::Add(std::string new_word) {
  size_t dist = EditDistance(word_, new_word);
  if (auto it = children_.find(dist); it == children_.end()) {
    children_[dist] = make_unique<BKTreeNode>(new_word);
  } else {
    it->second->Add(new_word);
  }
}

std::vector<std::string_view> BKTreeNode::Search(std::string_view query, size_t radius) {
  std::vector<std::string_view> result;
  SearchImpl(query, radius, result);
  return result;
}

void BKTreeNode::SearchImpl(std::string_view query, size_t radius,
                            std::vector<std::string_view> &results) const {
  int dist = EditDistance(word_, query);
  if (dist <= radius) {
    results.emplace_back(word_);
  }

  for (int i = dist - radius; i <= dist + radius; ++i) {
    auto it = children_.find(i);
    if (it != children_.end()) {
      it->second->SearchImpl(query, radius, results);
    }
  }
}

void BKTreeIndex::Insert(std::string string) {
  if (size == 0) {
    bk_tree = std::make_unique<BKTreeNode>(string);
  } else {
    bk_tree->Add(string);
  }
  size += 1;
}

std::vector<std::string_view> BKTreeIndex::Search(std::string_view query, size_t radius) {
  return bk_tree->Search(query, radius);
}
