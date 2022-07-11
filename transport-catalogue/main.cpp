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

using namespace std;

int main(){

    TransportsCatalogue::JsonReader GetInf;
    TransportsCatalogue::TransportCatalogue A;
    TransportsCatalogue::renderer::MapRenderer Map(A);
    Map.SetMapSetting(GetInf);
    GetInf.GetCatalog(A);
    GetInf.PrepairJson(std::cin);
    TransportsCatalogue::RequestHandler Manager(Map,GetInf,A);
    Manager.OutputResult();
    return 0;
}

