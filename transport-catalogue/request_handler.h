#pragma once
#include <iostream>
#include "json.h"
#include "domain.h"
#include "transport_catalogue.h"
#include "svg.h"
#include "map_renderer.h"
#include "iostream"
#include <algorithm>
#include "json_reader.h"
#include <cstdlib>
#include <limits>
#include <iomanip>
#include <fstream>

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * В качестве источника для идей предлагаем взглянуть на нашу версию обработчика запросов.
 * Вы можете реализовать обработку запросов способом, который удобнее вам.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

// Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON reader-а
// с другими подсистемами приложения.
// См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)
namespace TransportsCatalogue {
class RequestHandler {
public:
    // MapRenderer понадобится в следующей части итогового проекта
    RequestHandler(TransportsCatalogue::TransportCatalogue &db);
    void SetMapSetting(JsonReader &MapSer_);
    void FillingPolyline(svg::Polyline &Marshrut, JsonReader *MapSet, int clolorPaletIndex, std::string &color);
    svg::Document GetTraec();
    void FillingText(svg::Document& doc, std::deque<Bus>& orderBus, const TransportsCatalogue::Plane::SphereProjector& proj);
    void FillingCircl(std::vector<Stop>&stops, svg::Document &doc, const Plane::SphereProjector &proj) const ;
    std::string GetColor(MapSetting settings);
    void  PrepareText(svg::Text &temp, bool zaliv,std::string busName,std::string color,const MapSetting& tempSetting);
    void  PrepareTextCoordinatsRing(svg::Text &temp, const Bus& bus, const Plane::SphereProjector &proj);
    bool  FindDublicate(Stop item, std::vector<Stop> &stops)const;
    void  PrepareTextCoordinatsNotRing(svg::Text &temp, const Bus& bus, const Plane::SphereProjector &proj, int lastStop, int stop_num);
    void  MainPrepareText(svg::Text &temp, svg::Document &doc, bool zalivka, bool ring,const Bus &bus, const Plane::SphereProjector &proj,const MapSetting& tempSetting);
    void  FillingTextStop(std::vector<Stop>&stops, svg::Document &doc, const Plane::SphereProjector &proj);
    void  PrepareTextStop(svg::Text &temp, bool zaliv,std::string busName,std::string color,const MapSetting& tempSetting,const Stop stop,const Plane::SphereProjector &proj);
    void  ScreeningMap(std::string& str);
    // Возвращает информацию о маршруте (запрос Bus)
   // std::optional<BusStat> GetBusStat(const std::string_view& bus_name) const;

    // Возвращает маршруты, проходящие через
   // const std::unordered_set<BusPtr>* GetBusesByStop(const std::string_view& stop_name) const;

    // Этот метод будет нужен в следующей части итогового проекта
  //  svg::Document RenderMap() const;

private:
    // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
     TransportCatalogue& Temp;
     JsonReader *MapSet;
     std::map<std::string,std::string> busWithColor;
   // const renderer::MapRenderer& renderer_;
};
}
