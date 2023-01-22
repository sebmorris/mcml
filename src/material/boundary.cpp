#include "boundary.hpp"

using std::cout;

Boundary::Boundary(double z, double nAbove, double nBelow) : z(z), nAbove(nAbove), nBelow(nBelow) {  };

double Boundary::reflect(const CartVec& direction) const {
    if (std::abs(nBelow - nAbove) < 1e-10 /* small number */) return 0; // matched n

    // before = going_deeper ? nAbove : nBelow
    double before = direction.z() < 0 ? nAbove : nBelow;
    double after = direction.z() < 0 ? nBelow : nAbove;

    // transmission cosT is always positive
    // easier to follow through if we ensure cosI is positive
    double cosI = std::abs(direction.z());
    double sinI = std::sqrt(1 - cosI*cosI);

    double sinT = before*sinI/after;
    double cosT = std::sqrt(1 - sinT*sinT);

    if (sinT > 1) {
        // TIR
        return 1;
    } else if(cosI > 1 - 1e-12) {
        // normal incidence
        return rSpecular(direction.z() < 0);
    } else if (cosI < 1e-6) {
        // very oblique incidence
        // TODO: check this since mcml sets r = 1 here
        return 1;
    }

    double cap = cosI*cosT - sinI*sinT;
    double cam = cosI*cosT + sinI*sinT;
    double sap = sinI*cosT + cosI*sinT;
    double sam = sinI*cosT - cosI*sinT;
    double R = 0.5*sam*sam*(cam*cam+cap*cap)/(sap*sap*cam*cam); 

    return R;
}

CartVec Boundary::refractionDirection(const CartVec& direction) const {
    if (std::abs(nBelow - nAbove) < 1e-6) return direction; // matched n

    double ratio = direction.z() < 0 ? nAbove/nBelow : nBelow/nAbove; // before / after

    double cosI = std::abs(direction.z());
    double sinI = std::sqrt(1 - cosI*cosI);

    double sinT = ratio*sinI;
    double cosT = std::sqrt(1 - sinT*sinT); // always positive

    if (cosI > 1 - 1e-12 /*cos(~0)*/) {
        return CartVec(direction.x()*ratio, direction.y()*ratio, direction.z());
    }


    // if we're going deeper, we're still going deeper
    double uz = (direction.z() < 0 ? -1 : +1)*cosT;
    CartVec afterDirection(direction.x()*ratio, direction.y()*ratio, uz);

    return afterDirection;
}

// movingDown true for passing nAbove->nBelow (uz < 0)
double Boundary::rSpecular(bool movingDown) const {
    double before = movingDown ? nAbove : nBelow;
    double after = movingDown ? nBelow : nAbove;

    double rootR = (before - after) / (before + after);

    return rootR*rootR;
}

std::ostream& operator<<(std::ostream& os, const Boundary& boundary) {
    os << "Boundary at z=" << boundary.z << " with nAbove=" << boundary.nAbove << ", nBelow=" << boundary.nBelow;

    return os;
}