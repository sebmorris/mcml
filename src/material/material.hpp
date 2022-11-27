#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>

#include "../layer/layer.hpp"
#include "../cartvec/cartvec.hpp"
#include "boundary.hpp"

#include <vector>
using std::vector;

struct Material {
    const int num_layers;
    vector<Layer> layers;
    vector<Boundary> boundaries;
    
    Material(vector<Layer> layers);

    const Boundary upperBoundary(const CartVec& position);
    const Boundary lowerBoundary(const CartVec& position);
    const Layer& getLayer(const CartVec& position);
    const int getLayerIndex(const CartVec& position);
};

std::ostream& operator<<(std::ostream& os, const Material& material);

#endif