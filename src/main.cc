#include <QuantumCircuit/QuantumCircuit.h>
#include <any>
#include <bit>
#include <complex>
#include <exception>
#include <iostream>
#include <linalg/Matrix.h>

using Linalg::Matrix;
using Linalg::ZMatrix;
using QuantumCircuit::measure;
using QuantumCircuit::QCircuit;

template <size_t NUM_BIT> ZMatrix num_to_mat(uint bit) {
  if (bit > (0b1 << NUM_BIT)) {
    throw std::runtime_error("bit exceed NUM_BIT");
  }
  ZMatrix ret(1, NUM_BIT);
  for (int i = 1; i <= NUM_BIT; i++) {
    ret(1, i) =
        static_cast<std::complex<double>>(static_cast<bool>(bit & (0b1 << i)));
  }
}

template <size_t NUM_BIT> void print_bitrepr(uint32_t n) {
  for (int i = NUM_BIT - 1; i >= 0; i--) {
    std::cout << static_cast<int>(static_cast<bool>(n & (0b1 << i)));
  }
}

uint32_t triadder_encode(uint32_t x, uint32_t y) {
  uint32_t ret{0};
  for (int i = 0; i < 3; i++) {
    if ((x & (0b1 << i)) != 0u) {
      ret += (0b1 << ((3 * i) + 1));
    }
    if ((y & (0b1 << i)) != 0u) {
      ret += (0b1 << ((3 * i) + 2));
    }
  }
  return ret;
}

uint32_t triadder_decode(uint32_t x) {
  uint32_t ret{0};
  for (int i = 0; i < 3; i++) {
    if ((x & (0b1 << (3 * i + 2))) != 0u) {
      ret += (0b1 << i);
    }
  }
  if ((x & (0b1 << 9)) != 0u) {
    ret += (0b1 << 3);
  }
  return ret;
}

int main() {
  Linalg::DMatrix::set_precision(1);
  QCircuit<10> q;

  auto C = [&q](int offset) {
    q.ccx(offset + 1, offset + 2, offset + 3);
    q.cx(offset + 1, offset + 2);
    q.ccx(offset + 0, offset + 2, offset + 3);
  };
  auto CT = [&q](int offset) {
    q.ccx(offset + 0, offset + 2, offset + 3);
    q.cx(offset + 1, offset + 2);
    q.ccx(offset + 1, offset + 2, offset + 3);
  };
  C(0);
  C(3);
  C(6);
  q.cx(6, 8);
  CT(3);
  q.cx(3, 5);
  CT(0);
  q.cx(1, 2);
  q.cx(4, 5);

  q.compile();

  for (int i = 0; i < 8; i++) {

    for (int j = 0; j < 8; j++) {
      for (auto &&k : measure(q.eval(triadder_encode(i, j)))) {
        if (k.second > 0.9) {
          print_bitrepr<3>(i);
          std::cout << "(" << i << ")"
                    << " + ";
          print_bitrepr<3>(j);
          std::cout << "(" << j << ")"
                    << " = ";
          print_bitrepr<4>(triadder_decode(k.first));
          std::cout << "(" << triadder_decode(k.first) << ")" << std::endl;
        }
      }
    }
  }
}
