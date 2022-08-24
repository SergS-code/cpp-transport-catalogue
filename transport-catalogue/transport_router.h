#pragma once
#include <iostream>
#include <deque>
#include <unordered_map>
#include <vector>
#include "geo.h"
#include <unordered_set>
#include <set>
#include <iterator>
#include "domain.h"
#include "ranges.h"
#include "graph.h"
#include "router.h"
#include "transport_catalogue.h"
#include <memory>

namespace TransportsCatalogue {

using VertexId = size_t;
using EdgeId = size_t;

class TransportRouter{
public:
    TransportRouter(TransportCatalogue &db);
    void SetRouterSetting(RoutingSettings RouterSet_);
    void PrepareGraf();
     graph::DirectedWeightedGraph<double>& ReturnGraf();

     RoutingSettings& GetRoutingSettings();
     std::map<std::pair<size_t,size_t>,InfoEdge>& GetInfoEdges();
     std::map<size_t,std::string>& GetInfoStop();
     std::vector<graph::Edge<double>>& GetEdges();
     graph::DirectedWeightedGraph<double>& GetGraph();


    void PrepareEdges(std::vector<graph::Edge<double>>& Edges, VertexId from, VertexId to, double weight);
    void Rezult(size_t from, size_t to, double &total_time, std::vector<InfoToPrintRoute>& rez, graph::Router<double> &marshrut);
    void PrepareOneEdgeInfo(const std::string &bus_came, int span_count, double time, size_t from, size_t to);
    void PrepareStops();

private:
    RoutingSettings RouterSet;
    TransportCatalogue& Temp;
    std::map<std::pair<size_t,size_t>,InfoEdge> InfoEdges;
    std::map<size_t,std::string> InfoStop;

    std::vector<graph::Edge<double>> Edges;
    graph::DirectedWeightedGraph<double> Graph;

};

}
