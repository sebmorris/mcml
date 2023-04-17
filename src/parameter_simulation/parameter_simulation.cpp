#include "parameter_simulation.hpp"

using std::vector;

ParameterSimulation::ParameterSimulation(std::vector<Material> mats, std::vector<ParameterSimulationLayerOptions> options, Random random) : materials_(mats), layers_(options) {
    vector<double> tracked{};
    double trackingInterval = 0;
    for (Material m: materials_) {
        simulations_.emplace_back(m, tracked, trackingInterval, random);
    }
}

void runSimulation(ParameterSimulation& sims) {
    for (Simulation& s: sims.simulations_) {
        runSimulation(s);
    }
}