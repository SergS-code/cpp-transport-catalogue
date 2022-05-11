#pragma once
#include <iostream>
#include <deque>
#include <unordered_map>
#include <vector>
#include "geo.h"
#include <unordered_set>
#include <set>

namespace TransportsCatalogue {

struct Stop{
    std::string name;
    double latitude;
    double longitude;
};

struct InfoToPrintStop{
    std::vector<std::string_view> buss;
    bool stop_exist=false;
};

struct StopToStop{
    StopToStop(Stop*A,Stop*B)
    {
        AtoB.first=A;
        AtoB.second=B;
    }
    bool operator==(StopToStop A)const{
        return AtoB==A.AtoB;
    }
    bool operator!=(StopToStop A)const{
        return AtoB!=A.AtoB;
    }
    std::pair<Stop*,Stop*>AtoB;

};

struct Stats {
    Stats() = default;
    int stops{};
    int unique_stops{};
    double route_length{};
    double route_length2{};

};
struct Requests{
    std::vector<std::pair<std::string,bool>> requests;
};


struct Bus{
    std::string name;
    std::vector<Stop*>busStop;
};


class TransportCatalogueHasher{
public:
    std::size_t operator()(const StopToStop pair_of_pointers) const noexcept;
private:
    std::hash<const void*> hasher_;
};

class TransportCatalogue {

public:
    TransportCatalogue ();
    std::deque<Stop>* GetStops();
    std::deque<Bus>* GetBuses();
    std::unordered_map<std::string_view,Stop*> *get_stopname_to_stop();
    std::unordered_map<std::string_view,Bus*> *get_busname_to_bus();
    std::unordered_map<Stop*,std::set<std::string_view>> *get_stop_wiht_bus();
    std::unordered_map<const StopToStop,double,TransportCatalogueHasher> *get_stop_distance();

    void PrepareStops();
    void PrepareBus();
    void AddStop(Stop &stop);
    void AddBus(Bus &bus);
    Stop* FindStop(std::string name);
    Bus* FindBus(std::string name);
    Stats GetBusInfo(std::string name);
    InfoToPrintStop GetStopInfo(std::string_view name);
    void SetDistance(Stop*A,Stop*B,double distant);
    double GetDistance(Stop*A,Stop*B);

private:
    std::deque<Stop> stops;
    std::deque<Bus> buses;
    std::unordered_map<Stop*,std::set<std::string_view>>stop_wiht_bus;
    std::unordered_map<std::string_view,Stop*> stopname_to_stop;
    std::unordered_map<std::string_view,Bus*> busname_to_bus;
    std::unordered_map<const StopToStop,double,TransportCatalogueHasher> stop_distance;

};
}
