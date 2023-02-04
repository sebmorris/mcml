#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <iostream>
#include <memory>
#include <cstdint>
#include <vector>

struct RandomInternals {
    std::default_random_engine gen;
    std::uniform_real_distribution<double> dist{0, 1};

    RandomInternals(std::uint_fast32_t);
};

class Random {
    private:
        mutable std::shared_ptr<RandomInternals> internals_;
    public:
        Random() = delete;
        Random(std::uint_fast32_t);

        double operator()();
        double random_between(double, double);
};

std::vector<std::uint_fast32_t> manySeeds(std::size_t);
std::vector<Random> manyRandoms(std::vector<std::uint_fast32_t>);

#endif