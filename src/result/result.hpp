#ifndef RESULT_H
#define RESULT_H

#include <vector>

#include "../cartvec/cartvec.hpp"

struct RadialTracker {
    typedef std::vector<double>::size_type index_type;
    private:
        const index_type NUM_BINS;
        const double RADIAL_MAX;
        const double BIN_SIZE;

        double rawOverflow;
        std::vector<double> rawRadial;

        index_type index(double r) const;
        index_type index(const CartVec&) const;
    public:
        void rawDrop(double amount, index_type index);
        void rawDrop(double amount, double r);
        double overflow() const;

        RadialTracker() = delete;
        RadialTracker(index_type, double);
};

const std::vector<double>& normReflectance(const RadialTracker&, int);

struct BulkTracker {
    typedef RadialTracker::index_type index_type;
    private:
        const index_type NUM_RADIAL_BINS;
        const index_type NUM_HEIGHT_BINS;
        const double HEIGHT_MAX;
        const double RADIAL_MAX;
        const double HEIGHT_BIN_SIZE;
        const double RADIAL_BIN_SIZE;

        double overflow;
        std::vector<RadialTracker> rawBulk;

        index_type heightIndex(double z) const;
        index_type heightIndex(const CartVec& position) const;
    public:
        void rawDrop(double amount, const CartVec& position);
        void rawDrop(double amount, index_type, index_type);

        BulkTracker() = delete;
        BulkTracker(index_type, index_type, double, double);
};

const std::vector<std::vector<double> >& normAbsorption(const BulkTracker&, int);

#endif