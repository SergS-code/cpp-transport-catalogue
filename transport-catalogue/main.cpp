#include <iostream>
#include "input_reader.h"
#include "transport_catalogue.h"
#include <vector>
#include <string>


using namespace TransportCatalogy;
int main(){

    std::vector<std::pair<std::string,bool>> bus_request;

    detail::outPrint::StatReader stats(std::cout);
    TransportCatalogue A(stats);
    detail::Request(std::cin,std::move(A),bus_request);
    for(auto & str : bus_request ){
        if(str.second)
             A.GetBusInfo(str.first);
          else
             A.GetStopInfo(str.first);

    }
    return 0;
}
