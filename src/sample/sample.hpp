#ifndef SAMPLE_H
#define SAMPLE_H

#include <vector>

#include "../layer/layer.hpp"
#include "../material/material.hpp"
#include "../random/random.hpp"

/*
    All four layers require g, mu_a, mu_s values

    The first 3 layers are additionally specified by height
*/

struct LayerParameterValues {
    double n;
    double g;
    double mu_a;
    double mu_s;
    double height;
};

Layer sim_random_parameter_values(LayerParameterValues lower, LayerParameterValues upper, bool infinite, Random);

const size_t numLayers = 3;
/*const LayerParameterValues layerParameterRanges[numLayers][2] = {
    { LayerParameterValues{1.33, 0.9, 0.0015,   1,      5},     LayerParameterValues{1.5, 0.9, 0.28,    100,    15} },
    { LayerParameterValues{1.33, 0.9, 0,        0.2,    1},     LayerParameterValues{1.5, 0.9, 0,       0.2,    5} }, // csf broadly fixed layer
    { LayerParameterValues{1.33, 0.9, 0.0015, 1},     LayerParameterValues{1.5, 0.9, 1, 100} } // semi-infinite
};*/

const LayerParameterValues layerParameterRanges[numLayers][2] = {
    { LayerParameterValues{1.4, 0.9, 0.0015,   1,      5},     LayerParameterValues{1.4, 0.9, 0.28,    100,    15} },
    { LayerParameterValues{1.4, 0.9, 0,        0.2,    1},     LayerParameterValues{1.4, 0.9, 0,       0.2,    5} }, // csf broadly fixed layer
    { LayerParameterValues{1.4, 0.9, 0.0015, 1},     LayerParameterValues{1.4, 0.9, 1, 100} } // semi-infinite
};

Material sampleMaterial(Random, double nAir = 1);

#endif