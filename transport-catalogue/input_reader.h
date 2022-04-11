#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <deque>
#include <tuple>
#include <map>
#include "transport_catalogue.h"
namespace TransportCatalogy {
namespace detail {

void AddComplitTraffic(std::vector<std::string> &bus_stop);
Bus FillingBusTrafic(std::string str,TransportCatalogue &A);
void FillingStopsWithBus(std::string str,TransportCatalogue &A);
Stop FillingBusStop (std::string str);
void Prepair(std::string& str);
void Request(std::istream& input, TransportCatalogue &&A, std::vector<std::pair<std::string, bool> > &bus_request);
void Prepair_zapros(std::istream &input,std::vector<std::pair<std::string,bool>>& bus_request);
void Prepair_Spravochnick( TransportCatalogue &&A,std::vector<std::string>&list_input_bus);
void Prepair_Distance(TransportCatalogue &A, std::vector<std::string>&list_input_bus);
void Parsing_distanc_substr(std::string& dist, double &Dist,std::string& name_to);
}
}
