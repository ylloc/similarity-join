#include <doctest.h>

#include <trie_index.h>
#include <string_algorithms.h>
#include <inverted_index.h>

#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>

TEST_CASE("simple-tests") {
  TrieIndex trie(StringCompacter{StringCompacterConfig{.compact_size=4}});

  trie.InsertCompactedString("abcd", "original-1");
  trie.InsertCompactedString("abce", "original-2");
  trie.InsertCompactedString("abcf", "original-3");
  trie.InsertCompactedString("xyzz", "original-4");

  SUBCASE("exact-match") {
    auto results = trie.SearchCompactedQuery("abcd", 0);
    REQUIRE_EQ(results.size(), 1);
    CHECK_EQ(results[0], "original-1");
  }

  SUBCASE("within-threshold") {
    auto results = trie.SearchCompactedQuery("abcc", 1);
    REQUIRE_EQ(results.size(), 3);
    CHECK(std::find(results.begin(), results.end(), "original-1") != results.end());
    CHECK(std::find(results.begin(), results.end(), "original-2") != results.end());
    CHECK(std::find(results.begin(), results.end(), "original-3") != results.end());
  }

  SUBCASE("beyond-threshold") {
    auto results = trie.SearchCompactedQuery("abcg", 0);
    CHECK(results.empty());
  }

  SUBCASE("bad-prefix") {
    auto results = trie.SearchCompactedQuery("xxxx", 1);
    CHECK(results.empty());
  }

  SUBCASE("short-sketch-padded") {
    auto results = trie.SearchCompactedQuery("ab__", 2);
    REQUIRE_EQ(results.size(), 3);
  }

  SUBCASE("empty-trie") {
    TrieIndex empty_trie(StringCompacterConfig{});
    auto results = empty_trie.SearchCompactedQuery("abcd", 1);
    CHECK(results.empty());
  }
}

TEST_CASE("trie-hard-test") {
  TrieIndex trie(StringCompacterConfig{.compact_size=3});
  trie.InsertCompactedString("aba", "1");
  trie.InsertCompactedString("abb", "2");
  trie.InsertCompactedString("aca", "3");
  trie.InsertCompactedString("baa", "4");
  trie.InsertCompactedString("bab", "5");
  trie.InsertCompactedString("bba", "6");
  trie.InsertCompactedString("bdb", "7");

  SUBCASE("test-1") {
    REQUIRE_EQ(trie.SearchCompactedQuery("b__", 2).size(), 4);
    REQUIRE_EQ(trie.SearchCompactedQuery("a_a", 1).size(), 2);
    REQUIRE_EQ(trie.SearchCompactedQuery("_ba", 1).size(), 2);
  }

  SUBCASE("test-2") {
    REQUIRE_EQ(trie.SearchCompactedQuery("___", 3).size(), 7);
  }
}

TEST_CASE("pivot-positions") {
  TrieIndex trie(StringCompacterConfig{.compact_size = 3});

  constexpr size_t kBaseStrings = 100;
  constexpr size_t kVariantsPerString = 10;
  constexpr size_t kStringLength = 20;

  std::vector<std::string> base_strings;
  base_strings.reserve(kBaseStrings);

  std::mt19937 gen(42);
  std::uniform_int_distribution<> char_dis('a', 'z');
  std::uniform_int_distribution<> pos_dis(0, kStringLength - 1);

  auto GenerateRandomString = [&]() {
    std::string s(kStringLength, 'a');
    for (auto &c: s) {
      c = static_cast<char>(char_dis(gen));
    }
    return s;
  };

  auto ModifyString = [&](std::string s, size_t num_edits) {
    for (size_t i = 0; i < num_edits; ++i) {
      size_t pos = pos_dis(gen);
      s[pos] = static_cast<char>(char_dis(gen));
    }
    return s;
  };

  for (size_t i = 0; i < kBaseStrings; ++i) {
    auto base = GenerateRandomString();
    base_strings.push_back(base);

    for (size_t j = 0; j < kVariantsPerString; ++j) {
      auto variant = ModifyString(base, 1 + (j % 2));
      trie.Insert(variant);
    }
  }

  for (const auto &query: base_strings) {
    auto found_strings = trie.Search(query, 1);
    std::map < size_t, size_t > histogram;

    for (const auto &found: found_strings) {
      size_t dist = EditDistance(query, found);
      histogram[dist]++;
    }


    // histogram
    std::cout << "query: " << query << '\n';
    for (const auto &[dist, count]: histogram) {
      std::cout << "  distance: " << dist << " count: " << count << '\n';
    }
  }
}

TEST_CASE("from-file") {
  TrieIndex index(StringCompacterConfig{.compact_size=2});

  std::ifstream file("../tests/fixtures/strings.txt");
  REQUIRE(file.is_open());

  std::vector<std::string> queries;

  std::string line;
  while (std::getline(file, line)) {
    index.Insert(line);
    line[rand() % line.size()] = 'a' + (rand() % 26);

    queries.push_back(line);
  }

  file.close();

  size_t matches = 0;
  for (const auto& query: queries) {
    matches += static_cast<size_t>(index.Search(query, 1).size() >= 1);
  }

  REQUIRE(matches >= 990);
}

TEST_CASE("from-file") {
  InvertedIndex index(StringCompacterConfig{.compact_size=2});

  std::ifstream file("../tests/fixtures/strings.txt");
  REQUIRE(file.is_open());

  std::vector<std::string> queries;

  std::string line;
  while (std::getline(file, line)) {
    line[rand() % line.size()] = 'a' + (rand() % 26);
    queries.push_back(line);
  }

  index.Build(queries);

  file.close();

  size_t matches = 0;
  for (const auto& query: queries) {
    matches += static_cast<size_t>(index.Search(query, 1).size() >= 1);
  }

  REQUIRE(matches >= 990);
}
