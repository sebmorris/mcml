#include <algorithm>

#include "material.hpp"
#include "boundary.hpp"

Material::Material(vector<Layer> layers) : layers(layers), num_layers(layers.size()) {
    float nLast = 1; // refractive index of air taken to be one
    double runningSum = 0;
    for(Layer l : layers) {
        boundaries.push_back(Boundary(-runningSum, nLast, l.n));
        runningSum += l.height;
        nLast = l.n;
    }
}

const int Material::getLayerIndex(const CartVec& position) const {
    auto lower = std::lower_bound(boundaries.begin(), boundaries.end(), position.z(),
        [](const Boundary& b, double value) { return b.z < value; }
    );
    int layerIndex = std::distance(boundaries.begin(), lower);
    return layerIndex;
}

const Layer& Material::getLayer(const CartVec& position) const {
    return layers[getLayerIndex(position)];
}

const Boundary& Material::upperBoundary(const CartVec& position) const {
    return boundaries[getLayerIndex(position)];
}

const Boundary& Material::lowerBoundary(const CartVec& position) const {
    return boundaries[getLayerIndex(position) + 1];
}

std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << "Material with " << material.num_layers << " layers:" << std::endl;
    for (auto l : material.layers) {
        os << l << std::endl;
    }

    return os;
}