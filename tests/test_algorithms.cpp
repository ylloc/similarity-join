#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>
#include <algorithms/string_algorithms.h>

TEST_CASE("edit-distance-simple") {
  CHECK_EQ(EditDistance("123", "23"), 1);
  CHECK_EQ(EditDistance("123", "225"), 2);
  CHECK_EQ(EditDistance("123", ""), 3);
}