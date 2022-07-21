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

void TransportRouter::Rezult(size_t from, size_t to , double &total_time, std::vector<InfoToPrintRoute> &rez, graph::Router<double>& marshrut )
{

    if(marshrut.BuildRoute(from,to).has_value()){
        total_time =marshrut.BuildRoute(from,to).value().weight;
        auto vect=marshrut.BuildRoute(from,to).value().edges;

        for (size_t i=0;i<vect.size();++i)
        {
            size_t from,to;
            from=Graff.GetEdge(vect[i]).from;
            to=Graff.GetEdge(vect[i]).to;

            InfoToPrintRoute tempW;
            tempW.type="Wait";
            tempW.stop_name=InfoStop.at(from);
            tempW.time=RouterSet->GetRouterSetting().bus_wait_time;
            rez.push_back(tempW);
            InfoToPrintRoute tempB;
            tempB.bus=InfoEdges.at({from,to}).name_bus;//
            tempB.span_count=InfoEdges.at({from,to}).spaun_count;
            tempB.time=InfoEdges.at({from,to}).time;
            tempB.type="Bus";
            rez.push_back(tempB);
        }
    }
}

void TransportRouter::PrepareOneEdgeInfo(const string& busName,int spaunCount,double time,size_t from,size_t to){
    InfoEdge edge;
    edge.name_bus=busName;
    edge.spaun_count=spaunCount+1;
    edge.time=time;
    InfoEdges[{from,to}]=edge;
}

void TransportRouter:: PrepareStops(){
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
    double speedMetrs=bus_velocity*1000/60;
    PrepareStops();

    for(const auto& bus: *Temp.GetBuses()){

        if(bus.is_roundtrip){
            for (size_t i=0;i<bus.busStop.size()-1;++i){
                double distance=0;
                size_t k=i;
                size_t spaunCount=0;
                for (size_t j=i+1;j<bus.busStop.size();++j){

                    size_t from,to;
                    from=bus.busStop[i]->id;
                    to=bus.busStop[j]->id;
                    if(from!=to){
                        distance+=Temp.GetDistance(bus.busStop[k],bus.busStop[k+1]);
                        PrepareEdges(Edges, from, to, distance/speedMetrs + bus_wait_time);
                        PrepareOneEdgeInfo(bus.name,spaunCount,distance/speedMetrs,from,to);
                        ++spaunCount;
                    }
                    ++k;
                }
            }
        }else{

            for (size_t i=0;i<bus.busStop.size()/2;++i){
                double distance=0;
                size_t k=i;
                size_t spaunCount=0;
                for (size_t j=i+1;j<=bus.busStop.size()/2;++j){
                    size_t from,to;
                    from=bus.busStop[i]->id;
                    to=bus.busStop[j]->id;
                    if(from!=to){
                        distance+=Temp.GetDistance(bus.busStop[k],bus.busStop[k+1]);
                        PrepareEdges(Edges,bus.busStop[i]->id,bus.busStop[j]->id, distance/speedMetrs +bus_wait_time);
                        PrepareOneEdgeInfo(bus.name,spaunCount,distance/speedMetrs,from,to);
                        ++spaunCount;
                    }
                    ++k;
                }
            }

            for (size_t i=bus.busStop.size()/2;i<bus.busStop.size();++i){
                double distance=0;
                size_t k=i;
                size_t spaunCount=0;
                for (size_t j=i+1;j<bus.busStop.size();++j){
                    size_t from,to;
                    from=bus.busStop[i]->id;
                    to=bus.busStop[j]->id;
                    if(from!=to){
                        distance+=Temp.GetDistance(bus.busStop[k],bus.busStop[k+1]);
                        PrepareEdges(Edges,bus.busStop[i]->id,bus.busStop[j]->id, distance/speedMetrs + bus_wait_time);
                        PrepareOneEdgeInfo(bus.name,spaunCount,distance/speedMetrs,from,to);
                        ++spaunCount;
                    }
                    ++k;
                }
            }
        }
    }
    graph::DirectedWeightedGraph<double> Graf(Temp.GetStops()->size());
    for (auto & it:Edges){
        Graf.AddEdge(it);
    }
    Graff=std::move(Graf);

}

graph::DirectedWeightedGraph<double> &TransportRouter::ReturnGraf()
{
    return Graff;
}
}
