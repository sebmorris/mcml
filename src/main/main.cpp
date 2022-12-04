#include <iostream>
using std::cout;

#include <vector>
using std::vector;

#include <cmath>
#include <cstring>

#include "../layer/layer.hpp"
#include "../material/material.hpp"
#include "../simulation/simulation.hpp"

void verificationModels() {
    cout << "Running verification models" << std::endl;

    vector<Layer> layers{Layer(0.9, 1.4, 0.04, 20, 10), Layer(0.9, 1.4, 0.001, 0.1, 2), Layer(0.9, 1.4, 0.025, 25, 4), Layer(0.9, 1.4, 0.005, 60)};
    
    Material material(layers);
    cout << material;

    vector<double> trackedDistances{10, 20, 30};
    double trackingInterval = 0.5;

    Simulation simulation(material, trackedDistances, trackingInterval);

    int N = 1e6;
    for (int i = 0; i < N; i++) {
        if (i % 100 == 0) {
            cout << "Done " << std::ceil(1e4*i / N) / 100 << "%" << std::endl;
        }
        simulation.nextPhoton();
    }
    cout << std::endl << "Done, " << simulation.launchedPhotons() << " photons used" << std::endl;
}

int main(int argc, char** argv) {
    if (argc == 2 && strcmp(argv[1], "verification") == 0) {
        verificationModels();
        return 0;
    }

    cout << "No program matches" << std::endl;
}