#include <cmath>
#include <utility>
#include <iostream>
using std::cout;

#include "boundary.hpp"

Boundary::Boundary(double z, double nAbove, double nBelow) : z(z), nAbove(nAbove), nBelow(nBelow) {  };

const double Boundary::reflect(CartVec& direction) {
    double& before = direction.z > 0 ? nBelow : nAbove;
    double& after = direction.z > 0 ? nAbove : nBelow;

    //cout << "before " << before << " after " << after << std::endl;

    double uz = std::abs(direction.z);
    double sinI = std::sin(uz);
    double sinT = before*sinI/after;
    //cout << uz << sinI << sinT << std::endl;
    // TIR check
    if (after < before && sinT > 1) {
        return 1;
    }

    double cosI = std::sqrt(1 - sinI*sinI);
    double cosT = std::sqrt(1 - sinT*sinT);

    double rootRs = (before*cosI - after*cosT) / (before*cosI + after*cosT);
    double rootRp = (before*cosT - after*cosI) / (before*cosT + after*cosI);

    double R = 0.5*(rootRs*rootRs + rootRp*rootRp);

    //cout << cosI << cosT << rootRs << rootRp << std::endl;

    return R;
}

const CartVec Boundary::refractionDirection(CartVec& direction) {
    double ratio = direction.z > 0 ? nBelow/nAbove : nAbove/nBelow; // before / after

    double sinI = std::abs(std::sin(direction.z));
    double cosI = std::sqrt(1 - sinI*sinI);
    double sinT = ratio*sinI;
    double cosT = std::sqrt(1 - sinT*sinT);

    CartVec newDirection = direction * ratio;

    newDirection.z += (direction.z > 0 ? -1 : +1)*(ratio*cosI - cosT);

    return newDirection;
}

std::ostream& operator<<(std::ostream& os, const Boundary& boundary) {
    os << "Boundary at z=" << boundary.z << " with nAbove=" << boundary.nAbove << ", nBelow=" << boundary.nBelow;

    return os;
}