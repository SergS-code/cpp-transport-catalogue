#pragma once
#include <iostream>
#include "json.h"
#include "domain.h"
#include "transport_catalogue.h"
#include <array>
#include <list>

namespace TransportsCatalogue {
class JsonReader{
public:
    JsonReader(){
    }
    void PrepareJson(std::istream &streamDate);
    void GetCatalog(TransportCatalogue& primary);
    void GetInfoBus(const json::Node &Businf);
    void GetInfoStop(const json::Node &Stop);
    void GetReqInf(const json::Node &ReqItem);
    void GetMapInfo(const json::Node &MapItem);
    void FillCatalogy();
    MapSetting &GetSetting();

    void PrintInfoStop(InfoToPrintStop item,std::string request_id);
    void PrintInfoBus(Stats item,std::string request_id);

    std::vector<StatRequest> GetReqInf();

private:
    std::vector<StatRequest> ReqInf;
    json::Document Doc;
    MapSetting Mapset;
    TransportCatalogue* Temp;

};

}
