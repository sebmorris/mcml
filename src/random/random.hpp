#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <iostream>
#include <memory>
#include <cstdint>
#include <vector>
#include <cmath>
#include <beta.hpp>

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
        double jeffreys_random_between(double, double);
        double random_beta(double a, double b);

        static double min() { return 0.0; }
        static double max() { return 1.0; }
};

std::vector<std::uint_fast32_t> manySeeds(std::size_t);
std::vector<Random> manyRandoms(std::vector<std::uint_fast32_t>);

#endif