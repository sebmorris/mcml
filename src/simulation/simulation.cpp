#include <iostream>
using std::cout;

#include <cmath>
#include <limits>

#include "simulation.hpp"

constexpr double pi = 3.14159265358979323846;

Simulation::Simulation(Material material) : material(material), results() {
    photonsLaunched = 1;
    launch();
};

inline double Simulation::random() {
    return distribution(generator);
}

void Simulation::nextPhoton() {
    if (currentPhoton.weight == DEAD) {
        photonsLaunched++;
        launch();
    }
    while (currentPhoton.weight != DEAD) {
        next();
    }
}

void Simulation::launch() {
    // essentially a reset function
    currentPhoton = Photon();
    stepLeft = 0;
    upperBoundary = begin(material.boundaries);
    currentLayer = begin(material.layers);
}

void Simulation::next() {
    // potentially uses up stepLeft after a boundary
    hop();

    if (hitBoundary()) {
        processBoundaries();
    } else {
        drop();
        spin();
        terminate();
    }
}

void Simulation::hop() {
    double step;
    if (stepLeft == 0) {
        double rand = random();
        step = -std::log(rand) / (currentLayer->mu_a + currentLayer->mu_s);
        currentPhoton.step(step);
    } else {
        cout << "Using some leftover step " << stepLeft << std::endl;
        // this has not been tested
        step = stepLeft / (currentLayer->mu_a + currentLayer->mu_s);
        stepLeft = 0;
        currentPhoton.step(step);
    }
}

bool Simulation::hitBoundary() {
    if (currentPhoton.direction.z > 0 && currentPhoton.position.z > upperBoundary->z) return true;
    if (currentPhoton.direction.z < 0 && !currentLayer->infinite && currentPhoton.position.z < (upperBoundary+1)->z) return true;

    return false;
}

void Simulation::processBoundaries() {
    if (currentPhoton.position.z > upperBoundary->z) {
        bool tryingToEscape = currentPhoton.position.z > 0;
        double stepped = currentPhoton.unstep();
        // stepLeft not used by verification model as R=0 with matched n
        stepLeft = stepped - (currentPhoton.position.z - upperBoundary->z) / currentPhoton.direction.z;
        currentPhoton.stepToHeight(upperBoundary->z);
        if (tryingToEscape) {
            escape(*upperBoundary);
        } else {
            cout << "impossible reach reflection" << std::endl;
            reflect(*upperBoundary);
            
        }
    }
    if (!currentLayer->infinite) {
        cout << "impossible reach lower" << std::endl;
        auto lowerBoundary = *(upperBoundary+1);
        if (currentPhoton.position.z < lowerBoundary.z) {
            double stepped = currentPhoton.unstep();
            stepLeft = stepped - (currentPhoton.position.z - lowerBoundary.z) / currentPhoton.direction.z;
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

        currentLayer += change;
        upperBoundary += change;

        currentPhoton.direction = boundary.refractionDirection(currentPhoton.direction);
    } else {
        // reflect
        currentPhoton.direction.z = -currentPhoton.direction.z;
    }
}

const void Simulation::escape(Boundary& boundary) {
    double R = boundary.reflect(currentPhoton.direction);

    double weightT = (1 - R) * currentPhoton.weight;
    results.escape(currentPhoton.position, weightT);

    currentPhoton.weight -= weightT;
    currentPhoton.direction.z = -currentPhoton.direction.z;
}

void Simulation::drop() {
    double amount = (currentLayer->mu_a / (currentLayer->mu_a + currentLayer->mu_s))*currentPhoton.weight;
    currentPhoton.weight -= amount;

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

        currentPhoton.changeDirection(uxx, uyy, uzz);
    } else {
        double uxx = sinTheta*(ux*uz*cosPhi - uy*sinPhi)/temp + ux*cosTheta;
        double uyy = sinTheta*(uy*uz*cosPhi + ux*sinPhi)/temp + uy*cosTheta;
        double uzz = -sinTheta*cosPhi*temp + uz*cosTheta;

        currentPhoton.changeDirection(uxx, uyy, uzz);
    }
}

void Simulation::terminate() {
    if (currentPhoton.weight < TERMINATION_THRESHOLD) {
        if (random() < TERMINATION_CHANCE) {
            currentPhoton.weight /= TERMINATION_CHANCE;
        } else {
            currentPhoton.weight = DEAD;
        }
    }
}