#include <inverted_index.h>

#include <algorithm>
#include <cmath>

InvertedIndex::InvertedIndex(StringCompacter compacter, InvertedIndexConfig config)
        : compacter_(std::move(compacter)), config_(config) {
}

void InvertedIndex::Build(std::vector<std::string> strings) {
  size_t levels = compacter_.Length();
  index_levels_.resize(levels);

  for (auto &&original_str: std::move(strings)) {
    std::string compacted_str = compacter_.Compact(original_str);
    auto pivot_positions = compacter_.GetPivotPositions(original_str);

    Record record{
            std::move(original_str),
            compacted_str.size(),
            std::move(pivot_positions)
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

  size_t min_length = static_cast<size_t>(std::ceil((1 - config_.epsilon) * query_length));
  size_t max_length = static_cast<size_t>(std::floor((1 + config_.epsilon) * query_length));

  auto query_pivot_positions = compacter_.GetPivotPositions(query);

  for (size_t i = 0; i < levels; ++i) {
    char c = compacted_query[i];
    const auto &level_map = index_levels_[i];
    auto it = level_map.find(c);
    if (it != level_map.end()) {
      const auto &records = it->second;

      auto lower = std::lower_bound(records.begin(), records.end(), min_length,
                                    [](const Record &record, size_t length) {
                                      return record.length < length;
                                    });

      auto upper = std::upper_bound(records.begin(), records.end(), max_length,
                                    [](size_t length, const Record &record) {
                                      return length < record.length;
                                    });

      for (auto rec_it = lower; rec_it != upper; ++rec_it) {
        size_t position_difference = std::abs(int(query_pivot_positions[i]) - int(rec_it->pivot_positions[i]));
        if (position_difference <= threshold) {
          frequency[rec_it->original_str] += 1;
        }
      }
    }
  }

  std::vector<std::string_view> results;
  for (const auto &[candidate, freq]: frequency) {
    if (levels - freq <= threshold) {
      results.emplace_back(candidate);
    }
  }

  return results;
}
