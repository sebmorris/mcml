#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stack>

#include "../cartvec/cartvec.hpp"
#include "../layer/layer.hpp"
#include "../photon/photon.hpp"
#include "../material/boundary.hpp"
#include "../material/material.hpp"
#include "../result/result.hpp"

const double TERMINATION_THRESHOLD = 1e-4;
const double TERMINATION_CHANCE = 0.1;

const double SIM_EXTENT = 50;
const unsigned int BINS = 100;

struct AbsorptionEvent {
    CartVec position_;
    double amount_;

    AbsorptionEvent() = delete;
    AbsorptionEvent(double, const CartVec&);
};

struct TrackedDistance {
    BulkTracker absorption_;
    double distance_;
    double tolerance_;

    TrackedDistance() = delete;
    TrackedDistance(const BulkTracker&, double, double);
};

bool operator==(const TrackedDistance&, double);
bool operator==(double, const TrackedDistance&);

class Simulation {
    private:
        // simulation history
        unsigned int photonsLaunched_;
        BulkTracker totalAbsorption_;
        RadialTracker reflectance_;
        double trackingInterval_;
        std::vector<TrackedDistance> trackedDistances_;

        typedef std::vector<Layer>::iterator layer_it;
        typedef std::vector<Boundary>::iterator boundary_it;
        // physical situation
        Material material_;
        Photon currentPhoton_;
        double stepLeft_;

        // for tracking
        std::stack<AbsorptionEvent> absorptionHistory_; // tracks current Photon's abs history

        layer_it currentLayer_;
        boundary_it upperBoundary_;

        //mutable std::default_random_engine generator_;
        mutable std::mt19937_64 generator_;
        mutable std::uniform_real_distribution<double> distribution_{0.0, 1.0};

        double random() const;
        void launch();
        void hop();
        void flipDirection();

        // managing photon events
        bool hitBoundary() const;
        void safeProcessBoundaries();
        void spin();
        void drop();
        void safeTrack(double amount);

        void reflect(Boundary& boundary);
        void escape(Boundary& boundary);
    public:
        Simulation() = delete;
        Simulation(Material material, std::vector<double> trackedDistances, double trackingInterval);

        // maybe have these return the instance
        void next();
        void nextPhoton();
        void rouletteTerminate();

        // accessors
        const RadialTracker::row& rawReflectance() const;
        RadialTracker::row reflectance() const;
        BulkTracker::grid absorption() const;
        std::vector<BulkTracker::grid> trackedAbsorption() const;
        unsigned int launchedPhotons() const;
};

#endif