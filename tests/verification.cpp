#include <iostream>
using std::cout;

#include <vector>
using std::vector;

#include <cmath>

#include "../src/layer/layer.hpp"
#include "../src/material/material.hpp"
#include "../src/simulation/simulation.hpp"

int main() {
    // based on the comparison in Biomedical Optics (Wang & Wu)
    vector<Layer> layers{Layer(0.9, 1, 0.1, 100)};
    
    Material material(layers);
    cout << material;

    Simulation simulation(material);

    int N = 1e6;
    for (int i = 0; i < N; i++) {
        if (i % 100 == 0) {
            cout << "Done " << std::ceil(1e4*i / N) / 100 << "%" << std::endl;
        }
        simulation.nextPhoton();
    }
    cout << std::endl << "Done, " << simulation.photonsLaunched << " photons used" << std::endl;

    simulation.results.save(simulation.photonsLaunched);
}