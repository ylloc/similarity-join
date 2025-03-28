#pragma once

#include <cstdint>
#include <random>

static constexpr int64_t kP = 87'178'291'199;

class LinearFunction {
public:
  explicit LinearFunction() = default;

  LinearFunction(int64_t shift, int64_t scale, int64_t mod);

  int64_t operator()(char value) const;

private:
  int64_t shift_{0};
  int64_t scale_{0};
  int64_t mod_{1};
};

LinearFunction RandomLinearFunction(std::mt19937 &mt, int64_t mod = kP);
LinearFunction DefaultLinearFunction();