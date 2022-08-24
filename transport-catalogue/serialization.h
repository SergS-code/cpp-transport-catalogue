#pragma once

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include <memory>
#include <transport_catalogue.pb.h>
#include <filesystem>
#include <fstream>


#include "map_renderer.h"
#include "transport_catalogue.h"
#include "transport_router.h"



class Serializator{
public:
    void FillingCatalogToSerStop(catalog::Stop &stop);
    void FillingCatalogToSerBus(catalog::Bus &bus);
    catalog::TransportCatalogue* GettempSeriliz();
    void GetCatalog(TransportsCatalogue::TransportCatalogue& primary);
    void GetMapRenderer(TransportsCatalogue::renderer::MapRenderer& primary);
    void GetRouter(TransportsCatalogue::TransportRouter& primary);

    void Serialize();
    void SerializeMapSetting();
    void SetPath( std::filesystem::path path);
    void SetSerilizating(bool val);
    bool GetSerilizating();
    catalog::RenderSettings* GetMapSetting();
    void DeserializeMapRender();
    void DeserializePerson();
    catalog::RenderSettings tempSerMapRender_;
    catalog::Router tempSerRouter_;
    catalog::TransportCatalogue tempSeriliz_;

private:

    TransportsCatalogue::TransportRouter* TempRouter;
    TransportsCatalogue::renderer::MapRenderer* TempRender;
    TransportsCatalogue::TransportCatalogue* Temp;
    catalog::RouteSettings routeSettings;
    std::filesystem::path path_;
    bool Serilizating;
};
