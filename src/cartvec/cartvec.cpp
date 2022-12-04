#include "cartvec.hpp"

CartVec::CartVec() : xc(0), yc(0), zc(0) {  };
CartVec::CartVec(double x, double y, double z) : xc(x), yc(y), zc(z) {  };

double CartVec::x() const { return xc; }
double CartVec::y() const { return yc; }
double CartVec::z() const { return zc; }
double CartVec::r() const { return std::sqrt(xc*xc + yc*yc); }

double CartVec::magnitude() {
    return std::sqrt(xc*xc + yc*yc + zc*zc);
}

void CartVec::operator+=(const CartVec& rhs) {
    xc += rhs.x();
    yc += rhs.y();
    zc += rhs.z();
}

CartVec& CartVec::operator=(const CartVec& rhs) {
    xc = rhs.x();
    yc = rhs.y();
    zc = rhs.z();
}

const CartVec operator*(double lhs, const CartVec& vec) {
    CartVec newVec(vec.x() * lhs, vec.y() * lhs, vec.z() * lhs);
    return newVec;
}

const CartVec operator*(const CartVec& vec, double rhs) {
    return rhs * vec; // commutative
}

const CartVec operator+(const CartVec& lhs, const CartVec& rhs) {
    CartVec newVec(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
    return newVec;
}

const CartVec operator-(const CartVec& lhs, const CartVec& rhs) {
    return lhs + (-1*rhs);
}

std::ostream& operator<<(std::ostream& os, const CartVec& vec) {
    os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";

    return os;
}