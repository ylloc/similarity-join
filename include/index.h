#pragma once

#include <string>

class Index {
public:
  virtual void Insert(std::string str) = 0;

  virtual std::vector<std::string_view> Search(std::string_view query, size_t threshold) const = 0;

  virtual ~Index() = 0;
};