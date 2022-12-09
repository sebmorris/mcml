#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>

#include "../layer/layer.hpp"
#include "../cartvec/cartvec.hpp"
#include "boundary.hpp"

#include <vector>
using std::vector;

struct Material {
    vector<Layer> layers_;
    vector<Boundary> boundaries_;
    
    Material(vector<Layer> layers, double);

    const Boundary& upperBoundary(const CartVec& position) const;
    const Boundary& lowerBoundary(const CartVec& position) const;
    const Layer& getLayer(const CartVec& position) const;
    const int getLayerIndex(const CartVec& position) const;
};

std::ostream& operator<<(std::ostream& os, const Material& material);

#endif