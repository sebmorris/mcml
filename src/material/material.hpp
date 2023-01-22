#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <algorithm>

#include "../layer/layer.hpp"
#include "../cartvec/cartvec.hpp"
#include "boundary.hpp"

#include <vector>
#include <iostream>

struct Material {
    // spin and drop in layers
    std::vector<Layer> layers_;

    // reflect from boundaries
    // the boundary vector precomputes some information
    std::vector<Boundary> boundaries_;

    Material() = delete;
    Material(std::vector<Layer> layers, double nAir = 1);

    const Boundary& upperBoundary(const CartVec& position) const;
    const Boundary& lowerBoundary(const CartVec& position) const;
    const Layer& getLayer(const CartVec& position) const;
    const int getLayerIndex(const CartVec& position) const;
};

std::ostream& operator<<(std::ostream& os, const Material& material);

#endif