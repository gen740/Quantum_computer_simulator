#pragma once

#include <_types/_uint32_t.h>
#include <iomanip>

namespace Utils {

template <size_t BIT_NUM> struct Bit_Repr_Ostream_ {
  uint32_t bits;
  explicit Bit_Repr_Ostream_(uint32_t n) : bits{n} {}

  // template <size_t BIT_NUM2>
  friend std::ostream &operator<<(std::ostream &os,
                                  const Bit_Repr_Ostream_<BIT_NUM> &bit_repr) {
    // for (int i = 0; i < BIT_NUM; i++) {
    for (int i = BIT_NUM - 1; i >= 0; i--) {
      if (static_cast<bool>(bit_repr.bits & (0b1 << i))) {
        os << "1";
      } else {
        os << "0";
      }
    }
    return os;
  }
};

template <size_t BIT_NUM> auto bit_repr(uint32_t n) {
  return Bit_Repr_Ostream_<BIT_NUM>{n};
}

} // namespace Utils
