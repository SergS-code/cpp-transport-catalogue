#include "transport_catalogue.h"
namespace TransportCatalogy {
std::deque<Bus> *TransportCatalogue::GetBuses()
{
   return &buses;

}

std::unordered_map<std::string_view, Stop *>* TransportCatalogue::get_stopname_to_stop()
{
    return  &stopname_to_stop;

}

std::unordered_map<std::string_view, Bus *>* TransportCatalogue::get_busname_to_bus()
{
    return &busname_to_bus;
}

  std::unordered_map<Stop *, std::set<std::string_view>> *TransportCatalogue::get_stop_wiht_bus()
{
      return &stop_wiht_bus;
}

std::unordered_map<const std::pair<Stop *, Stop *>, double, TransportCatalogueHasher> *TransportCatalogue::get_stop_distance()
{
    return &stop_distance;
}

void TransportCatalogue::AddStop()
{
    for (Stop& stop: stops)
     stopname_to_stop[stop.name]=&stop;


}

void TransportCatalogue::AddBus()
{
    for (Bus& temp_bus: buses)
     busname_to_bus[temp_bus.name]=&temp_bus;
    
}

Bus *TransportCatalogue::FindBus(std::string name)
{
    if(busname_to_bus.count(name)!=0){
        return busname_to_bus[name];
    }

    return nullptr;
}

void TransportCatalogue::GetBusInfo(std::string bus_name)
{
    if(busname_to_bus.count(bus_name) == 0) {
        stat_reader_.PrintStats(bus_name, detail::outPrint::StatReader::Stats{});
    }
    else {
        detail::outPrint::StatReader::Stats stats{};
        std::unordered_set<Stop * > unique_stops;

        stats.stops = static_cast<int>(busname_to_bus.at(bus_name)->busStop.size());

        unique_stops.insert(busname_to_bus.at(bus_name)->busStop.begin(), busname_to_bus.at(bus_name)->busStop.end());
        stats.unique_stops = static_cast<int>(unique_stops.size());
        double distance2=0;

        for(auto to = busname_to_bus.at(bus_name)->busStop.begin(), from = to++;  to != busname_to_bus.at(bus_name)->busStop.end(); ++to, ++from) {

             {
                detail::distant::Coordinates A,B;
                A.lat=(*from)->latitude;
                A.lng=(*from)->longitude;
                B.lat=(*to)->latitude;
                B.lng=(*to)->longitude;
                distance2+=GetDistance(*from,*to);
                auto distance = ComputeDistance(A,B);
                stats.route_length = distance2;
                stats.route_length2 += distance;


            }
        }
          stats.route_length2 = stats.route_length/stats.route_length2;
        stat_reader_.PrintStats(bus_name, stats);
    }
    
}

void TransportCatalogue::GetStopInfo(std::string_view name)
{
    std::vector<std::string_view> buss;
    bool stop_exist=false;
    if(stopname_to_stop.count(name)!=0){
        stop_exist=true;
        if(stop_wiht_bus.count(stopname_to_stop.at(name))!=0){

            for (auto& bus:stop_wiht_bus.at(stopname_to_stop.at(name))){
                buss.push_back(move(bus));
            }
        }
    }
    stat_reader_.PrintBus(buss,name,stop_exist);
}

void TransportCatalogue::SetDistance(Stop *A, Stop *B, double distant)
{
        get_stop_distance()->operator[]({A,B})=distant;

}

double TransportCatalogue::GetDistance(Stop *A, Stop *B)
{
    if(get_stop_distance()->count({A,B})!=0)
        return get_stop_distance()->at({A,B});
    else
        return get_stop_distance()->at({B,A});

    return 0.0;


}

Stop* TransportCatalogue::FindStop(std::string name)
{
    if(stopname_to_stop.count(name)!=0){
        return stopname_to_stop[name];
    }

    return nullptr;
}
}
