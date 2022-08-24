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
#include "serialization.h"

using namespace std;



void PrintUsage(std::ostream& stream = std::cerr) {
    stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

int main(int argc, char* argv[]) {
 if (argc != 2) {
        PrintUsage();
        return 1;
    }

    const std::string_view mode(argv[1]);

    TransportsCatalogue::JsonReader GetInf;
    TransportsCatalogue::TransportCatalogue A;
    TransportsCatalogue::renderer::MapRenderer Map(A);

    Serializator Ser;


    if (mode == "make_base"sv) {
        Ser.SetSerilizating(true);
        Ser.GetCatalog(A);
        Ser.GetMapRenderer(Map);
        //Ser.GetRouter(Router);

        GetInf.GetSeriliz(Ser);
        GetInf.GetCatalog(A);
        GetInf.PrepareJson(std::cin);



    } else if (mode == "process_requests"sv) {
        TransportsCatalogue::TransportRouter Router(A);
        Ser.SetSerilizating(false);
        Ser.GetCatalog(A);
        Ser.GetMapRenderer(Map);
        Ser.GetRouter(Router);

        GetInf.GetSeriliz(Ser);
        GetInf.GetCatalog(A);
        GetInf.PrepareJson(std::cin);
        TransportsCatalogue::RequestHandler Manager(Map,GetInf,A,Router);
        Manager.OutputResult();

    } else {
        PrintUsage();
        return 1;
    }
    return 0;
}

