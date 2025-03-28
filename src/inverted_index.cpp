#include <inverted_index.h>
#include <compacter.h>

InvertedIndex::InvertedIndex(StringCompacter compacter) : compacter_(compacter) {
}

void InvertedIndex::Build(std::vector<std::string> strings) {
  size_t levels = compacter_.Length();
  index_levels_.resize(levels);

  for (auto &&original_str: std::move(strings)) {
    std::string compacted_str = compacter_.Compact(original_str);
    for (size_t j = 0; j < levels; ++j) {
      char c = compacted_str[j];
      index_levels_[j][c].push_back(std::move(original_str));
    }
  }
}

void InvertedIndex::BuildFromFile(std::ifstream &fin) {
  std::vector<std::string> strings;
  std::string line;
  while (std::getline(fin, line)) {
    if (!line.empty()) {
      strings.push_back(std::move(line));
    }
  }
  Build(std::move(strings));
}

std::vector<std::string_view> InvertedIndex::Search(std::string_view query, size_t threshold) const {
  size_t levels = compacter_.Length();
  std::unordered_map<std::string_view, size_t> frequency;

  std::string compacted_query = compacter_.Compact(query);

  for (size_t i = 0; i < levels; ++i) {
    char c = compacted_query[i];
    const auto &level_map = index_levels_[i];
    auto it = level_map.find(c);
    if (it != level_map.end()) {
      for (const auto &candidate: it->second) {
        frequency[candidate] += 1;
      }
    }
  }

  std::vector<std::string_view> results;
  for (const auto &[candidate, freq]: frequency) {
    if (freq >= levels - threshold) {
      results.emplace_back(candidate);
    }
  }

  return results;
}
