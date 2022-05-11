#pragma once


#include <ostream>
#include <iomanip>
#include <functional>
#include <algorithm>
#include "transport_catalogue.h"

using namespace std::string_literals;

namespace TransportsCatalogue {
namespace detail {
namespace outPrint {

class StatReader {
public:
    StatReader();
    void PrintBus(std::string_view name, std::ostream &out_,TransportCatalogue &transport_catalogue);
    void PrintStats(std::string_view bus_name, std::ostream& out_,TransportCatalogue &transport_catalogue);
};
}
}
}
