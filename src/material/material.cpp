#include "material.hpp"
#include "boundary.hpp"

using std::vector;

Material::Material(vector<Layer> layers, double nAir) : layers_(layers) {
    double runningSum = 0;
    double prevRefractiveIndex = nAir;
    for(auto layer : layers_) {
        double n = layer.n();
        boundaries_.emplace_back(-runningSum, prevRefractiveIndex, n);
        runningSum += layer.h();
        prevRefractiveIndex = n;
    }
}

const int Material::getLayerIndex(const CartVec& position) const {
    auto lower = std::lower_bound(boundaries_.begin(), boundaries_.end(), position.z(),
        [](const Boundary& b, double value) { return b.z < value; }
    );
    int layerIndex = std::distance(boundaries_.begin(), lower);
    return layerIndex;
}

const Layer& Material::getLayer(const CartVec& position) const {
    return layers_[getLayerIndex(position)];
}

const Boundary& Material::upperBoundary(const CartVec& position) const {
    return boundaries_[getLayerIndex(position)];
}

const Boundary& Material::lowerBoundary(const CartVec& position) const {
    return boundaries_[getLayerIndex(position) + 1];
}

std::ostream& operator<<(std::ostream& os, const Material& material) {
    os << "Material with " << material.layers_.size() << " layers:" << std::endl;
    for (auto l : material.layers_) {
        os << l << std::endl;
    }

    return os;
}