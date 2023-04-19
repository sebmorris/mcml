#include "parameter_simulation.hpp"

using std::vector;

ParameterSimulation::ParameterSimulation(vector<Material> mats, vector<ParameterSimulationLayerOptions> options, vector<Random> randoms) : materials_(mats), layers_(options) {
    if (mats.size() != randoms.size()) {
        std::cout << mats.size() << " " << randoms.size() << std::endl;
        throw std::invalid_argument("The size of materials and randoms should be the same");
    }

    vector<double> tracked{};
    double trackingInterval = 0;

    for (std::size_t i = 0; i < mats.size(); i++) {
        simulations_.emplace_back(mats[i], tracked, trackingInterval, randoms[i]);
    }
}

void runSimulation(ParameterSimulation& sims) {
    for (Simulation& s: sims.simulations_) {
        runSimulation(s);
    }
}