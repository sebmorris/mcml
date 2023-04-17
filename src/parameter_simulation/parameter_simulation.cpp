#include "parameter_simulation.hpp"

using std::vector;

ParameterSimulation::ParameterSimulation(vector<Material> mats, Random random) : materials_(mats) {
    vector<double> tracked{};
    double trackingInterval = 0;
    for (Material m: materials_) {
        simulations_.emplace_back(m, tracked, trackingInterval, random);
    }
}

void runSimulation(ParameterSimulation& sims) {
    for (Simulation s: sims.simulations_) {
        runSimulation(s);
    }
}