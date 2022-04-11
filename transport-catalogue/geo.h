#pragma once

#include <cmath>

namespace TransportCatalogy {
namespace detail {

namespace distant {


struct Coordinates {
    double lat;
    double lng;
    bool operator==(const Coordinates& other) const {
        return lat == other.lat && lng == other.lng;
    }
    bool operator!=(const Coordinates& other) const {
        return !(*this == other);
    }
};

inline double ComputeDistance(Coordinates from, Coordinates to) {
    using namespace std;

    if (from == to) {
        return 0;
    }
    int RadiusZemli=6371000;
    static const double Radian = 3.1415926535 / 180.;
    return acos(sin(from.lat * Radian) * sin(to.lat * Radian) + cos(from.lat * Radian) * cos(to.lat * Radian) * cos(abs(from.lng - to.lng) * Radian))* RadiusZemli;
}
}
}
}
