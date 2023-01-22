#include "result.hpp"

using std::vector;
using std::cout;

RadialTracker::RadialTracker(index_type noBins, double rMax) : NUM_BINS(noBins), RADIAL_MAX(rMax), BIN_SIZE(rMax/noBins) {
    rawRadial.resize(NUM_BINS, 0.0);
}

RadialTracker::index_type RadialTracker::index(double r) const {
    return std::floor(r / BIN_SIZE);
}

RadialTracker::index_type RadialTracker::index(const CartVec& position) const {
    return index(position.r());
}

void RadialTracker::rawDrop(double amount, index_type i) {
    if (i >= NUM_BINS) {
        rawOverflow += amount;
        return;
    }

    rawRadial[i] += amount;
}

void RadialTracker::rawDrop(double amount, double r) {
    rawDrop(amount, index(r));
}

double RadialTracker::overflow() const {
    return rawOverflow;
}

const RadialTracker::row& RadialTracker::rawData() const {
    return rawRadial;
}

RadialTracker::row RadialTracker::normData(unsigned int N) const {
    row copy = rawRadial;

    for (index_type i = 0; i != copy.size(); i++) {
        double areaElement = 2.0 * pi * (i + 0.5) * BIN_SIZE * BIN_SIZE;
        copy[i] /= areaElement * N;
    }

    return copy;
}

BulkTracker::BulkTracker(index_type noHeightBins, index_type noRadialBins, double hMax, double rMax) :
NUM_HEIGHT_BINS(noHeightBins), NUM_RADIAL_BINS(noRadialBins), HEIGHT_MAX(hMax), RADIAL_MAX(rMax),
HEIGHT_BIN_SIZE(hMax/noHeightBins), RADIAL_BIN_SIZE(rMax/noRadialBins) {
    rawBulk.resize(NUM_HEIGHT_BINS, RadialTracker(NUM_RADIAL_BINS, RADIAL_MAX));
};

BulkTracker::index_type BulkTracker::heightIndex(double z) const {
    return std::abs(z) / HEIGHT_BIN_SIZE;
}

BulkTracker::index_type BulkTracker::heightIndex(const CartVec& position) const {
    return heightIndex(position.z());
}

void BulkTracker::rawDrop(double amount, index_type hi, index_type ri) {
    if (hi >= NUM_HEIGHT_BINS) {
        overflow += amount;
        return;
    }

    rawBulk[hi].rawDrop(amount, ri);
}

void BulkTracker::rawDrop(double amount, const CartVec& position) {
    index_type hi = heightIndex(position);

    if (hi >= NUM_HEIGHT_BINS) {
        overflow += amount;
        return;
    }

    rawBulk[hi].rawDrop(amount, position.r());
}

BulkTracker::grid BulkTracker::rawData() const {
    grid result;

    for (RadialTracker i : rawBulk) {
        RadialTracker::row normRow = i.rawData();
        result.push_back(normRow);
    }

    return result;
}

BulkTracker::grid BulkTracker::normData(unsigned int N) const {
    grid result;

    for (RadialTracker i : rawBulk) {
        RadialTracker::row normRow = i.normData(N);
        std::transform(normRow.begin(), normRow.end(), normRow.begin(),
            [this](double e) { return e / HEIGHT_BIN_SIZE; });
        result.push_back(normRow);
    }

    return result;
}