#include "transport_router.h"
using namespace std;
namespace TransportsCatalogue {
template <typename Weight>
struct RouteInfo {
    Weight weight;
    std::vector<graph::EdgeId> edges;
};
TransportRouter::TransportRouter(TransportCatalogue &db):Temp(db){


}

void TransportRouter::SetRouterSetting(JsonReader &RouterSet_)
{
    RouterSet=&RouterSet_;
}
void TransportRouter::PrepareEdges(vector<graph::Edge<double>>& Edges,VertexId from,VertexId to,double weight){
    graph::Edge<double>temp;
    temp.from=from;
    temp.to=to;
    temp.weight=weight;
    Edges.push_back(temp);
}

void TransportRouter::Rezult(size_t from, size_t to , double &total_time, std::vector<InfoToPrintRoute>& rez, graph::Router<double>& marshrut)
{

    if(marshrut.BuildRoute(from,to).has_value()){
        total_time =marshrut.BuildRoute(from,to).value().weight;
        auto vect=marshrut.BuildRoute(from,to).value().edges;

        for (size_t i=0;i<vect.size();++i)
        {
            size_t from,to;
            from=Graph.GetEdge(vect[i]).from;
            to=Graph.GetEdge(vect[i]).to;

            InfoToPrintRoute temp_wait;
            temp_wait.type="Wait";
            temp_wait.stop_name=InfoStop.at(from);
            temp_wait.time=RouterSet->GetRouterSetting().bus_wait_time;
            rez.push_back(temp_wait);
            InfoToPrintRoute temp_bus;
            temp_bus.bus=InfoEdges.at({from,to}).name_bus;
            temp_bus.span_count=InfoEdges.at({from,to}).span_count;
            temp_bus.time=InfoEdges.at({from,to}).time;
            temp_bus.type="Bus";
            rez.push_back(temp_bus);
        }
    }
}

void TransportRouter::PrepareOneEdgeInfo(const string& bus_name, int span_count, double time, size_t from, size_t to){
    InfoEdge edge;
    edge.name_bus=bus_name;
    edge.span_count=span_count+1;
    edge.time=time;
    InfoEdges[{from,to}]=edge;
}

void TransportRouter::PrepareStops(){
    size_t id_stop=0;
    for (auto& stop: *Temp.GetStops()){
        stop.id=id_stop;
        InfoStop[stop.id]=stop.name;
        ++id_stop;
    }
}


void TransportRouter::PrepareGraf()
{

    double bus_velocity=RouterSet->GetRouterSetting().bus_velocity;
    double bus_wait_time=RouterSet->GetRouterSetting().bus_wait_time;
    double speed_metrs=bus_velocity*1000/60;
    PrepareStops();

    for(const auto& bus: *Temp.GetBuses()){
        if(bus.is_roundtrip){
            for (size_t i=0;i<bus.bus_stop.size()-1;++i){
                double distance=0;
                size_t k=i;
                size_t span_count=0;
                for (size_t j=i+1;j<bus.bus_stop.size();++j){
                    size_t from,to;
                    from=bus.bus_stop[i]->id;
                    to=bus.bus_stop[j]->id;
                    if(from!=to){
                        distance+=Temp.GetDistance(bus.bus_stop[k],bus.bus_stop[k+1]);
                        PrepareEdges(Edges, from, to, distance/speed_metrs + bus_wait_time);
                        PrepareOneEdgeInfo(bus.name,span_count,distance/speed_metrs,from,to);
                        ++span_count;
                    }
                    ++k;
                }
            }
        }else{

            for (size_t i=0;i<bus.bus_stop.size()/2;++i){
                double distance=0;
                size_t k=i;
                size_t span_count=0;
                for (size_t j=i+1;j<=bus.bus_stop.size()/2;++j){
                    size_t from,to;
                    from=bus.bus_stop[i]->id;
                    to=bus.bus_stop[j]->id;
                    if(from!=to){
                        distance+=Temp.GetDistance(bus.bus_stop[k],bus.bus_stop[k+1]);
                        PrepareEdges(Edges,bus.bus_stop[i]->id,bus.bus_stop[j]->id, distance/speed_metrs +bus_wait_time);
                        PrepareOneEdgeInfo(bus.name,span_count,distance/speed_metrs,from,to);
                        ++span_count;
                    }
                    ++k;
                }
            }

            for (size_t i=bus.bus_stop.size()/2;i<bus.bus_stop.size();++i){
                double distance=0;
                size_t k=i;
                size_t span_count=0;
                for (size_t j=i+1;j<bus.bus_stop.size();++j){
                    size_t from,to;
                    from=bus.bus_stop[i]->id;
                    to=bus.bus_stop[j]->id;
                    if(from!=to){
                        distance+=Temp.GetDistance(bus.bus_stop[k],bus.bus_stop[k+1]);
                        PrepareEdges(Edges,bus.bus_stop[i]->id,bus.bus_stop[j]->id, distance/speed_metrs + bus_wait_time);
                        PrepareOneEdgeInfo(bus.name,span_count,distance/speed_metrs,from,to);
                        ++span_count;
                    }
                    ++k;
                }
            }
        }
    }
    graph::DirectedWeightedGraph<double> graph_temp(Temp.GetStops()->size());
    for (auto & it:Edges){
        graph_temp.AddEdge(it);
    }
    Graph=std::move(graph_temp);

}

graph::DirectedWeightedGraph<double> &TransportRouter::ReturnGraf()
{
    return Graph;
}
}
