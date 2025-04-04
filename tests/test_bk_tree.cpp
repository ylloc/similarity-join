#include <doctest.h>

#include <bk_tree.h>

#include <iostream>

TEST_CASE("bk-tree-tests") {
  BKTreeIndex bk_index;

  bk_index.Insert("book");
  bk_index.Insert("cook");
  bk_index.Insert("books");
  bk_index.Insert("cake");
  bk_index.Insert("bake");
  bk_index.Insert("cookie");
  bk_index.Insert("back");

  auto results = bk_index.Search("book", 1);

  REQUIRE(std::find(results.begin(), results.end(), "book") != results.end());
  REQUIRE(std::find(results.begin(), results.end(), "cook") != results.end());
  REQUIRE(std::find(results.begin(), results.end(), "books") != results.end());
  REQUIRE(std::find(results.begin(), results.end(), "bake") == results.end());
}