#ifndef PARAMETER_SIMULATION_H
#define PARAMETER_SIMULATION_H

#include <vector>
#include <iostream>

#include "../material/material.hpp"
#include "../simulation/simulation.hpp"
#include "../random/random.hpp"

struct ParameterSimulationLayerOptions {
    double n;
    double height;
    double g;

    bool constantLayer;
    // constant layers define these
    double mu_a;
    double mu_s;

    // wavelength dependent scattering
    double a;
    double b;

    bool concentrationLayer;
    // wavelength dependent absorption
    double cBlood;
    double fOxy;
    double fWater;
    double fLipid;
};

struct ParameterSimulationMaterialOptions {
    std::vector<Material> material;
    std::vector<ParameterSimulationLayerOptions> layers;
};

struct ParameterSimulation {
    std::vector<Simulation> simulations_;
    std::vector<Material> materials_;
    std::vector<ParameterSimulationLayerOptions> layers_;

    ParameterSimulation(std::vector<Material>, std::vector<ParameterSimulationLayerOptions>, std::vector<Random>);
    ParameterSimulation() = delete;
};

void runSimulation(ParameterSimulation&);

#endif