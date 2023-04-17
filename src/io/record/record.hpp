#ifndef RECORD_H
#define RECORD_H

#include <string>

#include <google/protobuf/util/time_util.h>
#include "../../simulation/simulation.hpp"
#include "../../parameter_simulation/parameter_simulation.hpp"
#include "../proto/simulation.pb.h"
#include "../database/database.hpp"
#include "../../sample/sample.hpp"

class Recording {
    private:
        Database db_;

        simulation::Simulation proto(const Simulation&);
        simulation::ParameterSimulation proto(const ParameterSimulation&);
        void proto(simulation::ParameterSimulation_Layer* const, const ParameterSimulationLayerOptions&);

    public:
        Recording() = delete;
        Recording(const std::string&);
        
        void save(const Simulation&);
        void save(const ParameterSimulation&);
        void save(const std::string&);
};

#endif