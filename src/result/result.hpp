#ifndef RESULT_H
#define RESULT_H

#include "../cartvec/cartvec.hpp"

const int REFLECTANCE_BINS = 50;
const int HEIGHT_BINS = 100;
const int RADIAL_BINS = 100;

const double RADIAL_MAX = 50;

const double HEIGHT_MIN = -50;

struct Result {
    private:
        const double REFLECTANCE_BIN_SIZE = RADIAL_MAX / REFLECTANCE_BINS;
        const double HEIGHT_BIN_SIZE = -HEIGHT_MIN / RADIAL_BINS;
        const double RADIAL_BIN_SIZE = RADIAL_MAX / RADIAL_BINS;

        const int radialIndex(CartVec& position);
        int radialIndex(double x, double y);
        
        const int heightIndex(CartVec& position);
        int heightIndex(double z);

        const int reflectanceIndex(CartVec& position);
        int reflectanceIndex(double x, double y);

        double r(double x, double y);
    
    public:
        double reflectance[REFLECTANCE_BINS];
        double reflectanceOverflow;
        double absorption[RADIAL_BINS][HEIGHT_BINS];
        double absorptionOverflow;

        const void drop(CartVec& position, double amount);
        const void escape(CartVec& position, double amount);
        void save(const int launchedPhotons);
        void save();

        Result();
};


#endif