#ifndef LAYER_H
#define LAYER_H

#include <iostream>
#include <cmath>
#include <memory>
#include <stdexcept>

#include "../constants/constants.hpp"
#include "../photon/photon.hpp"
#include "../random/random.hpp"

/*
Look up if constexpr is the appropriate thing to use in some cases here
Sort out constructors 
Do I need to add a virtual destructor?
*/

struct BaseLayerOptions {
    double height;
    bool infinite;
    double n;
    double mu_a;

    BaseLayerOptions(double, double);
    BaseLayerOptions(double, double, double);
};

// virtual base class stores non-scattering information
class BaseLayer {
    friend class Layer;
    protected:
        double n_;
        double mu_a_;
        double height_;
        bool infinite_;

        void drop(Photon&) const;
        double dropFrac() const;
        double n() const;
        double h() const;
        bool isFinite() const;

        BaseLayer() = delete;
        BaseLayer(const BaseLayerOptions&);

        virtual double interactionRate() const = 0;
        virtual void interact(Photon&, Random) const = 0;
    public:
        virtual ~BaseLayer();
};

// isotropic scattering with a reduced scattering coefficient rate (similarity relation)
class IsoLayer : public BaseLayer {
    friend class Layer;
    private:
        double red_mu_s_;

        double interactionRate() const;
        void interact(Photon&, Random) const;

        IsoLayer(const BaseLayerOptions&, double);
};

// HG phase-function scattering layer with anisotroy g and scattering rate mu_s
class HGLayer : public BaseLayer {
    friend class Layer;
    private:
        double mu_s_;
        double g_;

        double interactionRate() const;
        void interact(Photon&, Random) const;

        HGLayer(const BaseLayerOptions&, double, double);
};

// interface
class Layer {
    private:
        std::shared_ptr<BaseLayer> layer_;

        double g_;
        double scattering_rate_;

        void safeCall() const;
    public:
        Layer() = delete;
        Layer(const BaseLayerOptions&, double, double); // HGLayer
        Layer(const BaseLayerOptions&, double); // IsoLayer

        double interactionRate() const;
        void interact(Photon&, Random) const;
        double n() const;
        double h() const;
        double scattering_rate() const;
        double g() const;
        double mu_a() const;
        double dropFrac() const;
        bool isFinite() const;
};

std::ostream& operator<<(std::ostream&, const Layer&);

// TODO: define a free layer

#endif