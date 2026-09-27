// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Julien Bernard
#include <iostream>

#include "Random.h"

int main()
{
  my::Random random;

  for (int i = 0; i < 10; ++i) {
    std::cout << 1 + random.compute_uniform_integer(20) << '\n'; // 1d20
  }

}
