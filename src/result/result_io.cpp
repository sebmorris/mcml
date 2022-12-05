#include "result.hpp"

using std::string;
using std::to_string;
using std::vector;
using std::ostream;

string csvRowString(const RadialTracker::row& row) {
    if (row.size() == 0) return "";

    // have to treat either the first or last element specially
    string result(to_string(row[0]));
    for (auto it = row.begin() + 1; it != row.end(); it++) {
        result += ", " + to_string(*it);
    }

    return result;
}

ostream& csvRowString(ostream& os, const RadialTracker::row& row) {
    os << csvRowString(row);

    return os;
}

ostream& csvGridString(ostream& os, const BulkTracker::grid& grid) {
    for (auto i : grid) {
        os << csvRowString(i) << "\n";
    }

    return os;
}