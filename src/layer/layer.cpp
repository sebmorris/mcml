#include "layer.hpp"

using std::cout;

BaseLayerOptions::BaseLayerOptions(double n, double mu_a) : n(n), mu_a(mu_a), infinite(true), height(inf) {  }
BaseLayerOptions::BaseLayerOptions(double n, double mu_a, double height) : n(n), mu_a(mu_a), infinite(false), height(height) {  }

BaseLayer::BaseLayer(const BaseLayerOptions& opts) : n_(opts.n), mu_a_(opts.mu_a), height_(opts.height), infinite_(opts.infinite) {  }
BaseLayer::~BaseLayer() {  }

void BaseLayer::drop(Photon& photon) const {
    photon.incrementWeight(-1 * photon.weight() * dropFrac());
}

double BaseLayer::dropFrac() const {
    return mu_a_ / interactionRate();
}

double BaseLayer::n() const { return n_; }
double BaseLayer::h() const { return height_; }
bool BaseLayer::isFinite() const { return !infinite_; }

/*
    IsoLayer definitions
*/
IsoLayer::IsoLayer(const BaseLayerOptions& baseOpts, double red_mu_s) : red_mu_s_(red_mu_s), BaseLayer(baseOpts) {  }

double IsoLayer::interactionRate() const {
    return mu_a_ + red_mu_s_;
}

void IsoLayer::interact(Photon& photon, double rand()) const {
    double cosT = 2*rand() - 1;
    double sinT = std::sqrt(1-cosT*cosT);

    double P = 2*pi*rand();
    double cosP = std::cos(P);
    double sinP = (P < pi ? 1 : -1)*std::sqrt(1-cosP*cosP);

    photon.setDirection(sinT*cosP, sinT*sinP, cosT);

    drop(photon);
}

/*
    HGLayer definitions
*/
HGLayer::HGLayer(const BaseLayerOptions& baseOpts, double mu_s, double g) : mu_s_(mu_s), g_(g), BaseLayer(baseOpts) {  }

double HGLayer::interactionRate() const {
    return mu_a_ + mu_s_;
}

void HGLayer::interact(Photon& photon, double rand()) const {
    drop(photon);

    
    // eq. 5.45, 5.46, 5.47a of:
    // Monte Carlo Modeling of Light Transport in Tissue (Steady State and Time of Flight) (Jacques 2011)

    // avoiding messy formulas
    double ux = photon.direction().x();
    double uy = photon.direction().y();
    double uz = photon.direction().z();

    // I chose g*g instead of square functions
    // apparently it's more efficient
    // https://stackoverflow.com/a/2940800
    double num_factor = (1 - g_*g_) / (1 - g_ + 2*g_*rand());
    double num = 1 + g_*g_ - num_factor*num_factor;

    // nb: sqrt faster than sin
    double cosTheta = num / (2*g_);

    double sinTheta = std::sqrt(1 - cosTheta*cosTheta);
    double Phi = 2*pi*rand();
    double cosPhi = std::cos(Phi);
    double sinPhi = std::sqrt(1 - cosPhi*cosPhi);
    if(Phi > pi) sinPhi *= -1;

    double temp = std::sqrt(1 - uz*uz);

    if (std::abs(uz) > 1 - 1e-12 /*cos(~0)*/) {
        double uxx = sinTheta*cosPhi;
        double uyy = sinTheta*sinPhi;
        double uzz = (uz > 0 ? 1 : -1)*cosTheta;

        photon.setDirection(uxx, uyy, uzz);
    } else {
        double uxx = sinTheta*(ux*uz*cosPhi - uy*sinPhi)/temp + ux*cosTheta;
        double uyy = sinTheta*(uy*uz*cosPhi + ux*sinPhi)/temp + uy*cosTheta;
        double uzz = -sinTheta*cosPhi*temp + uz*cosTheta;

        photon.setDirection(uxx, uyy, uzz);
    }
}


/*
    Layer definitions
*/
// need constructor to be called internally so make_shared not appropriate
Layer::Layer(const BaseLayerOptions& baseOpts, double red_mu_s) :
    layer_(std::shared_ptr<IsoLayer>(new IsoLayer(baseOpts, red_mu_s))),
    scattering_rate_(red_mu_s) {  }

Layer::Layer(const BaseLayerOptions& baseOpts, double mu_s, double g) :
    layer_(std::shared_ptr<HGLayer>(new HGLayer(baseOpts, mu_s, g))),
    scattering_rate_(mu_s),
    g_(g) {  }

void Layer::safeCall() const {
    if (!layer_)
        throw std::logic_error("layer shared ptr not defined");
}

// don't need to worry about safeCall since deleted default constructor
double Layer::interactionRate() const {
    return layer_->interactionRate();
}

void Layer::interact(Photon& photon, double rand()) const {
    return layer_->interact(photon, rand);
}

double Layer::n() const {
    return layer_->n_;
}

double Layer::h() const {
    return layer_->height_;
}

double Layer::dropFrac() const {
    return layer_->dropFrac();
}

bool Layer::isFinite() const {
    return layer_->isFinite();
}

double Layer::mu_a() const {
    return layer_->mu_a_;
}

double Layer::scattering_rate() const {
    return scattering_rate_;
}

double Layer::g() const {
    return g_;
}

std::ostream& operator<<(std::ostream& os, const Layer& layer) {
    os << "refractive index=" << layer.n() << ", height=" << layer.h();

    return os;
}