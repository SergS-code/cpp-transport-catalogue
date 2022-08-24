#include "serialization.h"
using namespace std;

void Serializator::FillingCatalogToSerStop(catalog::Stop& stop)
{
    tempSeriliz_.mutable_stops()->Add(std::move(stop));
}

void Serializator::FillingCatalogToSerBus(catalog::Bus &bus)
{
    tempSeriliz_.mutable_buses()->Add(std::move(bus));
}

catalog::TransportCatalogue* Serializator::GettempSeriliz()
{
    return &tempSeriliz_;
}

catalog::RenderSettings* Serializator::GetMapSetting()
{
    return &tempSerMapRender_;
}

void Serializator::GetCatalog(TransportsCatalogue::TransportCatalogue &primary)
{
    Temp=&primary;
}

void Serializator::GetMapRenderer(TransportsCatalogue::renderer::MapRenderer &primary)
{
    TempRender=&primary;
}

void Serializator::GetRouter(TransportsCatalogue::TransportRouter &primary)
{
    TempRouter=&primary;
}

void Serializator::Serialize() {

    std::ofstream out_file(path_, std::ios::binary);
    catalog::Data data;
    data.mutable_tr()->CopyFrom(tempSeriliz_);
    data.mutable_r()->CopyFrom(tempSerRouter_);
    data.mutable_s()->CopyFrom(tempSerMapRender_);
    data.SerializeToOstream(&out_file);
//    tempSeriliz_.SerializeToOstream(&out_file);
//    tempSerMapRender_.SerializeToOstream(&out_file);
//    tempSerRouter_.SerializeToOstream(&out_file);
}

void Serializator::SerializeMapSetting() {

    //std::ofstream out_file(path_, std::ios::binary);
    //tempSerMapRender_.SerializeToOstream(&out_file);
}

void Serializator::SetPath(std::filesystem::__cxx11::path path){
    path_=path;
}

void Serializator::SetSerilizating(bool val)
{
    Serilizating=val;
}

bool Serializator::GetSerilizating()
{
    return Serilizating;
}

void Serializator::DeserializePerson()
{
    ifstream in_file11(path_, ios::binary);
    catalog::Data objectData;
    if (!objectData.ParseFromIstream(&in_file11)) {

    }


    for(auto & it: *objectData.mutable_tr()->mutable_stops()){
        TransportsCatalogue::Stop temp;
        temp.latitude=it.lat();
        temp.longitude=it.long_();
        temp.name=it.name();
        for(auto& dist :*it.mutable_distant())
            temp.road_distances[dist.stop_name()]=dist.distant();

        Temp->GetStops()->push_back(temp);
    }
    Temp->PrepareStops();

    for(auto & it: *objectData.mutable_tr()->mutable_buses()){
        TransportsCatalogue::Bus temp;
        for(auto & stop: it.stops()){
            temp.bus_stop.push_back(Temp->get_stopname_to_stop()->at(stop));
        }
        temp.is_roundtrip=it.is_round_trip();
        temp.name=it.name();
        Temp->GetBuses()->push_back(temp);
    }

    Temp->PrepareBus();
    Temp->PrepareDistance();
    Temp->PrepareStopsWithBus();

    size_t id_stop=0;
    for (auto& stop: *Temp->GetStops()){
        stop.id=id_stop;
        ++id_stop;
    }

    //MapREnder
//    ifstream in_file2(path_, ios::binary);
//    catalog::RenderSettings objectR;
//    if (!objectR.ParseFromIstream(&in_file2)) {

//    }

    TransportsCatalogue::MapSetting temp;
    temp.width=objectData.mutable_s()->width();
    temp.height=objectData.mutable_s()->height();
    temp.padding=objectData.mutable_s()->padding();
    temp.line_width=objectData.mutable_s()->line_width();
    temp.stop_radius=objectData.mutable_s()->stop_radius();
    temp.bus_label_font_size=objectData.mutable_s()->bus_label_font_size();
    temp.bus_label_offset[0]=objectData.mutable_s()->bus_label_offset().x();
    temp.bus_label_offset[1]=objectData.mutable_s()->bus_label_offset().y();

    temp.stop_label_font_size=objectData.mutable_s()->stop_label_font_size();

    temp.stop_label_offset[0]=objectData.mutable_s()->stop_label_offset().x();
    temp.stop_label_offset[1]=objectData.mutable_s()->stop_label_offset().y();


    switch (objectData.mutable_s()->underlayer_color().color_case()) {
    case catalog::Color::kStringColor :
        temp.underlayer_color = objectData.mutable_s()->underlayer_color().string_color();
        break;
    case catalog::Color::kRgbColor :
    {
        TransportsCatalogue::color color;
        auto &p_rgb = objectData.mutable_s()->underlayer_color().rgb_color();
        std::array<int,3> te;
        te[0]=p_rgb.r();
        te[1]=p_rgb.g();
        te[2]=p_rgb.b();
        temp.underlayer_color= te;
    }
        break;
    case catalog::Color::kRgbaColor :
    {
        auto &p_rgba =objectData.mutable_s()->underlayer_color().rgba_color();
        std::array<double,4> te;
        te[0]=p_rgba.r();
        te[1]=p_rgba.g();
        te[2]=p_rgba.b();
        te[3]=p_rgba.o();

        temp.underlayer_color= te;
    }
        break;
    default:

        break;
    }

    temp.underlayer_width=objectData.mutable_s()->underlayer_width();

    for(auto & it :objectData.mutable_s()->color_palette()){
        switch (it.color_case()) {
        case catalog::Color::kStringColor :
             temp.color_palette.push_back(it.string_color());
            break;
        case catalog::Color::kRgbColor :
        {
            TransportsCatalogue::color color;
            auto &p_rgb = it.rgb_color();
            std::array<int,3> te;
            te[0]=p_rgb.r();
            te[1]=p_rgb.g();
            te[2]=p_rgb.b();
            temp.color_palette.push_back(te);
        }
            break;
        case catalog::Color::kRgbaColor :
        {
            auto &p_rgba =it.rgba_color();
            std::array<double,4> te;
            te[0]=p_rgba.r();
            te[1]=p_rgba.g();
            te[2]=p_rgba.b();
            te[3]=p_rgba.o();
            temp.color_palette.push_back(te);
        }
            break;
        default:

            break;
        }
    }

    TempRender->SetMapSetting(temp);



    TransportsCatalogue::TransportRouter Router(*Temp);
    TempRouter->GetRoutingSettings().bus_velocity=objectData.mutable_r()->routesettings().velocity();//velocity
    TempRouter->GetRoutingSettings().bus_wait_time=objectData.mutable_r()->routesettings().wait_time();//wait_time

    //InfoEdges
    for(auto &edge :*objectData.mutable_r()->mutable_infoedges()->mutable_infoe()){
        TransportsCatalogue::InfoEdge tempInfo;
        tempInfo.name_bus=edge.name_bus();
        tempInfo.span_count=edge.span_count();
        tempInfo.time=edge.time();
        TempRouter->GetInfoEdges()[{edge.from(),edge.to()}]=tempInfo;
    }


     //InfoStop
    for(auto &stop :*objectData.mutable_r()->mutable_infostops()->mutable_infos()){
       TempRouter->GetInfoStop()[stop.id_s()]=stop.name_bus();
    }

    //Edges
    for(auto &edge :*objectData.mutable_r()->mutable_edges()){
      graph::Edge<double> tempEdge;
      tempEdge.from=edge.from();
      tempEdge.to=edge.to();
      tempEdge.weight=edge.weight();
      TempRouter->GetEdges().push_back(tempEdge);
    }

    //Graph

    graph::DirectedWeightedGraph<double> graph_temp(Temp->GetStops()->size());
    for (auto & it:TempRouter->GetEdges()){
        graph_temp.AddEdge(it);
    }
    TempRouter->GetGraph()=std::move(graph_temp);

}
void Serializator::DeserializeMapRender()
{


}
