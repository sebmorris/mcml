#include "random.hpp"

using std::cout;

RandomInternals::RandomInternals(std::uint_fast32_t seed) : gen(seed) {  }

Random::Random(std::uint_fast32_t seed) : internals_(std::make_shared<RandomInternals>(seed)) {  }

double Random::operator()() {
    double rand;

    do rand = internals_->dist(internals_->gen);
    while (rand <= 0); // ensures rand != 0

    return rand;
}

double Random::random_between(double lower, double upper) {
    return lower + (upper - lower) * (*this)();
}

std::vector<std::uint_fast32_t> manySeeds(std::size_t n) {
    std::vector<std::uint_fast32_t> seeds(n);
    std::seed_seq seq{std::random_device()()};

    seq.generate(seeds.begin(), seeds.end());

    return seeds;
}

std::vector<Random> manyRandoms(std::vector<std::uint_fast32_t> seeds) {
    std::vector<Random> randoms;
    randoms.reserve(seeds.size());

    for (auto seed : seeds) {
        randoms.emplace_back(seed);
    }

    return randoms;
}
