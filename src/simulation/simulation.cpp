#include "simulation.hpp"

using std::cout;
using std::ostream;
using std::vector;

// Probably want another one with double width, double height, size_type bins

Simulation::Simulation(Material material, vector<double> trackedDistances, double trackingInterval) :
material_(material), trackingInterval_(trackingInterval), tracking_(!trackedDistances.empty()),
totalAbsorption_(BINS, BINS, SIM_EXTENT, SIM_EXTENT), reflectance_(BINS, SIM_EXTENT) {
    launch();
    photonsLaunched_ = 1;

    for (auto distance : trackedDistances) {
        TrackedDistance tracker(BulkTracker(BINS, BINS, SIM_EXTENT, SIM_EXTENT), distance, trackingInterval_);
        trackedDistances_.push_back(tracker);
    }
};

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
    upperBoundary_ = begin(material_.boundaries_);
    currentLayer_ = begin(material_.layers_);
    currentPhoton_ = Photon(ALIVE - upperBoundary_->rSpecular());
    stepLeft_ = 0;
    absorptionHistory_ = std::stack<AbsorptionEvent>();
}

void Simulation::next() {
    hop();

    if (hitBoundary()) {
        safeProcessBoundaries();
    } else {
        recordDrop();
        interact();
        rouletteTerminate();
    }
}

void Simulation::hop() {
    double step;
    if (stepLeft_ == 0) {
        step = -std::log(sim_random()) / currentLayer_->interactionRate();
    } else {
        step = stepLeft_ / currentLayer_->interactionRate();
        stepLeft_ = 0; // the hitBoundary function will reassign stepLeft_ if multiple layers are traversed
    }

    currentPhoton_.step(step);
}

bool Simulation::hitBoundary() const {
    if (currentPhoton_.direction().z() > 0 && currentPhoton_.position().z() > upperBoundary_->z) return true;
    if (currentPhoton_.direction().z() < 0 && currentLayer_->isFinite() && currentPhoton_.position().z() < upperBoundary_[1].z) return true;

    return false;
}

double Simulation::stepLeft(double stepped, double target) const {
    double reqStep = (target - currentPhoton_.position().z()) / currentPhoton_.direction().z();
    double dimfulStepLeft = stepped - reqStep;
    double dimlessStepLeft = dimfulStepLeft * currentLayer_->interactionRate();

    return dimlessStepLeft;
}

// assumes we have already hit a boundary
void Simulation::safeProcessBoundaries() {
    bool tryingToEscape = false;
    boundary_it hitBoundary;

    // this whole block could be replaced with a single ternary if we sacrifice should never reach
    if (currentPhoton_.position().z() > upperBoundary_->z) {
        hitBoundary = upperBoundary_;
        tryingToEscape = upperBoundary_ == begin(material_.boundaries_);
    } else if (currentLayer_->isFinite()) {
        hitBoundary = upperBoundary_ + 1; // there must be a lower a boundary if we hit it 
    } else {
        cout << "Should never reach" << std::endl;
    }

    double lastStep = currentPhoton_.unstep();

    stepLeft_ = stepLeft(lastStep, hitBoundary->z);
    currentPhoton_.stepToHeight(hitBoundary->z);

    if (tryingToEscape) {
        escape(*upperBoundary_);
    } else {
        reflect(*hitBoundary);
    }
}

void Simulation::flipDirection() {
    currentPhoton_.setDirection(
        currentPhoton_.direction().x(),
        currentPhoton_.direction().y(),
        -currentPhoton_.direction().z()
    );
}

void Simulation::reflect(Boundary& boundary) {
    double R = boundary.reflect(currentPhoton_.direction());
    double rand = sim_random();

    if (rand > R) {
        // transmit

        // if we're going deeper, advance iterator
        int change = currentPhoton_.direction().z() < 0 ? +1 : -1;

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

    // implements partial reflection
    double weightT = (1 - R) * currentPhoton_.weight();

    reflectance_.rawDrop(weightT, currentPhoton_.position().r());

    currentPhoton_.setWeight(currentPhoton_.weight() * R);

    flipDirection();

    if (tracking_) safeTrack(weightT);
}

void Simulation::safeTrack(double amount) {
    auto it = std::find(trackedDistances_.begin(), trackedDistances_.end(), currentPhoton_.position().r());
    
    if (it != trackedDistances_.end()) {
        while (absorptionHistory_.size() != 0) {
            it->absorption_.rawDrop(absorptionHistory_.top().amount_, absorptionHistory_.top().position_);
            absorptionHistory_.pop();
        }
    }
}

void Simulation::recordDrop() {
    double amount = currentLayer_->dropFrac()*currentPhoton_.weight();

    totalAbsorption_.rawDrop(amount, currentPhoton_.position());
    if (tracking_) absorptionHistory_.emplace(amount, currentPhoton_.position());
}

void Simulation::interact() {
    currentLayer_->interact(currentPhoton_, sim_random);
}

void Simulation::rouletteTerminate() {
    if (currentPhoton_.weight() < TERMINATION_THRESHOLD) {
        if (sim_random() < TERMINATION_CHANCE) {
            currentPhoton_.setWeight(currentPhoton_.weight() / TERMINATION_CHANCE);
        } else {
            currentPhoton_.setWeight(DEAD);
        }
    }
}

unsigned int Simulation::launchedPhotons() const {
    return photonsLaunched_;
}

const RadialTracker::row& Simulation::rawReflectance() const {
    return reflectance_.rawData();
}

RadialTracker::row Simulation::reflectance() const {
    return reflectance_.normData(photonsLaunched_);
}

BulkTracker::grid Simulation::absorption() const {
    return totalAbsorption_.normData(photonsLaunched_);
}

vector<BulkTracker::grid> Simulation::trackedAbsorption() const {
    vector<BulkTracker::grid> result;

    for (auto i : trackedDistances_) {
        BulkTracker::grid grid = i.absorption_.normData(photonsLaunched_);
        result.push_back(grid);
    }

    return result;
}