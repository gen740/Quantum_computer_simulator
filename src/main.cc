#include <iostream>
#include <linalg/Matrix.h>

using Linalg::Matrix;

auto a = Matrix<double _Complex>({{1, 2, 3}, {4, 5, 6}, {7, 8, 10}});

int main() { std::cout << a << std::endl; }
