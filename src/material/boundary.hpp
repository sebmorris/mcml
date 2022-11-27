#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "../cartvec/cartvec.hpp"

struct Boundary {
    double z;
    double nAbove;
    double nBelow;

    const double reflect(CartVec& direction);
    const CartVec refractionDirection(CartVec& direction);

    Boundary(double z, double nAbove, double nBelow);

    // total internal reflection and fresnel calculation functions
    // need to overload > operator to compare z
};

std::ostream& operator<<(std::ostream& os, const Boundary& boundary);

#endif