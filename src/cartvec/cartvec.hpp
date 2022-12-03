#ifndef CARTVEC_H
#define CARTVEC_H

#include <iostream>
#include <cmath>

struct CartVec {
    private:
        double xc;
        double yc;
        double zc;
    public:
        CartVec();
        CartVec(double, double, double);

        double magnitude();

        void operator+=(const CartVec& rhs);

        double x() const;
        double y() const;
        double z() const;
};

const CartVec operator*(double lhs, const CartVec& vec);
const CartVec operator*(const CartVec& vec, double rhs);

const CartVec operator+(const CartVec& lhs, const CartVec& rhs);
const CartVec operator-(const CartVec& lhs, const CartVec& rhs);
std::ostream& operator<<(std::ostream& os, const CartVec& vec);

#endif