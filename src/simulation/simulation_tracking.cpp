#include "simulation.hpp"

AbsorptionEvent::AbsorptionEvent(double amount, const CartVec& position) : amount_(amount), position_(position) {  };

TrackedDistance::TrackedDistance(const BulkTracker& tracker, double distance, double tolerance) :
absorption_(tracker), distance_(distance), tolerance_(tolerance) {  };

bool operator==(const TrackedDistance& tracker, double distance) {
    return std::abs(tracker.distance_ - distance) < tracker.tolerance_;
}

bool operator==(double distance, const TrackedDistance& tracker) {
    return tracker == distance;
}
