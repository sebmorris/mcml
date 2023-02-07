#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <limits>
#include <cstdint>

constexpr double pi = 3.14159265358979323846;
constexpr double inf = std::numeric_limits<double>::infinity();
constexpr uint64_t SIMULATIONS_PER_THREAD = 1000;
constexpr uint64_t PHOTONS_PER_SIMULATION = 1e6;
constexpr uint64_t PHOTONS_PER_REPORT = 1e5;

#endif