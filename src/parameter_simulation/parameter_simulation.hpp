#ifndef PARAMETER_SIMULATION_H
#define PARAMETER_SIMULATION_H

#include <vector>
#include <iostream>

#include "../material/material.hpp"
#include "../simulation/simulation.hpp"
#include "../random/random.hpp"

struct ParameterSimulation {
    std::vector<Simulation> simulations_;
    std::vector<Material> materials_;

    ParameterSimulation(std::vector<Material>, Random);
    ParameterSimulation() = delete;
    void run();
};

#endif