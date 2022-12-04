#ifndef RESULT_H
#define RESULT_H

#include <vector>

#include "../cartvec/cartvec.hpp"

struct RadialTracker {
    typedef std::vector<double>::size_type index_type;
    private:
        // logically const values
        index_type NUM_BINS;
        double RADIAL_MAX;
        double BIN_SIZE;

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
        // logically const values
        index_type NUM_RADIAL_BINS;
        index_type NUM_HEIGHT_BINS;
        double HEIGHT_MAX;
        double RADIAL_MAX;
        double HEIGHT_BIN_SIZE;
        double RADIAL_BIN_SIZE;

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