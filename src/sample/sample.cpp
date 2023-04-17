#include "sample.hpp"

using std::vector;

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
        baseLayer.infinite = false;
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

ParameterSimulationMaterialOptions sampleParameterMaterials(Random random, double nAir) {
    vector<Material> mats{};

    std::size_t n = waterExtinction.size();

    double oneWater = random.random_beta(5, 2);
    double twoWater = random.random_beta(5, 2);

    double oneLipid = random.random_beta(2, 2);
    double twoLipid = random.random_beta(2, 2);
    
    // units uM
    double oneBlood = random.jeffreys_random_between(1, 100);
    double twoBlood = random.jeffreys_random_between(1, 100);
    
    double oneOxyFrac = random.random_beta(5, 2);
    double twoOxyFrac = random.random_beta(5, 2);

    double oneA = random.jeffreys_random_between(5, 80);
    double twoA = random.jeffreys_random_between(5, 80);

    double oneB = random.jeffreys_random_between(0.2, 3);
    double twoB = random.jeffreys_random_between(0.2, 3);

    double oneH = random.jeffreys_random_between(5, 15);
    double csfH = random.jeffreys_random_between(1, 10);

    for (std::size_t i = 0; i < n; i++) {
        // absorption cm-1
        double oneAbs = oneWater*waterExtinction[i]*55.5 + oneLipid*lipidExtinction[i] + 
            oneBlood*oneOxyFrac*oxyExtinction[i]*1e-6 + oneBlood*(1 - oneOxyFrac)*deoxyExtinction[i]*1e-6;
        double twoAbs = twoWater*waterExtinction[i]*55.5 + twoLipid*lipidExtinction[i] + 
            twoBlood*twoOxyFrac*oxyExtinction[i]*1e-6 + twoBlood*(1 - twoOxyFrac)*deoxyExtinction[i]*1e-6;
    

        // also cm-1 w/ *10 due to conversion from reduced scattering coeff        
        double oneScatter = oneA*std::pow(wavelengths[i] / 500, -1*oneB)*10;
        double twoScatter = twoA*std::pow(wavelengths[i] / 500, -1*twoB)*10;

        BaseLayerOptions oneOptions{1.4, oneAbs, oneH};
        BaseLayerOptions twoOptions{1.4, twoAbs};
        BaseLayerOptions csfOptions{1.4, 0.0, csfH};

        Layer oneL{oneOptions, oneScatter, 0.9};
        Layer twoL{twoOptions, twoScatter, 0.9};
        Layer csfL{csfOptions, 1, 0.9};

        vector<Layer> layers{oneL, csfL, twoL};

        mats.emplace_back(layers, nAir);
    }

    ParameterSimulationLayerOptions optionsOne;
    optionsOne.n = 1.4;
    optionsOne.height = oneH;
    optionsOne.g = 0.9;

    optionsOne.a = oneA;
    optionsOne.b = oneB;
    
    optionsOne.cBlood = oneBlood;
    optionsOne.fOxy = oneOxyFrac;
    optionsOne.fWater = oneWater;
    optionsOne.fLipid = oneLipid;
    optionsOne.concentrationLayer = true;
    optionsOne.constantLayer = false;

    ParameterSimulationLayerOptions optionsTwo;
    optionsTwo.n = 1.4;
    optionsTwo.g = 0.9;

    optionsTwo.a = twoA;
    optionsTwo.b = twoB;
    
    optionsTwo.cBlood = twoBlood;
    optionsTwo.fOxy = twoOxyFrac;
    optionsTwo.fWater = twoWater;
    optionsTwo.fLipid = twoLipid;
    optionsTwo.concentrationLayer = true;
    optionsTwo.constantLayer = false;

    ParameterSimulationLayerOptions optionsCSF;
    optionsCSF.n = 1.4;
    optionsCSF.height = csfH;
    optionsCSF.g = 0.9;

    optionsCSF.mu_a = 0;
    optionsCSF.mu_s = 1;
    optionsCSF.concentrationLayer = false;
    optionsCSF.constantLayer = true;

    vector<ParameterSimulationLayerOptions> options{optionsOne, optionsCSF, optionsTwo};

    ParameterSimulationMaterialOptions result;
    result.material = mats;
    result.layers = options;

    return result;
}