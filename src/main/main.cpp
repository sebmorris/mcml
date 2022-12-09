#include <iostream>
using std::cout;
using std::to_string;

#include <vector>
using std::vector;

#include <cmath>
using std::round;
#include <cstring>
#include <stdexcept>

#include "../layer/layer.hpp"
#include "../material/material.hpp"
#include "../simulation/simulation.hpp"
#include "../result/result.hpp"

void verificationModels() {
    cout << "Running verification models" << std::endl;

    vector<Layer> layers{Layer(0.9, 1.4, 0.04, 20, 10), Layer(0.9, 1.4, 0.001, 0.1, 2), Layer(0.9, 1.4, 0.025, 25, 4), Layer(0.9, 1.4, 0.005, 60)};
    
    Material material(layers, 1.4);
    cout << material;

    vector<double> trackedDistances{10, 20, 30, 40};
    double trackingInterval = 0.5;

    Simulation simulation(material, trackedDistances, trackingInterval);

    int N = 1e6;
    for (int i = 0; i < N; i++) {
        if (i % 1000 == 0) {
            cout << "Done " << std::ceil(1e4*i / N) / 100 << "%" << std::endl;
        }
        simulation.nextPhoton();
    }
    cout << std::endl << "Done, " << simulation.launchedPhotons() << " photons used" << std::endl;

    std::ofstream outRef("reflectance_verification.csv");
    csvRowString(outRef, simulation.reflectance());

    std::ofstream outAbs("absorption_verification.csv");
    csvGridString(outAbs, simulation.absorption());

    vector<BulkTracker::grid> trackedAbs = simulation.trackedAbsorption();

    for (BulkTracker::index_type i = 0; i != trackedDistances.size(); i++) {
        std::string roundedDistance = to_string((int)round(trackedDistances[i]));
        std::ofstream outTracked("absorption_verification_" + roundedDistance + "mm.csv");
        csvGridString(outTracked, trackedAbs[i]);
    }
}

void checkApproximation() {
    cout << "Checking approximation" << std::endl;

    std::logic_error("check-approximation not yet implemented");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Must provide at least one argument" << std::endl;
        return -1;
    }

    if (strcmp(argv[1], "verification") == 0) {
        verificationModels();
        return 0;
    }

    if (strcmp(argv[1], "check-approximation") == 0) {
        checkApproximation();
        return 0;
    }

    cout << "No program matches" << std::endl;
}