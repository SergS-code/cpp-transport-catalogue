#include "stat_reader.h"

namespace TransportCatalogy {

namespace detail {

namespace outPrint {


void StatReader::PrintStats(std::string_view bus_name, const Stats &stats) {
    if(stats.stops == 0) {
        out_ << "Bus "s << bus_name << ": not found"s << std::endl;
    }
    else {
        out_ << "Bus "s                << bus_name << ": "s
             << stats.stops            << " stops on route, "s
             << stats.unique_stops     << " unique stops, "s
             << std::setprecision(6)   << stats.route_length << " route length, "s
             << std::setprecision(6)   << stats.route_length2 << " curvature"s
             << std::endl;
    }
}
void StatReader::PrintBus(std::vector<std::string_view>& stop_name,std::string_view name,bool stop_exist) {

    if(!stop_exist){
        out_ << "Stop "s << name << ": not found"s << std::endl;
        return;
    }
    if(stop_name.size()== 0) {
        out_ << "Stop "s << name << ": no buses"s << std::endl;
    }
    else {
        out_ << "Stop "s                << name << ":"s<<" buses"s;

        for(auto & bus: stop_name){
            out_<<" "<<bus;
        }
        out_<<std::endl;

    }
}
}
}
}
