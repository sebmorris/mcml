#include "result.hpp"

using std::string;
using std::to_string;
using std::vector;
using std::ostream;

ostream& csvRowString(ostream& os, const RadialTracker::row& row) {
    if (row.size() == 0) return os;

    // have to treat either the first or last element specially
    os << row[0];
    for (auto it = row.begin() + 1; it != row.end(); it++) {
        os << ", " << *it;
    }

    return os;
}

ostream& csvGridString(ostream& os, const BulkTracker::grid& grid) {
    for (auto i : grid) {
        csvRowString(os, i) << "\n";
    }

    return os;
}