#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <deque>
#include <tuple>
#include <map>
#include "transport_catalogue.h"
namespace TransportsCatalogue {
namespace detail {

    void AddComplitTraffic(std::vector<std::string> &bus_stop);
    Bus ParseBus(std::string str,TransportCatalogue &transport_catalogue);
    void ParseStopsWithBus(std::string str,TransportCatalogue &transport_catalogue);
    Stop ParseBusStop (std::string str);
    void RemovingSpaces(std::string& str);
    Requests Request(std::istream& input, TransportCatalogue &&transport_catalogue);
    void PrepareRequests(std::istream &input, Requests& request);
    void PrepareCatalogue(TransportCatalogue &&transport_catalogue,std::vector<std::string>&list_input_bus);
    void PrepareDistance(TransportCatalogue &transport_catalogue, std::vector<std::string>&list_input_bus);
    void Parsing_distanc_substr(std::string& dist, double &Dist,std::string& name_to);
}
}
