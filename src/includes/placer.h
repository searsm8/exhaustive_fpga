#pragma once
#include "common.h"
#include "bookshelf.h"
using namespace std;
using namespace bookshelf;


class Placer
{
    private:
    bookshelf::DesignFiles design_files;
    Coord layout_size;
    CellLibrary cell_lib;
    ResourceLibrary resource_lib;
    ResourceMap resource_map;
    SiteLibrary site_lib;
    SiteMap site_map;
    NetList net_list;
    NodeList node_list;
    NodeList fixed_nodes;
    NodeMap node_map;
    vector<Placement> best_placements;

    public:
    Placer(string aux_filepath);

    void placeNodeRandomly(Node* n);
    Placement makeRandomPlacement();

    int computeTotalHPWL();
    void clearSites();
};