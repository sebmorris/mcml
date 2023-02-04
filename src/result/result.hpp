#ifndef RESULT_H
#define RESULT_H

#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>

#include "../cartvec/cartvec.hpp"
#include "../constants/constants.hpp"

struct RadialTracker {
    public:
        typedef std::vector<double> row;
        typedef row::size_type index_type;
    private:
        // logically const values
        index_type NUM_BINS;
        double RADIAL_MAX;
        double BIN_SIZE;

        double rawOverflow;
        row rawRadial;

        index_type index(double r) const;
        index_type index(const CartVec&) const;
    public:
        void rawDrop(double amount, index_type index);
        void rawDrop(double amount, double r);
        double overflow() const;
        const row& rawData() const;
        row normData(unsigned int) const;

        RadialTracker() = delete;
        RadialTracker(index_type, double);
};

std::string csvRowString(const RadialTracker::row&);
std::ostream& csvRowString(std::ostream&, const RadialTracker::row&);

struct BulkTracker {
    typedef RadialTracker::index_type index_type;
    typedef std::vector<RadialTracker::row> grid;
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
        grid rawData() const;
        grid normData(unsigned int) const;

        BulkTracker() = delete;
        BulkTracker(index_type, index_type, double, double);
};

std::ostream& csvGridString(std::ostream&, const BulkTracker::grid&);

#endif