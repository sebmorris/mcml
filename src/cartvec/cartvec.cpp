#include "cartvec.hpp"

CartVec::CartVec() : x(0), y(0), z(0) {  };
CartVec::CartVec(double x, double y, double z) : x(x), y(y), z(z) {  };

const void CartVec::operator+=(const CartVec& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
}

CartVec operator*(const double lhs, const CartVec& vec) {
    CartVec newVec(vec.x * lhs, vec.y * lhs, vec.z * lhs);
    return newVec;
}

CartVec operator*(const CartVec& vec, const double rhs) {
    CartVec newVec(vec.x * rhs, vec.y * rhs, vec.z * rhs);
    return newVec;
}

std::ostream& operator<<(std::ostream& os, const CartVec& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";

    return os;
}