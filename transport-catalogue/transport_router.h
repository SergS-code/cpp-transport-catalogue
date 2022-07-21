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
#include "json_reader.h"
#include "transport_catalogue.h"
#include <memory>

namespace TransportsCatalogue {

using VertexId = size_t;
using EdgeId = size_t;

class TransportRouter{
public:
    TransportRouter(TransportCatalogue &db);
    void SetRouterSetting(JsonReader &RouterSet_);
    void PrepareGraf();
     graph::DirectedWeightedGraph<double>& ReturnGraf();

    void PrepareEdges(std::vector<graph::Edge<double>>& Edges,VertexId from,VertexId to,double weight);
    void Rezult(size_t from, size_t to, double &totalTime,std::vector<InfoToPrintRoute>& rez, graph::Router<double> &marshrut);
    void PrepareOneEdgeInfo(const std::string &busName, int spaunCount, double time, size_t from, size_t to);
    void PrepareStops();

private:
    JsonReader *RouterSet;
    TransportCatalogue& Temp;
    std::map<std::pair<size_t,size_t>,InfoEdge> InfoEdges;
    std::map<size_t,std::string> InfoStop;

    std::vector<graph::Edge<double>> Edges;
    graph::DirectedWeightedGraph<double> Graff;

};

}
