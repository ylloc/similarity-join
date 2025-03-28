#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>

#include <compacter.h>
#include <string_algorithms.h>

TEST_CASE("edit-distance-simple") {
  CHECK_EQ(EditDistance("123", "23"), 1);
  CHECK_EQ(EditDistance("123", "225"), 2);
  CHECK_EQ(EditDistance("123", ""), 3);
}

TEST_CASE("string-compacter-tests") {
  StringCompacter compacter(StringCompacterConfig{0.1, 3});

  SUBCASE("basic-test") {
    std::string s = "123456789abc";
    auto result = compacter.Compact(s);
    CHECK_EQ(result, "6325a9c");
  }

  SUBCASE("short-input-with-padding") {
    std::string s = "a";
    auto result = compacter.Compact(s);
    CHECK_EQ(result, "a______");
  }

  SUBCASE("empty-input") {
    std::string empty{};
    auto result = compacter.Compact(empty);
    CHECK_EQ(result, "_______");
  }

  SUBCASE("longer-input") {
    std::string s = "abcdefghijklmnopqrstuvwxyz";
    auto result = compacter.Compact(s);
    CHECK_EQ(result, "lfcispw");
  }

  SUBCASE("string-are-similar-then-representation-is-also-similar") {
    std::string first = "abcdefghijklmnopqrstuvwxyz";
    std::string second = "abcd_fg_ijklmnop_rstuvwxy_";
    auto first_compacted = compacter.Compact(first);
    auto second_compacted = compacter.Compact(second);
    CHECK_EQ(first_compacted, second_compacted);
  }
}

TEST_CASE("harder-test") {
  StringCompacter compacter(StringCompacterConfig{0.1, 4});
  std::string fst = "string-are-similar-then-representation-is-also-similar";
  std::string scd = "string-a__-similar-then--representation-_____is-also-___";
  CHECK(EditDistance(compacter.Compact(fst), compacter.Compact(scd)) <= 4);
}