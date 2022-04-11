#include "input_reader.h"

using namespace std;

namespace TransportCatalogy {
namespace detail {
void  Request(std::istream &input, TransportCatalogue &&A,   std::vector<std::pair<std::string,bool>>& bus_request )
{

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
                    rez=FillingBusStop(str.substr(t,str.size()));
                    A.GetStops()->push_back(move(rez));
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



    Prepair_Spravochnick(move(A),list_input_bus);
    Prepair_Distance(A,list_input_stop);
    Prepair_zapros(input,bus_request);



}

void Prepair_Distance( TransportCatalogue &A,std::vector<std::string>&list_input_stop){

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
            dolg_size=i;                     //!!!!!!!!!!!!!
            if(firs_zap==2)
                break;

        }

        std::string dist;
        for(size_t i=++dolg_size;i<str.size();++i ){///!!!!!!!

            if((str[i]==',')||(i==str.size()-1)){
                if(str[i]!=',')
                    dist+=str[i];

                to.push_back(dist);
                std::string name_to;
                Parsing_distanc_substr(dist,Distance,name_to);
                dist="";

                if(A.get_stopname_to_stop()->count(name)!=0 && A.get_stopname_to_stop()->count(name_to)!=0 ){
                    Stop *a=A.get_stopname_to_stop()->at(name);
                    Stop *b=A.get_stopname_to_stop()->at(name_to);
                    A.SetDistance(a,b,Distance);
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
    Prepair(bus_name);
    name_to=bus_name;

}


void Prepair_Spravochnick( TransportCatalogue &&A,std::vector<std::string>&list_input_bus){
    A.AddStop();

    for (auto &str:list_input_bus){
        A.GetBuses()->push_back(FillingBusTrafic(str,A));
    }

    A.AddBus();

    for (auto &str:list_input_bus){
        FillingStopsWithBus(str,A);
    }

}

void Prepair_zapros(std::istream &input,std::vector<std::pair<std::string,bool>>& bus_request){

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
                    Prepair(stt);
                    bus_request.push_back({move(stt),1});
                    break;

                }

                if(first=="Stop"s){
                    int t=i;
                    ++t;
                    string stt=str.substr(t,str.size());
                    Prepair(stt);
                    bus_request.push_back({move(stt),0});
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

Stop FillingBusStop(string str)
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
Bus FillingBusTrafic(string str,TransportCatalogue &A)
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
    Prepair(nomer_bus);

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
                Prepair(cur_bus);
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
                Prepair(cur_bus);
                bus_stop.push_back(cur_bus);
                cur_bus="";


            }

        }
        AddComplitTraffic(bus_stop);
    }

    for (auto &str:bus_stop){
        Stop* stop;

        if(A.get_stopname_to_stop()->count(str)!=0){
            stop=A.get_stopname_to_stop()->at(str);
            temp.busStop.push_back(stop);
        }

    }

    return temp;
}
void FillingStopsWithBus(string str,TransportCatalogue &A)
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
    Prepair(nomer_bus);

    string cur_stop;
    for (size_t i=++nomer;i<str.size();++i){

        if(str[i]!='>' && str[i]!='-')
        {
            cur_stop+=str[i];
        }
        if(i==str.size()-1 || str[i]=='>' || str[i]=='-')
        {
            Prepair(cur_stop);
            bus_stop.push_back(cur_stop);
            cur_stop="";
        }

    }

    for (auto &str:bus_stop){
        Stop* stop;

        if(A.get_stopname_to_stop()->count(str)!=0){
            stop=A.get_stopname_to_stop()->at(str);
            if(A.get_busname_to_bus()->count(nomer_bus)!=0){
                A.get_stop_wiht_bus()->operator[](stop).insert(A.get_busname_to_bus()->at(nomer_bus)->name);

            }
        }

    }

}

void Prepair(string& str){

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
