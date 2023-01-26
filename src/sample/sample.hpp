#ifndef SAMPLE_H

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

Layer sim_random_parameter_values(LayerParameterValues lower, LayerParameterValues upper, bool infinite);

const size_t numLayers = 4;
const LayerParameterValues layerParameterRanges[4][2] = {
    { LayerParameterValues{0, 1, 2, 3},     LayerParameterValues{0, 1, 2, 3} },
    { LayerParameterValues{0, 1, 2, 3},     LayerParameterValues{0, 1, 2, 3} },
    { LayerParameterValues{0, 1, 2, 3},     LayerParameterValues{0, 1, 2, 3} },
    { LayerParameterValues{0, 1, 2},        LayerParameterValues{0, 1, 2} }
};

Material sampleFourLayerMaterial(double nAir = 1);

#endif