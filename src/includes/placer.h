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
    CountMap site_counts;
    CountMap node_type_counts;
    vector<Placement> best_placements;
    map<Node*, NetList> node_to_net_list;

    unsigned long total_x_wl;
    unsigned long total_y_wl;
    unsigned long total_xy_wl;

    public:
    Placer(string aux_filepath);

    void placeNodeInNextCoord(Node* np);
    Coord getNextValidCoord(Node* np);
    void placeNodeRandomly(Node* np, NetList &modified_list);
    Placement makeRandomPlacement();

    unsigned long updateXWirelength(NetList nets_to_update);
    unsigned long updateYWirelength(NetList nets_to_update);
    unsigned long updateHPWL(NetList nets_to_update);
    void clearSites();
    void computeNumPossiblePlacements(Resource* resource);
    int getNumValidSites(Resource* resource);

    void appendNetsWhichBelongToNode(Node* np, NetList &nets);
};