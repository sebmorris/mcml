#include "random.hpp"

using std::cout;

double sim_random() {
    static std::random_device device;
    static std::default_random_engine generator(device());

    static std::uniform_real_distribution<double> distribution{0.0, 1.0};

    double rand;

    do rand = distribution(generator);
    while (rand <= 0); // ensures rand != 0

    if (rand > 1) {
        cout << "should never reach" << std::endl;
    }

    return rand;
}

double sim_random_between(double lower, double upper) {
    return (upper - lower) * sim_random();
}