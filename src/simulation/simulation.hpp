#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stack>
#include <cstdint>

#include "../cartvec/cartvec.hpp"
#include "../layer/layer.hpp"
#include "../photon/photon.hpp"
#include "../material/boundary.hpp"
#include "../material/material.hpp"
#include "../result/result.hpp"
#include "../constants/constants.hpp"
#include "../random/random.hpp"

const double TERMINATION_THRESHOLD = 1e-4;
const double TERMINATION_CHANCE = 0.1;

const double SIM_EXTENT = 50;
const uint32_t BINS = 100;

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
    public:
        using layer_it = std::vector<Layer>::iterator;
        using boundary_it = std::vector<Boundary>::iterator;
    private:
        // simulation history
        uint64_t photonsLaunched_;
        BulkTracker totalAbsorption_;
        RadialTracker reflectance_;

        const bool tracking_;
        double trackingInterval_;
        std::vector<TrackedDistance> trackedDistances_;

        // random generation
        Random random_;

        // physical situation
        Material material_;
        Photon currentPhoton_;
        double stepLeft_;

        // for tracking
        std::stack<AbsorptionEvent> absorptionHistory_; // tracks current Photon's abs history

        layer_it currentLayer_;
        boundary_it upperBoundary_;

        void launch();
        void hop();
        void flipDirection();
        double stepLeft(double, double) const;

        // managing photon events
        bool hitBoundary() const;
        void safeProcessBoundaries();
        void interact();
        void recordDrop();
        void safeTrack(double amount);

        void reflect(Boundary& boundary);
        void escape(Boundary& boundary);
    public:
        Simulation() = delete;
        Simulation(Material material, std::vector<double> trackedDistances, double trackingInterval, Random);

        // maybe have these return the instance
        void next();
        void nextPhoton();
        void rouletteTerminate();

        // accessors
        const RadialTracker::row& rawReflectance() const;
        RadialTracker::row reflectance() const;
        BulkTracker::grid absorption() const;
        std::vector<BulkTracker::grid> trackedAbsorption() const;
        uint64_t launchedPhotons() const;
        const Material& getMaterial() const;
};

#endif