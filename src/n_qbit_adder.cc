#include <QuantumCircuit/QuantumCircuit.h>
#include <complex>
#include <exception>
#include <format>
#include <iostream>
#include <linalg/Matrix.h>

#include "utils.h"

using QuantumCircuit::measure;
using QuantumCircuit::QCircuit;
using Utils::bit_repr;

template <size_t NUM_BIT> uint32_t bit_adder_encoder(uint32_t x, uint32_t y) {
  uint32_t ret{0};
  for (int i = 0; i < NUM_BIT; i++) {
    if ((x & (0b1 << i)) != 0u) {
      ret += (0b1 << ((2 * i) + 1));
    }
    if ((y & (0b1 << i)) != 0u) {
      ret += (0b1 << ((2 * i) + 2));
    }
  }
  return ret;
}

template <size_t NUM_BIT> uint32_t bit_adder_decoder(uint32_t x) {
  uint32_t ret{0};
  for (int i = 0; i < NUM_BIT; i++) {
    if ((x & (0b1 << (2 * i + 1))) != 0u) {
      ret += (0b1 << i);
    }
  }
  if ((x & (0b1 << (2 * NUM_BIT + 1))) != 0u) {
    ret += (0b1 << NUM_BIT);
  }
  return ret;
}

// bit_num の adder を作る。
constexpr int BIT_NUM = 3;

int main() {

  // 12 量子ビットで量子回路を作る
  QCircuit q{BIT_NUM * 2 + 2};

  // 一ビット加算機とその反対処理を記述する
  auto MAJ = [&q](int offset) {
    q.cx(offset + 0, offset + 1);
    q.cx(offset + 2, offset + 0);
    q.ccx(offset + 0, offset + 1, offset + 2);
  };
  auto UMA = [&q](int offset) {
    q.ccx(offset + 0, offset + 1, offset + 2);
    q.cx(offset + 2, offset + 0);
    q.cx(offset + 0, offset + 1);
  };

  for (int i = 0; i < BIT_NUM; i++) {
    MAJ(2 * i);
  }
  for (int i = 0; i < BIT_NUM * 2; i++) {
    q.x(i);
  }
  q.cx(BIT_NUM * 2, BIT_NUM * 2 + 1);
  for (int i = BIT_NUM; i >= 0; i--) {
    UMA(2 * i);
  }
  q.x(BIT_NUM * 2 + 1);

  // 上に指定した回路をここで初めて行列計算をする。
  // N 量子ビットに対し O(N^3) の計算オーダー
  std::cout << q << std::endl;
  // q.compile();




  // std::cout << bit_repr<8>(bit_adder_encoder<3>(0b100, 0b011)) << std::endl;
  //
  // std::cout << bit_adder_decoder<5>(0b001010101010) << std::endl;
  // // // 一度、行列計算をしているので、評価は compile よりも圧倒的に早い
  // // // N 量子ビットに対し O(N^2) の計算オーダー
  // for (int i = 0; i < (0b1 << BIT_NUM); i++) {
  //   for (int j = 0; j < (0b1 << BIT_NUM); j++) {
  //     for (auto &&k : measure(q.eval(bit_adder_encoder<BIT_NUM>(i, j)))) {
  //       if (k.second > 0.9) {
  //
  //         std::cout << bit_repr<BIT_NUM>(i) << "(" << i << ")"
  //                   << " + " << bit_repr<BIT_NUM>(j) << "(" << j << ")"
  //                   << " = "
  //                   << bit_repr<BIT_NUM + 1>(
  //                          bit_adder_decoder<BIT_NUM>(k.first))
  //                   << "(" << bit_adder_decoder<BIT_NUM>(k.first) << ")"
  //                   << std::endl;
  //         // if (i + j != bit_adder_decoder<BIT_NUM>(k.first)) {
  //         //   throw std::runtime_error("Invalid Value has occur");
  //         // }
  //       }
  //     }
  //   }
  // }
}
