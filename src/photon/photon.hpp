#ifndef PHOTON_H
#define PHOTON_H

#include <iostream>
#include <stack>

#include "../cartvec/cartvec.hpp"

extern const double ALIVE;
extern const double DEAD;

/*
keeps track of macro & micro dofs
facilitates dof control of the "photon"

perhaps should log positions separately
*/
struct Photon {
    CartVec position; // position
    CartVec direction; // direction cosines

    std::stack<CartVec> directionHistory; // history of previous directions

    double weight; // starts $ALIVE, ends $DEAD

    void step(const double size);
    void unstep(); // un(do) the last step
    double stepToHeight(const double z); // returns step size
    void changeDirection(double x, double y, double z);

    Photon();
    Photon(CartVec, CartVec);
    Photon(CartVec, CartVec, double);
};

std::ostream &print(std::ostream &os, const Photon &photon);

#endif