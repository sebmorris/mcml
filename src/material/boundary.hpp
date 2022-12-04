#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "../cartvec/cartvec.hpp"

#include <cmath>
#include <utility>
#include <iostream>

struct Boundary {
    double z;
    double nAbove;
    double nBelow;

    double reflect(const CartVec& direction) const;
    CartVec refractionDirection(const CartVec& direction) const;

    Boundary(double z, double nAbove, double nBelow);

    // total internal reflection and fresnel calculation functions
    // need to overload > operator to compare z
};

std::ostream& operator<<(std::ostream& os, const Boundary& boundary);

#endif