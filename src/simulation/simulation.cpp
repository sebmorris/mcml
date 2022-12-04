#include <iostream>
using std::cout;

#include <cmath>
#include <limits>

#include "simulation.hpp"

constexpr double pi = 3.14159265358979323846;

// Probably want another one with double width, double height, size_type bins

TrackedDistance::TrackedDistance(const BulkTracker& tracker, double distance, double tolerance) :
absorption_(tracker), distance_(distance), tolerance_(tolerance) {  };

bool operator==(const TrackedDistance& tracker, double distance) {
    return std::abs(tracker.distance_ - distance) < tracker.tolerance_;
}

bool operator==(double distance, const TrackedDistance& tracker) {
    return tracker == distance;
}

Simulation::Simulation(Material material, vector<double> trackedDistances, double trackingInterval) :
material_(material), trackingInterval_(trackingInterval),
totalAbsorption_(BINS, BINS, SIM_EXTENT, SIM_EXTENT), reflectance_(BINS, SIM_EXTENT) { // need to change these ones
    launch();
    photonsLaunched_ = 1;

    for (auto distance : trackedDistances) {
        TrackedDistance tracker(BulkTracker(BINS, BINS, SIM_EXTENT, SIM_EXTENT), distance, trackingInterval_);
        trackedDistances_.push_back(tracker);
    }
};

inline double Simulation::random() const {
    return distribution_(generator_);
}

void Simulation::nextPhoton() {
    if (currentPhoton_.weight() == DEAD) {
        photonsLaunched_++;
        launch();
    }
    while (currentPhoton_.weight() != DEAD) {
        next();
    }
}

void Simulation::launch() {
    // essentially a reset function
    currentPhoton_ = Photon();
    stepLeft_ = 0;
    upperBoundary_ = begin(material_.boundaries);
    currentLayer_ = begin(material_.layers);
}

void Simulation::next() {
    hop();

    if (hitBoundary()) {
        safeProcessBoundaries();
    } else {
        drop();
        spin();
        rouletteTerminate();
    }
}

void Simulation::hop() {
    double step;
    if (stepLeft_ == 0) {
        step = -std::log(random()) / (currentLayer_->mu_a + currentLayer_->mu_s);
        currentPhoton_.step(step);
    } else {
        // this has not been tested
        //cout << "Using some leftover step " << stepLeft << std::endl;
        step = stepLeft_ / (currentLayer_->mu_a + currentLayer_->mu_s);
        stepLeft_ = 0;
        currentPhoton_.step(step);
    }
}

bool Simulation::hitBoundary() const {
    if (currentPhoton_.direction().z() > 0 && currentPhoton_.position().z() > upperBoundary_->z) return true;
    if (currentPhoton_.direction().z() < 0 && !currentLayer_->infinite && currentPhoton_.position().z() < upperBoundary_[1].z) return true;

    return false;
}

// assumes we have already hit a boundary
void Simulation::safeProcessBoundaries() {
    if (currentPhoton_.position().z() > upperBoundary_->z) {
        bool tryingToEscape = currentPhoton_.position().z() > 0;
        double stepped = currentPhoton_.unstep();
        // stepLeft not used by verification model as R=0 with matched n
        stepLeft_ = stepped -
            (currentPhoton_.position().z() - upperBoundary_->z) / currentPhoton_.direction().z();
        currentPhoton_.stepToHeight(upperBoundary_->z);
        if (tryingToEscape) {
            escape(*upperBoundary_);
        } else {
            // hasn't yet been verified
            reflect(*upperBoundary_);
        }
    } else if (!currentLayer_->infinite) {
        // must be a collision with a lower boundary
        // hasn't yet been verified
        auto lowerBoundary = upperBoundary_[1];
        if (currentPhoton_.position().z() < lowerBoundary.z) {
            double stepped = currentPhoton_.unstep();
            stepLeft_ = stepped - (currentPhoton_.position().z() - lowerBoundary.z) / currentPhoton_.direction().z();
            currentPhoton_.stepToHeight(lowerBoundary.z);
            reflect(lowerBoundary);
        }
    } else {
        cout << "Should never reach" << std::endl;
    }
}

void Simulation::flipDirection() {
    const CartVec& dir = currentPhoton_.direction();
    currentPhoton_.setDirection(dir.x(), dir.y(), -dir.z());
}

void Simulation::reflect(Boundary& boundary) {
    double R = boundary.reflect(currentPhoton_.direction());
    double rand = random();

    if (rand > R) {
        // transmit
        int change = currentPhoton_.direction().z() > 0 ? -1 : 1;

        currentLayer_ += change;
        upperBoundary_ += change;

        currentPhoton_.setDirection(boundary.refractionDirection(currentPhoton_.direction()));
    } else {
        // reflect
        flipDirection();
    }
}

void Simulation::escape(Boundary& boundary) {
    double R = boundary.reflect(currentPhoton_.direction());

    double weightT = (1 - R) * currentPhoton_.weight();
    reflectance_.rawDrop(weightT, currentPhoton_.position().r());

    currentPhoton_.incrementWeight(weightT);
    flipDirection();

    // TODO: need to add this
    // track();
}

void Simulation::drop() {
    double amount = (currentLayer_->mu_a / (currentLayer_->mu_a + currentLayer_->mu_s))*currentPhoton_.weight;
    currentPhoton_.incrementWeight(amount);

    totalAbsorption_.rawDrop(amount, currentPhoton_.position());
}

void Simulation::spin() {
    // eq. 5.45, 5.46, 5.47a of:
    // Monte Carlo Modeling of Light Transport in Tissue (Steady State and Time of Flight) (Jacques 2011)
    double rand = random();

    // avoiding messy formulas
    double& g = (*currentLayer_).g;
    const double& ux = currentPhoton_.direction().x();
    const double& uy = currentPhoton_.direction().y();
    const double& uz = currentPhoton_.direction().z();

    // I chose g*g instead of square functions
    // apparently it's more efficient
    // https://stackoverflow.com/a/2940800
    double num_factor = (1 - g*g) / (1 - g + 2*g*rand);
    double num = 1 + g*g - num_factor*num_factor;

    // nb: sqrt faster than sin
    double cosTheta = num / (2*g);
    double sinTheta = std::sqrt(1 - cosTheta*cosTheta);
    double Phi = 2*pi*random();
    double cosPhi = std::cos(Phi);
    double sinPhi;
    if(Phi<pi) sinPhi = std::sqrt(1 - cosPhi*cosPhi);
    else sinPhi = - std::sqrt(1 - cosPhi*cosPhi);

    double temp = std::sqrt(1 - uz*uz);

    if (1 - std::abs(uz) < 1e-5 /*small number*/) {
        double uxx = sinTheta*cosPhi;
        double uyy = sinTheta*sinPhi;
        double uzz = (uz > 0 ? 1 : -1)*cosTheta;

        currentPhoton_.setDirection(uxx, uyy, uzz);
    } else {
        double uxx = sinTheta*(ux*uz*cosPhi - uy*sinPhi)/temp + ux*cosTheta;
        double uyy = sinTheta*(uy*uz*cosPhi + ux*sinPhi)/temp + uy*cosTheta;
        double uzz = -sinTheta*cosPhi*temp + uz*cosTheta;

        currentPhoton_.setDirection(uxx, uyy, uzz);
    }
}

void Simulation::rouletteTerminate() {
    if (currentPhoton_.weight() < TERMINATION_THRESHOLD) {
        if (random() < TERMINATION_CHANCE) {
            currentPhoton_.setWeight(currentPhoton_.weight() / TERMINATION_CHANCE);
        } else {
            currentPhoton_.setWeight(DEAD);
        }
    }
}

unsigned int Simulation::launchedPhotons() {
    return photonsLaunched_;
}