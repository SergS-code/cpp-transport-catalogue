#pragma once


#include <ostream>
#include <iomanip>
#include <functional>
#include <algorithm>

using namespace std::string_literals;

namespace TransportCatalogy {
namespace detail {

namespace outPrint {


class StatReader {
public:
    struct Stats {
        Stats() = default;
        int stops{};
        int unique_stops{};
        double route_length{};
        double route_length2{};
        
    };

    StatReader(std::ostream& out): out_(out)
    {
    }

    void PrintBus(std::vector<std::string_view> &stop_name, std::string_view name, bool stop_exist);
    void PrintStats(std::string_view bus_name, const Stats& stats);

private:
    std::ostream& out_;
};
}
}
}
