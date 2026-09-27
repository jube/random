// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Julien Bernard
#include "Random.h"

#include <random>

namespace my {

  static_assert(std::uniform_random_bit_generator<Random>);

  namespace {

    constexpr uint64_t rotl(const uint64_t x, int k)
    {
      return (x << k) | (x >> (64 - k));
    }

    // https://prng.di.unimi.it/splitmix64.c
    class SplitMix64 {
    public:
      constexpr SplitMix64(uint64_t seed)
      : m_state(seed)
      {
      }

      uint64_t next()
      {
        uint64_t z = (m_state += UINT64_C(0x9e3779b97f4a7c15));
        z = (z ^ (z >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
        z = (z ^ (z >> 27)) * UINT64_C(0x94d049bb133111eb);
        return z ^ (z >> 31);
      }

    private:
      uint64_t m_state;
    };

    uint64_t compute_random_seed()
    {
      std::random_device rd;
      uint64_t result = rd();
      result = (result << 32) + rd();
      return result;
    }

  } // anonymous namespace

  Random::Random()
  {
    for (uint64_t& state : m_state) {
      state = compute_random_seed();
    }
  }

  Random::Random(result_type seed)
  {
    SplitMix64 sm(seed);

    for (uint64_t& state : m_state) {
      state = sm.next();
    }
  }

  Random::Random(result_type seed0, result_type seed1, result_type seed2, result_type seed3)
  {
    m_state[0] = seed0;
    m_state[1] = seed1;
    m_state[2] = seed2;
    m_state[3] = seed3;
  }

  auto Random::operator()() -> result_type
  {
    return next();
  }

  void Random::short_jump()
  {
    static constexpr uint64_t Jump[] = {
      UINT64_C(0x180ec6d33cfd0aba),
      UINT64_C(0xd5a61266f0c9392c),
      UINT64_C(0xa9582618e03fc9aa),
      UINT64_C(0x39abdc4529b1661c)
    };

    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;

    for (auto jump : Jump) {
      for (int b = 0; b < 64; b++) {
        if ((jump & UINT64_C(1) << b) != 0) {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
          s2 ^= m_state[2];
          s3 ^= m_state[3];
        }

        next();
      }
    }

    m_state[0] = s0;
    m_state[1] = s1;
    m_state[2] = s2;
    m_state[3] = s3;
  }

  void Random::long_jump()
  {
    static constexpr uint64_t Jump[] = {
      UINT64_C(0x76e15d3efefdcbbf),
      UINT64_C(0xc5004e441c522fb3),
      UINT64_C(0x77710069854ee241),
      UINT64_C(0x39109bb02acbe635)
    };

    uint64_t s0 = 0;
    uint64_t s1 = 0;
    uint64_t s2 = 0;
    uint64_t s3 = 0;

    for (auto jump : Jump) {
      for (int b = 0; b < 64; b++) {
        if ((jump & UINT64_C(1) << b) != 0) {
          s0 ^= m_state[0];
          s1 ^= m_state[1];
          s2 ^= m_state[2];
          s3 ^= m_state[3];
        }

        next();
      }
    }

    m_state[0] = s0;
    m_state[1] = s1;
    m_state[2] = s2;
    m_state[3] = s3;
  }

  // https://prng.di.unimi.it/xoshiro256plusplus.c
  // License: http://creativecommons.org/publicdomain/zero/1.0/
  uint64_t Random::next()
  {
    const uint64_t result = rotl(m_state[0] + m_state[3], 23) + m_state[0];
    const uint64_t t = m_state[1] << 17;

    m_state[2] ^= m_state[0];
    m_state[3] ^= m_state[1];
    m_state[1] ^= m_state[2];
    m_state[0] ^= m_state[3];

    m_state[2] ^= t;

    m_state[3] = rotl(m_state[3], 45);

    return result;
  }

  // using "Debiased Modulo (Once) — Java's Method" from
  // https://www.pcg-random.org/posts/bounded-rands.html
  uint64_t Random::compute_raw_integer(uint64_t max_excluded)
  {
    assert(max_excluded > 0);

    uint64_t x = 0;
    uint64_t r = 0;
    const uint64_t threshold = std::numeric_limits<uint64_t>::max() - max_excluded + 1;

    do { // NOLINT(cppcoreguidelines-avoid-do-while)
      x = next();
      r = x % max_excluded;
    } while (x - r > threshold);

    return r;
  }

  double Random::compute_raw_double()
  {
    const uint64_t value = next();
    return static_cast<double>(value >> 11) * 0x1.0p-53;
  }

  float Random::compute_raw_float()
  {
    const uint64_t value = next();
    return static_cast<float>(value >> 40) * 0x1.0p-24f;
  }

}
