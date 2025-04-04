#include <inverted_index.h>
#include <algorithm>
#include <cmath>

InvertedIndex::InvertedIndex(StringCompacter compacter, InvertedIndexConfig config)
        : compacter_(std::move(compacter)), config_(config) {}

void InvertedIndex::Build(std::vector<std::string> strings) {
  size_t levels = compacter_.Length();
  index_levels_.resize(levels);

  for (auto &&original_str: strings) {
    std::string compacted_str = compacter_.Compact(original_str);

    Record record{
            compacted_str,
            std::move(original_str),
            compacted_str.size(),
            {} // todo: support pivot positions
    };

    for (size_t j = 0; j < levels; ++j) {
      char c = compacted_str[j];
      index_levels_[j][c].push_back(record);
    }
  }

  for (auto &level: index_levels_) {
    for (auto &[_, records]: level) {
      std::sort(records.begin(), records.end(),
                [](const Record &a, const Record &b) {
                  return a.length < b.length;
                });
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
  size_t query_length = query.size();

  [[maybe_unused]]  size_t min_length = static_cast<size_t>(std::ceil((1 - config_.epsilon) * query_length));
  [[maybe_unused]] size_t max_length = static_cast<size_t>(std::floor((1 + config_.epsilon) * query_length));

  for (size_t i = 0; i < levels; ++i) {
    char c = compacted_query[i];
    const auto &level_map = index_levels_[i];
    auto it = level_map.find(c);
    if (it != level_map.end()) {
      const auto &records = it->second;

      for (auto rec_it = records.begin(); rec_it != records.end(); ++rec_it) {
        frequency[rec_it->original_str] += 1;
      }
    }
  }

  std::vector<std::string_view> results;
  for (const auto &[candidate, freq]: frequency) {
    if (levels <= threshold + freq) {
      results.emplace_back(candidate);
    }
  }

  return results;
}
