// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Julien Bernard
#ifndef MY_RANDOM_H
#define MY_RANDOM_H

#include <cassert>
#include <cstdint>

#include <concepts>
#include <limits>

namespace my {

  class Random {
  public:
    using result_type = uint64_t;

    Random();
    Random(result_type seed);
    Random(result_type seed0, result_type seed1, result_type seed2, result_type seed3);

    static constexpr result_type min()
    {
      return std::numeric_limits<result_type>::min();
    }

    static constexpr result_type max()
    {
      return std::numeric_limits<result_type>::max();
    }

    result_type operator()();

    template<std::integral T>
    T compute_uniform_integer(T max_excluded)
    {
      assert(max > 0);
      return static_cast<T>(compute_raw_integer(max_excluded));
    }

    template<std::integral T>
    T compute_uniform_integer(T min_included, T max_excluded)
    {
      assert(min < max);
      using U = std::make_unsigned_t<T>;
      const U range = U(max_excluded) - U(min_included);
      return min_included + static_cast<T>(compute_raw_integer(range));
    }

    template<std::floating_point T>
    T compute_uniform_float()
    {
      if constexpr (std::is_same_v<T, double>) {
        return compute_raw_double();
      } else if constexpr (std::is_same_v<T, float>) {
        return compute_raw_float();
      } else {
        return static_cast<T>(compute_raw_float());
      }
    }

    template<std::floating_point T>
    T compute_uniform_float(T max_excluded)
    {
      return compute_uniform_float<T>() * max_excluded;
    }

    template<std::floating_point T>
    T compute_uniform_float(T min_included, T max_excluded)
    {
      return min_included + compute_uniform_float<T>() * (max_excluded - min_included);
    }


    void short_jump();
    void long_jump();

  private:
    uint64_t next();

    uint64_t compute_raw_integer(uint64_t max_excluded);
    double compute_raw_double();
    float compute_raw_float();

    uint64_t m_state[4];
  };

}

#endif // MY_RANDOM_H
