#pragma once
#include "common.h"
#include "bookshelf_io.h"
using namespace std;
using namespace bookshelf;


class Placer
{
    private:
    CellLibrary cell_lib;
    ResourceLibrary resource_lib;
    SiteMap site_map;
    NetList net_list;
    NodeList node_list;
    vector<Placement> best_placements;

    public:
    Placer(DesignFiles design_files);
    
};