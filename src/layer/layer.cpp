#include <iostream>
using std::cout;

#include <limits>

#include "layer.hpp"

Layer::Layer(double g, double n, double mu_a, double mu_s) : g(g), n(n), mu_a(mu_a), mu_s(mu_s), infinite(true) { 
    height = std::numeric_limits<double>::infinity();
}
Layer::Layer(double g, double n, double mu_a, double mu_s, double height) : g(g), n(n), mu_a(mu_a), mu_s(mu_s), height(height), infinite(false) { }

std::ostream& operator<<(std::ostream& os, const Layer& layer) {
    os << (layer.infinite ? "(Infinite layer)" : "(Finite layer, h=" + std::to_string(layer.height) + ")")
        << " (mu_a=" << layer.mu_a << ") (mu_s=" << layer.mu_s << ") (g=" << layer.g
        << ") (n=" << layer.n << ")";
    
    return os;
}