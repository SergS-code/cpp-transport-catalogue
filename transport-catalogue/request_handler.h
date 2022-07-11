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


namespace TransportsCatalogue {
class RequestHandler {
public:
    RequestHandler( renderer::MapRenderer& Renderer, JsonReader &Requests, TransportCatalogue& Catalogue_);
    void OutputResult();

private:
     renderer::MapRenderer& Renderer_;
     JsonReader &Requests_;
     TransportCatalogue& Catalogue_;
};
}
