#include <iostream>
#include "input_reader.h"
#include "transport_catalogue.h"
#include "stat_reader.h"
#include <vector>
#include <string>


using namespace TransportsCatalogue;
int main(){
    detail::outPrint::StatReader Print;
    Requests bus_request;
    TransportCatalogue transport_catalogue;
    bus_request=detail::Request(std::cin,std::move(transport_catalogue));

    for(auto & str : bus_request.requests ){
        if(str.second)
             Print.PrintStats(str.first,std::cout,transport_catalogue);
          else
             Print.PrintBus(str.first,std::cout,transport_catalogue);
    }
    return 0;
}
