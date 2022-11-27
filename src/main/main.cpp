#include <iostream>
using std::cout;

#include <vector>
using std::vector;

#include <cmath>

#include "../layer/layer.hpp"
#include "../material/material.hpp"
#include "../simulation/simulation.hpp"

int main() {
    vector<Layer> layers{Layer(0.9, 1.4, 0.04, 20, 10), Layer(0.9, 1.4, 0.001, 0.1, 2), Layer(0.9, 1.4, 0.025, 25, 4), Layer(0.9, 1.4, 0.005, 60)};
    
    Material material(layers);
    cout << material;

    Simulation simulation(material);

    /*for (int N = 0; N < 1e5; N++) {
        cout << std::endl;
        simulation.next();
        cout << *simulation.upperBoundary << std::endl;
    }*/
    int N = 1e7;
    for (int i = 0; i < N; i++) {
        if (i % 100 == 0) {
            cout << "Done " << std::ceil(1e4*i / N) / 100 << "%" << std::endl;
        }
        simulation.nextPhoton();
    }
    cout << std::endl << "Done, " << simulation.photonsUsed << " photons used" << std::endl;

    for (auto e : simulation.results.reflectance) {
        cout << e << " ";
    }
    cout << std::endl;

    cout << "Reflectance overflow " << simulation.results.reflectanceOverflow << std::endl;
    cout << "Absorption overflow " << simulation.results.absorptionOverflow << std::endl;

    simulation.results.save(simulation.photonsUsed);
}