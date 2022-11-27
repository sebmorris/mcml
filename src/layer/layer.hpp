#ifndef LAYER_H
#define LAYER_H

#include <iostream>

struct Layer {
    double g;
    double n;
    double mu_a, mu_s;
    double height;
    bool infinite;

    Layer(double g, double n, double mu_a, double mu_s);
    Layer(double g, double n, double mu_a, double mu_s, double height);
};

std::ostream& operator<<(std::ostream& os, const Layer& layer);

#endif