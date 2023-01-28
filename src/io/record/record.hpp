#ifndef RECORD_H
#define RECORD_H

#include <string>

#include <google/protobuf/util/time_util.h>
#include "../../simulation/simulation.hpp"
#include "../proto/simulation.pb.h"
#include "../database/database.hpp"

class Recording {
    private:
        Database db_;

    public:
        Recording() = delete;
        Recording(const std::string&);
        void saveSimulation(const Simulation&);
};

#endif