#include "record.hpp"

using std::string;
using std::cout;

Recording::Recording(const string& filepath) : db_(filepath) {  }

void Recording::saveSimulation(const Simulation& sim) {
    simulation::Simulation message;
    auto details = message.mutable_details();

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

    const string serializedMessage = message.SerializeAsString();

    if (serializedMessage == "") {
        throw std::runtime_error("Something went wrong serializing message");
    }
    
    db_.archive(serializedMessage);
}