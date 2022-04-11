#pragma once
#include <iostream>
#include <deque>
#include <unordered_map>
#include <vector>
#include "geo.h"
#include "stat_reader.h"
#include <unordered_set>
#include <set>

namespace TransportCatalogy {


struct Stop{
  std::string name;
  double latitude;
  double longitude;
};


struct Bus{
  std::string name;
  std::vector<Stop*>busStop;
};


class TransportCatalogueHasher{
public:


    std::size_t operator()(const std::pair<const Stop*, const Stop*> pair_of_pointers) const noexcept
    {
        auto ptr1 = static_cast<const void*>(pair_of_pointers.first);
        auto ptr2 = static_cast<const void*>(pair_of_pointers.second);
        return hasher_(ptr1) * 37 + hasher_(ptr2);
    }

private:
    std::hash<const void*> hasher_;

};


class TransportCatalogue {

public:
    TransportCatalogue (detail::outPrint::StatReader& sr) : stat_reader_(sr)
    {
    }
    std::deque<Stop>* GetStops(){
        return &stops;
    };

    std::deque<Bus>* GetBuses();
    std::unordered_map<std::string_view,Stop*> *get_stopname_to_stop();
    std::unordered_map<std::string_view,Bus*> *get_busname_to_bus();
    std::unordered_map<Stop*,std::set<std::string_view>> *get_stop_wiht_bus();
    std::unordered_map<const std::pair< Stop*, Stop*>,double,TransportCatalogueHasher> *get_stop_distance();

    void AddStop();
    void AddBus();
    Stop* FindStop(std::string name);
    Bus* FindBus(std::string name);
    void GetBusInfo(std::string name);
    void GetStopInfo(std::string_view name);
    void SetDistance(Stop*A,Stop*B,double distant);
    double GetDistance(Stop*A,Stop*B);

private:
    std::deque<Stop> stops;
    std::deque<Bus> buses;
    std::unordered_map<Stop*,std::set<std::string_view>>stop_wiht_bus;
    std::unordered_map<std::string_view,Stop*> stopname_to_stop;
    std::unordered_map<std::string_view,Bus*> busname_to_bus;
    std::unordered_map<const std::pair< Stop*, Stop*>,double,TransportCatalogueHasher> stop_distance;

    detail::outPrint::StatReader& stat_reader_;
};
}
