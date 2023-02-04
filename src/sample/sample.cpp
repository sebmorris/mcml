#include "sample.hpp"

/*
    All four layers require g, mu_a, mu_s values

    The first 3 layers are additionally specified by height
*/

Layer sim_random_parameter_values(LayerParameterValues lower, LayerParameterValues upper, bool infinite, Random random) {
    double n = random.random_between(lower.n, upper.n);
    double g = random.random_between(lower.g, upper.g);
    double mu_a = random.random_between(lower.mu_a, upper.mu_a);
    double mu_s = random.random_between(lower.mu_s, upper.mu_s);

    BaseLayerOptions baseLayer{n, mu_a};
    if (!infinite) {
        double height = random.random_between(lower.height, upper.height);
        baseLayer.height = height;
        baseLayer.infinite = true;
    }

    return Layer{baseLayer, mu_s, g};
}

Material sampleMaterial(Random random, double nAir) {
    std::vector<Layer> layers;

    for (size_t i = 0; i < numLayers; i++) {
        layers.push_back(sim_random_parameter_values(layerParameterRanges[i][0], layerParameterRanges[i][1], i == numLayers - 1, random));
    }

    return Material{layers, nAir};
}