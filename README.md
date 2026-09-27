# C++ `Random` class

A small `Random` class in C++20 that I use in many projects. It is based on [xoshiro256++](https://prng.di.unimi.it/) by Sebastiano Vigna. It includes a uniform integer generator and a uniform float generator. It can be used as a [`std::uniform_random_bit_generator`](https://en.cppreference.com/cpp/numeric/random/uniform_random_bit_generator) and replace engines from the standard library.
