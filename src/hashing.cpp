#include <hashing.h>

LinearFunction::LinearFunction(int64_t shift, int64_t scale, int64_t mod)
        : shift_(shift), scale_(scale), mod_(mod) {}

int64_t LinearFunction::operator()(char value) const {
  int64_t val = (shift_ + static_cast<int64_t>(value) * scale_) % mod_;
  return val;
}

LinearFunction RandomLinearFunction(std::mt19937 &mt, int64_t mod) {
  int64_t scale = std::uniform_int_distribution<int64_t>(1, mod - 1)(mt);
  int64_t shift = std::uniform_int_distribution<int64_t>(0, mod - 1)(mt);
  return {shift, scale, mod};
}

LinearFunction DefaultLinearFunction() {
  return {0, 1, kP};
}