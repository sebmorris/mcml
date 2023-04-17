#include "record.hpp"

using std::string;
using std::cout;

Recording::Recording(const string& filepath) : db_(filepath) {  }

simulation::Simulation Recording::proto(const Simulation& sim) {
    simulation::Simulation message;
    auto details = message.mutable_details();

    details->set_photonslaunched(sim.launchedPhotons());

    auto radialRange = details->mutable_radialextent();
    radialRange->set_bins(BINS);
    radialRange->set_max(SIM_EXTENT);
    radialRange->set_min(0);

    auto heightRange = details->mutable_heightextent();
    heightRange->set_bins(BINS);
    heightRange->set_max(SIM_EXTENT);
    heightRange->set_min(0);

    google::protobuf::util::TimeUtil time;
    auto currentTime = time.GetCurrentTime();

    auto timestamp = details->mutable_timestamp();
    timestamp->set_seconds(time.GetCurrentTime().seconds());
    timestamp->set_nanos(time.GetCurrentTime().nanos());

    const Material& mat = sim.getMaterial();
    for (auto layer : mat.layers_) {
        auto l = details->add_layers();
        l->set_g(layer.g());
        l->set_height(layer.h());
        l->set_n(layer.n());
        l->set_mu_s(layer.scattering_rate());
        l->set_mu_a(layer.mu_a());
    }

    auto absorption = sim.absorption();
    for (auto row : absorption) {
        for (auto e : row) {
            message.add_absorption(e);
        }
    }

    auto reflectance = sim.reflectance();
    for (auto e : reflectance) {
        message.add_reflectance(e);
    }

    return message;
}

simulation::ParameterSimulation Recording::proto(const ParameterSimulation& sims) {
    simulation::ParameterSimulation message;

    for (auto sim : sims.simulations_) {
        auto sim_ptr = message.add_simulations();
        *sim_ptr = proto(sim); 
    }

    for (auto layer : sims.layers_) {
        auto layer_ptr = message.add_layers();
        proto(layer_ptr, layer);
    }

    for (auto wl : wavelengths) {
        message.add_wavelengths(wl);
    }

    return message;
}

void Recording::proto(simulation::ParameterSimulation_Layer* const target, const ParameterSimulationLayerOptions& options) {
    target->set_height(options.height);
    target->set_n(options.n);
    target->set_g(options.g);

    target->set_a(options.a);
    target->set_b(options.b);

    if (options.constantLayer) {
        target->set_mu_a(options.mu_a);
        target->set_mu_s(options.mu_s);
    } else if (options.concentrationLayer) {
        target->set_cblood(options.cBlood);
        target->set_foxy(options.fOxy);
        target->set_flipid(options.fLipid);
        target->set_fwater(options.fWater);
    } else {
        throw std::invalid_argument("Options describes neither a constant nor concentration layer");
    }
}

void Recording::save(const ParameterSimulation& sims) {
    auto message = proto(sims);
    const string serializedMessage = message.SerializeAsString();

    db_.archive(serializedMessage);
}

void Recording::save(const Simulation& sim) {
    auto message = proto(sim);
    const string serializedMessage = message.SerializeAsString();

    db_.archive(serializedMessage);
}

void Recording::save(const std::string& serializedMessage) {
    if (serializedMessage == "") {
        throw std::runtime_error("Something went wrong serializing message");
    }
    
    db_.archive(serializedMessage);
}