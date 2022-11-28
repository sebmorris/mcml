#ifndef SIMULATION_H
#define SIMULATION_H

#include <random>
#include <vector>

#include "../cartvec/cartvec.hpp"
#include "../layer/layer.hpp"
#include "../photon/photon.hpp"
#include "../material/boundary.hpp"
#include "../material/material.hpp"
#include "../result/result.hpp"

struct Simulation {
    const double TERMINATION_THRESHOLD = 1e-4;
    const double TERMINATION_CHANCE = 0.1;

    int photonsLaunched;
    double stepLeft;

    Result results;

    Material material;
    Photon currentPhoton;

    vector<Layer>::iterator currentLayer;
    vector<Boundary>::iterator upperBoundary;

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution{0.0, 1.0};

    Simulation(Material material);

    double random();

    void launch();

    void next();
    void hop();
    bool hitBoundary();
    void processBoundaries();
    const void reflect(Boundary& boundary);
    const void escape(Boundary& boundary);
    void drop();
    void spin();
    void terminate();

    void nextPhoton();
};

#endif