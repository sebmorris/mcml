#include "result.hpp"

using std::string;
using std::to_string;
using std::vector;

string csvRowString(const vector<double>& row) {
    if (row.size() == 0) return "";

    // have to treat either the first or last element specially
    string result(to_string(row[0]));
    for (auto it = row.begin() + 1; it != row.end(); it++) {
        result += " ," + to_string(*it);
    }

    return result;
}