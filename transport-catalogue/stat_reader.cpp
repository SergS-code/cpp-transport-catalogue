#include "stat_reader.h"

namespace TransportsCatalogue {

namespace detail {

namespace outPrint {

StatReader::StatReader()
{

}

void StatReader::PrintStats(std::string_view bus_name, std::ostream& out_,TransportCatalogue &transport_catalogue) {

    Stats stats=transport_catalogue.GetBusInfo(std::string(bus_name));

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

void StatReader::PrintBus(std::string_view name, std::ostream &out_,TransportCatalogue &transport_catalogue) {

    InfoToPrintStop item=transport_catalogue.GetStopInfo(name);
    if(!item.stop_exist){
        out_ << "Stop "s << name << ": not found"s << std::endl;
        return;
    }
    if(item.buss.size()== 0) {
        out_ << "Stop "s << name << ": no buses"s << std::endl;
    }
    else {
        out_ << "Stop "s                << name << ":"s<<" buses"s;

        for(auto & bus: item.buss){
            out_<<" "<<bus;
        }
        out_<<std::endl;
    }
}
}
}
}
