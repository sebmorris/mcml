#include <iostream>
using std::cout;

#include <cmath>
#include <limits>

#include "simulation.hpp"

constexpr double pi = 3.14159265358979323846;

Simulation::Simulation(Material material) : material(material), results() {
    launch();
};

inline double Simulation::random() {
    return distribution(generator);
}

void Simulation::nextPhoton() {
    if (currentPhoton.weight == DEAD) {
        //cout << "Photon dead, " << photonsUsed << " photons used" << std::endl;
        launch();
        //cout << "direction history size " << currentPhoton.directionHistory.size() << std::endl;
    }
    while (currentPhoton.weight != DEAD) {
        //cout << "next" << std::endl;
        next();
    }
}

void Simulation::launch() {
    currentPhoton = Photon();
    upperBoundary = begin(material.boundaries);
    currentLayer = begin(material.layers);
}

void Simulation::next() {
    //cout << "Starting another one" << std::endl;
    //cout << "Going next (w=" << currentPhoton.weight << "), position " << currentPhoton.position << " direction " << currentPhoton.direction << std::endl;

    //cout << *upperBoundary << std:: endl;

    hop();

    //cout << "Photon weight, " << currentPhoton.weight << std::endl;

    if (hitBoundary()) {
        //cout << "Hit a boundary" << std::endl;
        processBoundaries();
        return;
    }

    drop();
    spin();
    terminate();
}

void Simulation::hop() {
    double rand = random();
    double mu_t = (*currentLayer).mu_a + (*currentLayer).mu_s;
    double step = -std::log(rand) / mu_t;
    //cout << "Random (" << rand << ")" << " hop, stepping " << step << std::endl;
    currentPhoton.step(step);
    //cout << "New position " << currentPhoton.position << std::endl;
}

bool Simulation::hitBoundary() {
    if (currentPhoton.direction.z > 0 && currentPhoton.position.z > upperBoundary->z) return true;
    if (currentPhoton.direction.z < 0 && !currentLayer->infinite && currentPhoton.position.z < (upperBoundary+1)->z) {
        //cout << "Hit lower boundary" << std::endl;
        return true;
    }

    return false;
}

void Simulation::processBoundaries() {
    if (currentPhoton.position.z > upperBoundary->z) {
        bool tryingToEscape = currentPhoton.position.z > 0;
        currentPhoton.unstep();
        currentPhoton.stepToHeight(upperBoundary->z);
        if (tryingToEscape) {
            escape(*upperBoundary);
        } else {
            reflect(*upperBoundary);
        }
    }
    if (!currentLayer->infinite) {
        auto lowerBoundary = *(upperBoundary+1);
        if (currentPhoton.position.z < lowerBoundary.z) {
            currentPhoton.unstep();
            currentPhoton.stepToHeight(lowerBoundary.z);
            reflect(lowerBoundary);
        }
    }
}

const void Simulation::reflect(Boundary& boundary) {
    double R = boundary.reflect(currentPhoton.direction);
    double rand = random();

    if (rand > R) {
        // transmit
        int change = currentPhoton.direction.z > 0 ? -1 : 1;

        //cout << "Change " << change << std::endl;
        
        currentLayer += change;
        upperBoundary += change;

        currentPhoton.direction = boundary.refractionDirection(currentPhoton.direction);
    } else {
        // reflect
        currentPhoton.direction.z = -currentPhoton.direction.z;
    }
}

const void Simulation::escape(Boundary& boundary) {
    // need to drop some in the appropriate reflectance bin
    // reflect the rest back in
    double R = boundary.reflect(currentPhoton.direction);

    double weightT = (1 - R) * currentPhoton.weight;
    results.escape(currentPhoton.position, weightT);

    //cout << currentPhoton.direction << " " << currentPhoton.position << std::endl;
    //cout << "Escaped (R=" << R << ") weight w=" << weightT << std::endl;
    currentPhoton.weight -= weightT;
    currentPhoton.direction.z = -currentPhoton.direction.z;
}

void Simulation::drop() {
    double amount = (*currentLayer).mu_a / ((*currentLayer).mu_a + (*currentLayer).mu_s);
    currentPhoton.weight -= amount;

    //cout << "Dropping " << amount << " at position " << currentPhoton.position << std::endl;

    results.drop(currentPhoton.position, amount);
}

void Simulation::spin() {
    // eq. 5.45, 5.46, 5.47a of:
    // Monte Carlo Modeling of Light Transport in Tissue (Steady State and Time of Flight) (Jacques 2011)
    double rand = random();

    // avoiding messy formulas
    double& g = (*currentLayer).g;
    double& ux = currentPhoton.direction.x;
    double& uy = currentPhoton.direction.y;
    double& uz = currentPhoton.direction.z;

    //cout << "Spinning in layer: " << *currentLayer << std::endl;

    //cout << "g=" << g << " direction= " << currentPhoton.direction << std::endl;

    // I chose g*g etc.
    // apparently it's more efficient
    // https://stackoverflow.com/a/2940800
    double num_factor = (1 - g*g) / (1 - g + 2*g*rand);
    double num = 1 + g*g - num_factor*num_factor;

    double cosTheta = num / (2*g);
    double sinTheta = std::sqrt(1 - cosTheta*cosTheta);
    double cosPhi = std::cos(2*pi*rand);
    double sinPhi = std::sin(2*pi*rand);

    //cout << num_factor << num << cosTheta << sinTheta << cosPhi << sinPhi << std::endl;

    double temp = std::sqrt(1 - uz*uz);

    if (1 - std::abs(uz) < 1e-6) {
        double uxx = sinTheta*cosPhi;
        double uyy = sinTheta*sinPhi;
        double uzz = (uz > 0 ? 1 : -1)*cosTheta;

        currentPhoton.changeDirection(uxx, uyy, uzz);
        return;
    }

    //cout << "temp " << temp << std::endl;

    double uxx = sinTheta*(ux*uz*cosPhi - uy*sinPhi)/temp + ux*cosTheta;
    double uyy = sinTheta*(uy*uz*cosPhi + ux*sinPhi)/temp + uy*cosTheta;
    double uzz = -sinTheta*cosPhi*temp + uz*cosTheta;

    //cout << "New direction " << uxx << " " << uyy << " " << uzz << std::endl;

    currentPhoton.changeDirection(uxx, uyy, uzz);
}

void Simulation::terminate() {
    if (currentPhoton.weight < TERMINATION_THRESHOLD) {
        if (random() <= TERMINATION_CHANCE) {
            currentPhoton.weight /= TERMINATION_CHANCE;
        } else {
            //cout << "Killed the photon at " << currentPhoton.position << std::endl;
            photonsUsed++;
            currentPhoton.weight = DEAD;
        }
    }
}