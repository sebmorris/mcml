using std::cout;

#include "boundary.hpp"

Boundary::Boundary(double z, double nAbove, double nBelow) : z(z), nAbove(nAbove), nBelow(nBelow) {  };

double Boundary::reflect(const CartVec& direction) const {
    if (std::abs(nBelow - nAbove) < 1e-6) return 0; // matched n

    double before = direction.z() > 0 ? nBelow : nAbove;
    double after = direction.z() > 0 ? nAbove : nBelow;

    double uz = std::abs(direction.z());
    double sinI = std::sin(uz);
    double sinT = before*sinI/after;

    if (after < before && sinT > 1) {
        // TIR
        return 1;
    }

    double cosI = std::sqrt(1 - sinI*sinI);
    double cosT = std::sqrt(1 - sinT*sinT);

    double rootRs = (before*cosI - after*cosT) / (before*cosI + after*cosT);
    double rootRp = (before*cosT - after*cosI) / (before*cosT + after*cosI);

    double R = 0.5*(rootRs*rootRs + rootRp*rootRp);

    return R;
}

CartVec Boundary::refractionDirection(const CartVec& direction) const {
    if (std::abs(nBelow - nAbove) < 1e-6) return direction; // matched n

    double ratio = direction.z() > 0 ? nBelow/nAbove : nAbove/nBelow; // before / after

    // think I corrected this but needs testing
    double cosI = direction.z();
    double sinI = std::sqrt(1 - cosI*cosI);
    double sinT = ratio*sinI;
    double cosT = std::sqrt(1 - sinT*sinT);

    double uz = (direction.z() > 0 ? +1 : -1)*cosT;

    return CartVec(direction.x()*ratio, direction.y()*ratio, uz);
}

std::ostream& operator<<(std::ostream& os, const Boundary& boundary) {
    os << "Boundary at z=" << boundary.z << " with nAbove=" << boundary.nAbove << ", nBelow=" << boundary.nBelow;

    return os;
}