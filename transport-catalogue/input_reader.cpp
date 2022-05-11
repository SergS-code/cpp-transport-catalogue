#include "input_reader.h"

using namespace std;

namespace TransportsCatalogue {
namespace detail {
Requests  Request(std::istream &input, TransportCatalogue && transport_catalogue )
{
    Requests bus_request;
    string str;
    vector<string> list_input_bus;
    vector<string> list_input_stop;
    getline(input, str);
    int size_zapros=std::stoi(str);
    int breaking_i=0;

    while(input){
        if(breaking_i==size_zapros){
            break;
        }
        string first;
        getline(input, str);

        for(size_t i=0;i<str.size();++i){
            Stop rez;
            if(str[i]!=' '){
                first+=str[i];
            }else{
                if(first=="Stop"s){
                    int t=i;
                    ++t;
                    rez=ParseBusStop(str.substr(t,str.size()));
                    transport_catalogue.GetStops()->push_back(move(rez));
                    list_input_stop.push_back(str.substr(t,str.size()));
                    break;
                }

                if(first=="Bus"s){
                    int t=i;
                    ++t;
                    list_input_bus.push_back(str.substr(t,str.size()));
                    break;
                }
            }
        }
        breaking_i++;
    }
    PrepareCatalogue(move(transport_catalogue),list_input_bus);
    PrepareDistance(transport_catalogue,list_input_stop);
    PrepareRequests(input,bus_request);
    return bus_request;
}

void PrepareDistance(TransportCatalogue &transport_catalogue,std::vector<std::string>&list_input_stop){

    std::vector<std::string> to;
    std::unordered_map<string,std:: vector<std::string>> buss ;
    double Distance=0;
    for (auto &str:list_input_stop){
        string name;

        size_t name_size=0;
        for(size_t i=0;i<str.size();++i){
            if(str[i]!=':')
                name+=str[i];
            else {
                name_size=++i;
                break;
            }
        }
        int firs_zap=0;
        size_t dolg_size=0;
        for(size_t i=name_size;i<str.size();++i ){
            if(str[i]==','){
                ++firs_zap;
            }
            dolg_size=i;
            if(firs_zap==2)
                break;
        }

        std::string dist;
        for(size_t i=++dolg_size;i<str.size();++i ){

            if((str[i]==',')||(i==str.size()-1)){
                if(str[i]!=',')
                    dist+=str[i];

                to.push_back(dist);
                std::string name_to;
                Parsing_distanc_substr(dist,Distance,name_to);
                dist="";

                if(transport_catalogue.get_stopname_to_stop()->count(name)!=0 && transport_catalogue.get_stopname_to_stop()->count(name_to)!=0 ){
                    Stop *a=transport_catalogue.get_stopname_to_stop()->at(name);
                    Stop *b=transport_catalogue.get_stopname_to_stop()->at(name_to);
                    transport_catalogue.SetDistance(a,b,Distance);
                }
            }
            else{
                dist+=str[i];
            }
        }
    }
}

void Parsing_distanc_substr(std::string& dist,double& Dist,std::string& name_to){

    std::string distan;
    size_t size_dist=0;
    for( size_t i=0;i<dist.size();++i){
        ++size_dist;
        if(dist[i]=='m'){
            break;
        }
        if(dist[i]!=' '){
            distan+=dist[i];
        }
    }
    Dist=std::atof(distan.c_str());

    std::string skip_to;
    size_t size_skip=0;
    for( size_t i=size_dist;i<dist.size();++i){
        ++size_skip;
        if(dist[i]!=' '){
            skip_to+=dist[i];

        }
        if(skip_to=="to")
            break;
    }

    std::string bus_name;
    for( size_t i=size_skip+size_dist;i<dist.size();++i){
        bus_name+=dist[i];
    }
    RemovingSpaces(bus_name);
    name_to=bus_name;
}
void PrepareCatalogue(TransportCatalogue &&transport_catalogue, std::vector<std::string>&list_input_bus){
    transport_catalogue.PrepareStops();

    for (auto &str:list_input_bus){
        transport_catalogue.GetBuses()->push_back(ParseBus(str,transport_catalogue));
    }
    transport_catalogue.PrepareBus();

    for (auto &str:list_input_bus){
        ParseStopsWithBus(str,transport_catalogue);
    }
}

void PrepareRequests(std::istream &input, Requests &bus_request){

    string str;
    getline(input, str);
    int size_zapros_bus=std::stoi(str);
    int breaking_i_zap=0;
    while(input){

        string first;
        getline(input, str);
        for(size_t i=0;i<str.size();++i){
            if(str[i]!=' '){
                first+=str[i];
            }else{

                if(first=="Bus"s){
                    int t=i;
                    ++t;
                    string stt=str.substr(t,str.size());
                    RemovingSpaces(stt);
                    bus_request.requests.push_back({move(stt),1});
                    break;
                }

                if(first=="Stop"s){
                    int t=i;
                    ++t;
                    string stt=str.substr(t,str.size());
                    RemovingSpaces(stt);
                    bus_request.requests.push_back({move(stt),0});
                    break;

                }
            }
        }
        breaking_i_zap++;
        if(breaking_i_zap==size_zapros_bus){
            break;
        }
    }
}

Stop ParseBusStop(string str)
{
    Stop A;
    string name;
    size_t name_size=0;
    for(size_t i=0;i<str.size();++i){
        if(str[i]!=':')
            name+=str[i];
        else {
            A.name=name;
            name_size=++i;
            break;
        }
    }
    string shir;
    size_t shir_size=0;

    for(size_t i=name_size;i<str.size();++i ){
        if(str[i]==','){
            A.latitude=std::atof(shir.c_str());
            shir_size=++i;
            break;
        }
        if(str[i]!=' '){
            shir+=str[i];
        }
    }

    string dolgota;
    for(size_t i=shir_size;i<str.size();++i ){

        if(str[i]!=' '){
            dolgota+=str[i];
        }
    }
    A.longitude=std::atof(dolgota.c_str());
    return A;
}

Bus ParseBus(string str,TransportCatalogue &transport_catalogue)
{
    Bus temp;
    bool type=false;
    for( auto & c:str){
        if(c=='>'){
            type=true;
            break;
        }
    }
    string nomer_bus;
    size_t nomer=0;
    vector<string> bus_stop;
    for (size_t i=0;i<str.size();++i){

        if(str[i]==':'){
            nomer=i;
            break;}
        nomer_bus+=str[i];
    }
    RemovingSpaces(nomer_bus);
    temp.name=nomer_bus;

    if(type){
        string cur_bus;
        for (size_t i=++nomer;i<str.size();++i){

            if(str[i]!='>')
            {
                cur_bus+=str[i];
            }
            if(i==str.size()-1 || str[i]=='>')
            {
                RemovingSpaces(cur_bus);
                bus_stop.push_back(cur_bus);
                cur_bus="";
            }
        }
    }else{
        string cur_bus;
        for (size_t i=++nomer;i<str.size();++i){

            if(str[i]!='-')
            {
                cur_bus+=str[i];
            }
            if(i==str.size()-1 || str[i]=='-')
            {
                RemovingSpaces(cur_bus);
                bus_stop.push_back(cur_bus);
                cur_bus="";
            }
        }
        AddComplitTraffic(bus_stop);
    }

    for (auto &str:bus_stop){
        Stop* stop;

        if(transport_catalogue.get_stopname_to_stop()->count(str)!=0){
            stop=transport_catalogue.get_stopname_to_stop()->at(str);
            temp.busStop.push_back(stop);
        }

    }
    return temp;
}
void ParseStopsWithBus(string str,TransportCatalogue &transport_catalogue)
{
    string nomer_bus;
    size_t nomer=0;
    vector<string> bus_stop;
    for (size_t i=0;i<str.size();++i){

        if(str[i]==':'){
            nomer=i;
            break;}
        nomer_bus+=str[i];
    }
    RemovingSpaces(nomer_bus);
    string cur_stop;
    for (size_t i=++nomer;i<str.size();++i){

        if(str[i]!='>' && str[i]!='-')
        {
            cur_stop+=str[i];
        }
        if(i==str.size()-1 || str[i]=='>' || str[i]=='-')
        {
            RemovingSpaces(cur_stop);
            bus_stop.push_back(cur_stop);
            cur_stop="";
        }
    }

    for (auto &str:bus_stop){
        Stop* stop;

        if(transport_catalogue.get_stopname_to_stop()->count(str)!=0){
            stop=transport_catalogue.get_stopname_to_stop()->at(str);
            if(transport_catalogue.get_busname_to_bus()->count(nomer_bus)!=0){
                transport_catalogue.get_stop_wiht_bus()->operator[](stop).insert(transport_catalogue.get_busname_to_bus()->at(nomer_bus)->name);
            }
        }
    }

}

void RemovingSpaces(string& str){
    size_t pref=0;
    size_t suff=str.size();
    for(size_t i=0;i<str.size();++i){
        if(str[i]!=' '){
            pref=i;
            break;
        }
    }

    for(int i=str.size()-1;i>=0;--i){
        if(str[i]!=' '){
            suff=i;
            break;
        }

    }
    str=str.substr(pref,suff-pref+1);
}

void AddComplitTraffic(vector<string>& bus_stop){
    vector<string> copy=bus_stop;
    for (int i=copy.size()-2; i>=0; --i){
        bus_stop.push_back(copy[i]);
    }
}
}
}
