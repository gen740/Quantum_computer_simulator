#include <QuantumCircuit/QuantumCircuit.h>
#include <cassert>
#include <complex>
#include <exception>
#include <format>
#include <iostream>
#include <limits>
#include <linalg/Matrix.h>

#include "utils.h"

using QuantumCircuit::measure;
using QuantumCircuit::QCircuit;
using Utils::bit_repr;

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
  // 10 量子ビットで量子回路を作る
  QCircuit q{10};

  // 一ビット加算機とその反対処理を記述する
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

  // 以下で量子回路の ascii 図を表示することができる。
  std::cout << q << std::endl;

  // 上に指定した回路をここで初めて行列計算をする。
  // N 量子ビットに対し O(N^3) の計算オーダー
  q.compile();

  // 一度、行列計算をしているので、評価は compile よりも圧倒的に早い
  // N 量子ビットに対し O(N^2) の計算オーダー
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      for (auto &&k : measure(q.eval(triadder_encode(i, j)))) {
        if (std::abs(k.second - 1.0) < std::numeric_limits<double>::min()) {
          std::cout << bit_repr<3>(i) << "(" << i << ")"
                    << " + " << bit_repr<3>(j) << "(" << j << ")"
                    << " = " << bit_repr<4>(triadder_decode(k.first)) << "("
                    << triadder_decode(k.first) << ")" << std::endl;
          if (i + j != triadder_decode(k.first)) {
            assert(false && "Invalid Value!!");
          }
        }
      }
    }
  }
}
