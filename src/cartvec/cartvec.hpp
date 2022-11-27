#ifndef CARTVEC_H
#define CARTVEC_H

#include <iostream>

struct CartVec {
    double x;
    double y;
    double z;

    CartVec();
    CartVec(double, double, double);

    const void operator+=(const CartVec& rhs);
};

CartVec operator*(const double lhs, const CartVec& vec);
CartVec operator*(const CartVec& vec, const double rhs);
std::ostream& operator<<(std::ostream& os, const CartVec& vec);

#endif