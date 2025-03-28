#include <doctest.h>

#include <trie_index.h>

#include <algorithm>

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