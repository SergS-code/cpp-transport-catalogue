#include "json_reader.h"
#include <string>

/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */
namespace TransportsCatalogue{

void JsonReader::PrepareJson(std::istream &streamDate)
{
    Doc=json::Load(streamDate);
    FillCatalogy();
}

void JsonReader::GetCatalog(TransportCatalogue &primary)
{
    Temp=&primary;
}

void JsonReader::GetSeriliz(Serializator &primary)
{
    Seriliz=&primary;
}

void JsonReader::GetInfoBus(const json::Node &Businf)
{
    catalog::Bus binBus;

    TransportsCatalogue::Bus bus;
    std::list<std::string> revers;
    json::Dict temp=Businf.AsMap();
    bool round_trip=temp.at("is_roundtrip").AsBool();

    binBus.set_is_round_trip(temp.at("is_roundtrip").AsBool());

    bus.name=temp.at("name").AsString();

    binBus.set_name(temp.at("name").AsString());

    for(auto& i: temp.at("stops").AsArray()){   // опасно, нет проверки что такая останвока заводилась в бд
        std:: string t= i.AsString();
        binBus.mutable_stops()->Add(std::move(t));
    }

    for(auto& i: temp.at("stops").AsArray()){
        std:: string t= i.AsString();
        revers.push_front(t);
        bus.bus_stop.push_back(Temp->FindStop(i.AsString()));
    }

    int i=0;
    if (!round_trip){
        for(auto& it:revers){
            if(i>0){
                std:: string t= it;
                binBus.mutable_stops()->Add(std::move(t));

                bus.bus_stop.push_back(Temp->FindStop(it));
            }
            i++;
        }

    }

    bus.is_roundtrip=temp.at("is_roundtrip").AsBool();

    Seriliz->FillingCatalogToSerBus(binBus);

    Temp->GetBuses()->push_back(bus);

}

void JsonReader::GetInfoStop(const json::Node &StopInf)
{
    catalog::Stop binStop;

    TransportsCatalogue::Stop stop;
    json::Dict temp=StopInf.AsMap();

    binStop.set_name(temp.at("name").AsString());
    binStop.set_lat(temp.at("latitude").AsDouble());
    binStop.set_long_(temp.at("longitude").AsDouble());

    stop.name=temp.at("name").AsString();
    stop.latitude=temp.at("latitude").AsDouble();
    stop.longitude=temp.at("longitude").AsDouble();

    for(auto &item: temp.at("road_distances").AsMap()){
        catalog::RoadDistances binRoad;
        binRoad.set_stop_name(item.first);
        binRoad.set_distant(item.second.AsDouble());
        binStop.mutable_distant()->Add(std::move(binRoad));

        stop.road_distances[item.first]=item.second.AsDouble();
    }
    Seriliz->FillingCatalogToSerStop(binStop);
    Temp->GetStops()->push_back(stop);

}

void JsonReader::GetReqInf(const json::Node &ReqItem)
{
    json::Dict temp=ReqItem.AsMap();
    StatRequest item;
    item.id=temp.at("id").AsInt();
    item.type=temp.at("type").AsString();
    if(temp.count("name")!=0)
        item.name=temp.at("name").AsString();

    if(temp.at("type").AsString()=="Route"){
        item.from=temp.at("from").AsString();
        item.to=temp.at("to").AsString();
    }
    ReqInf.push_back(item);
}
void JsonReader::GetMapInfo(const json::Node &MapItem)
{
    //catalog::RenderSettings binSetting=Seriliz->GetMapSetting();
    catalog::RenderSettings binSetting;

    json::Dict temp=MapItem.AsMap();
    Mapset.width=temp.at("width").AsDouble();
    Mapset.height=temp.at("height").AsDouble();
    Mapset.padding=temp.at("padding").AsDouble();
    Mapset.stop_radius=temp.at("stop_radius").AsDouble();
    Mapset.line_width=temp.at("line_width").AsDouble();
    Mapset.bus_label_font_size=temp.at("bus_label_font_size").AsInt();
    Mapset.stop_label_font_size=temp.at("stop_label_font_size").AsInt();

    binSetting.set_width(temp.at("width").AsDouble());
    binSetting.set_height(temp.at("height").AsDouble());
    binSetting.set_padding(temp.at("padding").AsDouble());
    binSetting.set_stop_radius(temp.at("stop_radius").AsDouble());
    binSetting.set_line_width(temp.at("line_width").AsDouble());
    binSetting.set_bus_label_font_size(temp.at("bus_label_font_size").AsInt());
    binSetting.set_stop_label_font_size(temp.at("stop_label_font_size").AsInt());
    {
        int i=0;
        for (auto& it:temp.at("bus_label_offset").AsArray()){
            Mapset.bus_label_offset[i]=it.AsDouble();
            ++i;
        }
        i=0;
        for (auto& it:temp.at("stop_label_offset").AsArray()){
            Mapset.stop_label_offset[i]=it.AsDouble();
            ++i;
        }
    }

    binSetting.mutable_bus_label_offset()->set_x(temp.at("bus_label_offset").AsArray()[0].AsDouble());
    binSetting.mutable_bus_label_offset()->set_y(temp.at("bus_label_offset").AsArray()[1].AsDouble());


    binSetting.mutable_stop_label_offset()->set_x(temp.at("stop_label_offset").AsArray()[0].AsDouble());
    binSetting.mutable_stop_label_offset()->set_y(temp.at("stop_label_offset").AsArray()[1].AsDouble());
    {
        if(temp.at("underlayer_color").IsArray()){
            if(temp.at("underlayer_color").AsArray().size()==4){
                std::array<double,4>te;
                int i=0;
                for (auto& it:temp.at("underlayer_color").AsArray()){
                    te[i]=it.AsDouble();
                    ++i;
                }
                Mapset.underlayer_color=te;

                binSetting.mutable_underlayer_color()->mutable_rgba_color()->set_r(temp.at("underlayer_color").AsArray()[0].AsDouble());
                binSetting.mutable_underlayer_color()->mutable_rgba_color()->set_g(temp.at("underlayer_color").AsArray()[1].AsDouble());
                binSetting.mutable_underlayer_color()->mutable_rgba_color()->set_b(temp.at("underlayer_color").AsArray()[2].AsDouble());
                binSetting.mutable_underlayer_color()->mutable_rgba_color()->set_o(temp.at("underlayer_color").AsArray()[3].AsDouble());

            }
            if(temp.at("underlayer_color").AsArray().size()==3){
                std::array<int,3>te;
                int i=0;
                for (auto& it:temp.at("underlayer_color").AsArray()){
                    te[i]=it.AsInt();
                    ++i;
                }
                Mapset.underlayer_color=te;

                binSetting.mutable_underlayer_color()->mutable_rgb_color()->set_r(temp.at("underlayer_color").AsArray()[0].AsInt());
                binSetting.mutable_underlayer_color()->mutable_rgb_color()->set_g(temp.at("underlayer_color").AsArray()[1].AsInt());
                binSetting.mutable_underlayer_color()->mutable_rgb_color()->set_b(temp.at("underlayer_color").AsArray()[2].AsInt());


            }
        }

        if(temp.at("underlayer_color").IsString()){
            Mapset.underlayer_color=temp.at("underlayer_color").AsString();
             binSetting.mutable_underlayer_color()->set_string_color(temp.at("underlayer_color").AsString());
        }
    }

    Mapset.underlayer_width=temp.at("underlayer_width").AsDouble();
    binSetting.set_underlayer_width(temp.at("underlayer_width").AsDouble());


    for(auto& it: temp.at("color_palette").AsArray()){
        if(it.IsString()){
            Mapset.color_palette.push_back(it.AsString());
            catalog::Color temp;
            temp.set_string_color(it.AsString());
            binSetting.mutable_color_palette()->Add(std::move(temp));
        }
        if(it.IsArray()){
            if(it.AsArray().size()==3){
                std::array<int,3>to;
                int i=0;
                for(auto& item:it.AsArray()){
                    to[i]=item.AsInt();
                    ++i;
                }
                Mapset.color_palette.push_back(to);
                catalog::Color temp;
                temp.mutable_rgb_color()->set_r(it.AsArray()[0].AsInt());
                temp.mutable_rgb_color()->set_g(it.AsArray()[1].AsInt());
                temp.mutable_rgb_color()->set_b(it.AsArray()[2].AsInt());
                binSetting.mutable_color_palette()->Add(std::move(temp));
            }
            if(it.AsArray().size()==4){
                std::array<double,4>to;
                int i=0;
                for(auto& item:it.AsArray()){
                    to[i]=item.AsDouble();
                    ++i;
                }
                Mapset.color_palette.push_back(to);

                catalog::Color temp;
                temp.mutable_rgba_color()->set_r(it.AsArray()[0].AsDouble());
                temp.mutable_rgba_color()->set_g(it.AsArray()[1].AsDouble());
                temp.mutable_rgba_color()->set_b(it.AsArray()[2].AsDouble());
                temp.mutable_rgba_color()->set_o(it.AsArray()[3].AsDouble());
                binSetting.mutable_color_palette()->Add(std::move(temp));

            }
        }

    }
    Seriliz->tempSerMapRender_=binSetting;
}

void JsonReader::GetRouterInfo(const json::Node &RouterItem)
{
    json::Dict temp=RouterItem.AsMap();
    Routerset.bus_velocity=temp.at("bus_velocity").AsDouble();
    Routerset.bus_wait_time=temp.at("bus_wait_time").AsDouble();
}

void JsonReader::FillCatalogy()
{
   // json::Node::Value temp= Doc.GetRoot().GetValue();

    json::Dict inf=Doc.GetRoot().AsMap();
    std::vector<json::Node>requestBus;
    for(auto &a : inf){
        if(a.first=="serialization_settings")
        {
            if(a.second.IsDict())
                for(auto &b : a.second.AsMap()){
                    if(b.first=="file"){
                        Seriliz->SetPath(b.second.AsString());
                    }
                }
        }
        if(a.first=="base_requests")
        {
            if(a.second.IsArray())
                for(auto &b : a.second.AsArray()){
                    if(b.IsMap()){
                        for(auto &infoReq:b.AsMap()){
                            if(infoReq.first=="type" && infoReq.second.AsString()=="Stop"){
                                GetInfoStop(b);
                            }
                            if(infoReq.first=="type" && infoReq.second.AsString()=="Bus"){
                                requestBus.push_back(b);
                            }
                        }
                    }
                }
        }
        if(a.first=="stat_requests"){
            if(a.second.IsArray()){
                for(auto &b : a.second.AsArray()){
                    if(b.IsMap()){
                        GetReqInf(b);
                    }
                }
            }

        }
        if(a.first=="render_settings"){
            if(a.second.IsMap()){
                GetMapInfo(a.second);
            }
        }
        if(a.first=="routing_settings"){
            if(a.second.IsMap()){
                GetRouterInfo(a.second);
            }
        }
    }
    Temp->PrepareStops();
    for (auto &item :requestBus ){
        GetInfoBus(item);
    }

    Temp->PrepareBus();
    Temp->PrepareDistance();
    Temp->PrepareStopsWithBus();

    TransportsCatalogue::TransportRouter Router(*Temp);
    Router.SetRouterSetting(GetRouterSetting());
    Router.PrepareGraf();

    //catalog::Router RouterSer;
    // RouteSettings
    Seriliz->tempSerRouter_.mutable_routesettings()->set_velocity(GetRouterSetting().bus_velocity);
    Seriliz->tempSerRouter_.mutable_routesettings()->set_wait_time(GetRouterSetting().bus_wait_time);

    //InfoEdges
    for(auto & edge: Router.GetInfoEdges()){
        catalog::InfoEdge tempEdge;
        tempEdge.set_from(edge.first.first); // from
        tempEdge.set_to(edge.first.second);//to
        tempEdge.set_name_bus(edge.second.name_bus); //name_bus
        tempEdge.set_span_count(edge.second.span_count);//span_count
        tempEdge.set_time(edge.second.time);//time
        Seriliz->tempSerRouter_.mutable_infoedges()->mutable_infoe()->Add(std::move(tempEdge));

    }

    //InfoStop
    for(auto & stop: Router.GetInfoStop()){
       catalog::InfoStop tempStop;
       tempStop.set_id_s(stop.first); //id
       tempStop.set_name_bus(stop.second);// name_bus
       Seriliz->tempSerRouter_.mutable_infostops()->mutable_infos()->Add(std::move(tempStop));
    }

    //Edges
    for(auto & stop: Router.GetEdges()){
       catalog::Edge tempEdge;
       tempEdge.set_from(stop.from); //from
       tempEdge.set_to(stop.to);//to
       tempEdge.set_weight(stop.weight);//weight
       Seriliz->tempSerRouter_.mutable_edges()->Add(std::move(tempEdge));
    }

    //Seriliz->tempSerRouter_=RouterSer;


    if(Seriliz->GetSerilizating()){
        Seriliz->Serialize();

    }else{
        Seriliz->DeserializePerson();
    }

}

MapSetting& JsonReader::GetSetting()
{
    return Mapset;
}

RoutingSettings &JsonReader::GetRouterSetting()
{
     return Routerset;
}



void JsonReader::PrintInfoStop(InfoToPrintStop item,std::string request_id)
{
    if(item.stop_exist){
        std::cout<<"    {"<<std::endl<<"        \""
                <<"buses"<<"\""<<": "<<"["<<std::endl;

        int i=0;
        std::cout<<"            ";
        for(auto& it:item.buss){
            if(i>0){
                std::cout<<", \""<<it<<"\"";

            }else{
                std::cout<<"\""<<it<<"\"";}
            i++;

        }
        std::cout<<std::endl;
        std::cout<<"        ]"<<","<<std::endl<<"        \""
                <<"request_id"<<"\""<<": "<<request_id<<std::endl<<"    }";
    }else{
        std::cout<<"    {"<<std::endl<<"        \""
                <<"request_id"<<"\""<<": "<<request_id<<","<<std::endl<<"        \""
               <<"error_message"<<"\""<<": "<<"\""<<"not found"<<"\""<<std::endl<<"    }";

    }
}

void JsonReader::PrintInfoBus(Stats item, std::string request_id)
{   if(item.route_length==0 && item.route_length2==0 && item.stops==0 && item.unique_stops==0){
        std::cout<<"    {"<<std::endl<<"        \""<<"request_id"<<"\""<<": "<<request_id<<","
                <<std::endl<<"        \""<<"error_message"<<"\""<<": "<<"\""
               <<"not found"<<"\""<<std::endl<<"    }";

    }else{
        std::cout<<"    {"<<std::endl<<"        \""<<"curvature"<<"\""<<": "<<item.route_length2
                <<","<<std::endl<<"        \""<<"request_id"<<"\""<<": "<<request_id
               <<","<<std::endl<<"        \""<<"route_length"<<"\""<<": "
              <<item.route_length<<","
             <<std::endl<<"        \""<<"stop_count"<<"\""<<": "
            <<item.stops<<","<<std::endl<<"        \""
           <<"unique_stop_count"<<"\""<<": "<<item.unique_stops<<std::endl<<"    }";}

}


std::vector<StatRequest> JsonReader::GetReqInf()
{
    return ReqInf;
}
}
