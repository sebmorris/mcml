#ifndef PHOTON_H
#define PHOTON_H

#include <iostream>

#include "../cartvec/cartvec.hpp"

const double ALIVE = 1;
const double DEAD = 0;

/*
keeps track of macro & micro dofs
facilitates dof control of the "photon"

perhaps should log positions separately
*/
class Photon {
    private:
        CartVec position_;
        CartVec lastPosition_; // can only go back once
        CartVec direction_; // direction cosines

        double weight_; // starts $ALIVE, ends $DEAD
    public:
        // accessors
        double weight() const;
        const CartVec& position() const;
        const CartVec& direction() const;

        // control
        void step(double size);
        void stepToHeight(double z);
        double unstep(); // un(do) the last step, return the step
        void setDirection(double x, double y, double z);
        void setDirection(const CartVec&);

        void incrementWeight(double);
        void setWeight(double);

        Photon();
        Photon(CartVec, CartVec);
};

std::ostream &print(std::ostream &os, const Photon &photon);

#endif