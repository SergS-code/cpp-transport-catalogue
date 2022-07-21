#include <iostream>
#include <deque>
#include <unordered_map>
#include <vector>
#include "geo.h"
#include <unordered_set>
#include "json_reader.h"
#include "request_handler.h"
#include "domain.h"
#include <string>
#include "map_renderer.h"
#include "transport_router.h"
#include "router.h"
#include "graph.h"

using namespace std;

int main(){

    TransportsCatalogue::JsonReader GetInf;
    TransportsCatalogue::TransportCatalogue A;
    TransportsCatalogue::renderer::MapRenderer Map(A);
    Map.SetMapSetting(GetInf);
    GetInf.GetCatalog(A);
    GetInf.PrepareJson(std::cin);
    TransportsCatalogue::TransportRouter Router(A);
    Router.SetRouterSetting(GetInf);
    Router.PrepareGraf();
    TransportsCatalogue::RequestHandler Manager(Map,GetInf,A,Router);
    Manager.OutputResult();
    return 0;
}

