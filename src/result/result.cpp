#include <iostream>
using std::cout;

#include <cmath>

#include <fstream>

#include "result.hpp"

constexpr double pi = 3.14159265358979323846;

Result::Result() : reflectance{0}, absorption{0} {  };

inline double Result::r(double x, double y) {
    return std::sqrt(x*x + y*y);
}

inline const int Result::radialIndex(CartVec& position) {
    return radialIndex(position.x, position.y);
}

int Result::radialIndex(double x, double y) {
    return std::floor(r(x, y) / RADIAL_BIN_SIZE);
}

inline const int Result::heightIndex(CartVec& position) {
    return heightIndex(position.z);
}

int Result::heightIndex(double z) {
    return std::floor(std::abs(z) / HEIGHT_BIN_SIZE);
}

inline const int Result::reflectanceIndex(CartVec& position) {
    return reflectanceIndex(position.x, position.y);
}

int Result::reflectanceIndex(double x, double y) {
    return std::floor(r(x, y) / REFLECTANCE_BIN_SIZE);
}

const void Result::drop(CartVec& position, double amount) {
    int i = radialIndex(position);
    int j = heightIndex(position);

    if (i >= RADIAL_BINS || j >= HEIGHT_BINS) {
        absorptionOverflow += amount;
        return;
    }

    //cout << "i " << i << " j " << j << " for position " << position << std::endl;
    absorption[i][j] += amount;
}

const void Result::escape(CartVec& position, double amount) {
    int i = reflectanceIndex(position);

    if (i >= REFLECTANCE_BINS) {
        reflectanceOverflow += amount;
        return;
    }
    reflectance[i] += amount;
}

void Result::save(int N) {
    double resultAbs[RADIAL_BINS][HEIGHT_BINS]{};
    for (int i = 0; i < RADIAL_BINS; i++) {
        for (int j = 0; j < HEIGHT_BINS; j++) {
            resultAbs[i][j] = absorption[i][j] / (2 * pi * (i + 0.5) * RADIAL_BIN_SIZE * RADIAL_BIN_SIZE * HEIGHT_BIN_SIZE * N);
        }
    }

    std::ofstream outAbs("absorption.csv");

    for (auto& row : resultAbs) {
        for (int j = 0; j < RADIAL_BINS; j++) {
            outAbs << row[j];
            if (j != RADIAL_BINS - 1) outAbs << ',';
        }
        outAbs << '\n';
    }

    double resultRef[REFLECTANCE_BINS]{};
    for (int i = 0; i < REFLECTANCE_BINS; i++) {
        resultRef[i] = reflectance[i] / (2 * pi * (i + 0.5) * REFLECTANCE_BIN_SIZE * REFLECTANCE_BIN_SIZE * N);
    }

    std::ofstream outRef("reflectance.csv");

    for (int j = 0; j < REFLECTANCE_BINS; j++) {
        outRef << resultRef[j];
        if (j != RADIAL_BINS - 1) outRef << ',';
    }
}