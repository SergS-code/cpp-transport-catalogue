#include "request_handler.h"
using namespace std;


TransportsCatalogue::RequestHandler::RequestHandler(renderer::MapRenderer &Renderer,  TransportsCatalogue::JsonReader &Requests, TransportCatalogue &Catalogue, TransportRouter &Router)
    :Renderer_(Renderer), Requests_(Requests),Catalogue_(Catalogue),Router_(Router),marshrut(Router_.ReturnGraf())
{
    
    
    
}

void TransportsCatalogue::RequestHandler::OutputResult()
{
    json::Array arr;
    if(Requests_.GetReqInf().size()!=0)
    {
        for(auto& item: Requests_.GetReqInf())
        {
            if(item.type=="Map"){
                json::Dict tem;
                tem["map"]=Renderer_.GetMap();
                tem["request_id"]=item.id;
                arr.push_back(tem);
            }
            if(item.type=="Stop"){
                InfoToPrintStop temp_info=Catalogue_.GetStopInfo(item.name);
                if(temp_info.stop_exist){
                    json::Dict tem;
                    json::Array busss;
                    for(auto& it:temp_info.buss){
                        busss.push_back(string(it));
                    }
                    tem["buses"]=busss;
                    tem["request_id"]=item.id;
                    arr.push_back(tem);
                    
                }else{
                    json::Dict tem;
                    tem["request_id"]=item.id;
                    tem["error_message"]="not found";
                    arr.push_back(tem);
                }
                
            }
            if(item.type=="Bus"){
                Stats temp_info=Catalogue_.GetBusInfo(item.name);
                if(temp_info.route_length==0 && temp_info.route_length2==0 && temp_info.stops==0 && temp_info.unique_stops==0){
                    json::Dict tem;
                    tem["request_id"]=item.id;
                    tem["error_message"]="not found";
                    arr.push_back(tem);
                    
                }else{
                    json::Dict tem;
                    tem["curvature"]=temp_info.route_length2;
                    tem["request_id"]=item.id;
                    tem["route_length"]=temp_info.route_length;
                    tem["stop_count"]=temp_info.stops;
                    tem["unique_stop_count"]=temp_info.unique_stops;
                    arr.push_back(tem);
                    
                }
                
            }
            if(item.type=="Route"){
                json::Dict items;
                json::Array itemsIntoItems;
                std::vector<InfoToPrintRoute> rez;
                size_t from,to;
                
                from=Catalogue_.get_stopname_to_stop()->at(item.from)->id;
                to=Catalogue_.get_stopname_to_stop()->at(item.to)->id;
                double weit_total=0;
                
                Router_.Rezult(from,to,weit_total,rez,marshrut);
                
                if(from==to){
                    items["items"]=itemsIntoItems;
                    items["request_id"]=item.id;
                    items["total_time"]=weit_total;
                    arr.push_back(items);
                    
                }else if(rez.size()==0){
                    std::string str;
                    str="not found";
                    items["error_message"]=str;
                    items["request_id"]=item.id;
                    arr.push_back(items);
                }else{
                    for (auto & infoIt : rez){
                        if(infoIt.type=="Wait"){
                            json::Dict info;
                            info["stop_name"]=infoIt.stop_name;
                            info["time"]=infoIt.time;
                            info["type"]=infoIt.type;
                            itemsIntoItems.push_back(info);
                            
                        }
                        if(infoIt.type=="Bus"){
                            json::Dict info;
                            info["bus"]=infoIt.bus;
                            info["span_count"]=infoIt.span_count;
                            info["time"]=infoIt.time;
                            info["type"]=infoIt.type;
                            itemsIntoItems.push_back(info);
                        }
                    }
                    
                    items["items"]=itemsIntoItems;
                    items["request_id"]=item.id;
                    items["total_time"]=weit_total;
                    arr.push_back(items);
                }
            }
        }
        json::Print(
                    json::Document{
                        // Форматирование не имеет формального значения:
                        // это просто цепочка вызовов методов
                        json::Builder{}.Value(arr).Build()
                    },
                    cout
                    );
        
    }
    
    
}







