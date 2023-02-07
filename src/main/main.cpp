#include <iostream>
using std::cout;
using std::to_string;

#include <vector>
using std::vector;

#include <cmath>
using std::round;
#include <cstring>
#include <stdexcept>

#include <random>
#include <thread>

#include "../layer/layer.hpp"
#include "../material/material.hpp"
#include "../simulation/simulation.hpp"
#include "../result/result.hpp"
#include "../sample/sample.hpp"
#include "../io/record/record.hpp"
#include "../random/random.hpp"
#include "../constants/constants.hpp"

// maybe move this into Simulation.hpp
void runSimulation(Simulation& sim) {
    for (uint64_t i = 0; i < PHOTONS_PER_SIMULATION; i++) {
        if (i % PHOTONS_PER_REPORT == 0) {
            cout << "Done " << std::ceil((i / (double)PHOTONS_PER_SIMULATION) * 1e4) / 100 << "%" << std::endl;
        }
        sim.nextPhoton();
    }
}

void verificationModels() {
    cout << "Running verification models" << std::endl;

    vector<Layer> layers;
    /*
        one layer unmatched
    */
    //layers.emplace_back(BaseLayerOptions(1.5,   0.1),  10    , 0.9);

    /*
        one layer matched
    */
    //layers.emplace_back(BaseLayerOptions(1,   0.1),  10    , 0.9);

    /*
        three layers
    */
    //layers.emplace_back(BaseLayerOptions(1.5, 0.01, 10), 20, 0.9);
    //layers.emplace_back(BaseLayerOptions(1, 0.001, 5), 0.1, 0.9);
    //layers.emplace_back(BaseLayerOptions(0.5, 0.005), 50, 0.8);

    std::vector<Random> randoms = manyRandoms(manySeeds(1));

    //Material material(layers, 1);
    Material material = sampleMaterial(randoms[0]);
    cout << material;

    for (auto b : material.boundaries_) {
        cout << b << std::endl;
    }

    vector<double> trackedDistances{};
    double trackingInterval = 0.5;

    Simulation simulation(material, trackedDistances, trackingInterval, randoms[0]);

    runSimulation(simulation);
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

    Recording recorder("/home/sebastian/Projects/mcml-simulation/build/test.db");

    recorder.saveSimulation(simulation);
}

void randomSampling(Random random, Recording recorder) {
    for (int i = 0; i < SIMULATIONS_PER_THREAD; i++) {
        Material mat = sampleMaterial(random);
        vector<double> trackedDistances;
        const double trackingInterval{0.5};
        Simulation sim(mat, trackedDistances, trackingInterval, random);

        runSimulation(sim);
        cout << "Done the " << i << "th simulation on one thread" << std::endl;

        recorder.saveSimulation(sim);

        cout << "Saved the simulation" << std::endl;
    }
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

    if (strcmp(argv[1], "random-simulations") == 0) {
        if (argc < 3) {
            cout << "Must provide a database path" << std::endl;
            return -1;
        }

        unsigned int noThreads = std::thread::hardware_concurrency();
        //unsigned int noThreads = 1;

        vector<Random> randoms = manyRandoms(manySeeds(noThreads));

        Recording recorder(argv[2]);
        vector<std::thread> threads;
        threads.reserve(randoms.size());

        cout << "Spawning " << noThreads << " threads" << std::endl;
        for (auto random : randoms) {
            threads.emplace_back(randomSampling, random, recorder);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        return 0;
    }

    if (strcmp(argv[1], "hardware-concurrency") == 0) {
        unsigned int noThreads = std::thread::hardware_concurrency();

        cout << "Hardware concurrency supports " << noThreads << " threads" << std::endl;

        return 0;
    }

    cout << "No program matches" << std::endl;
    return -1;
}