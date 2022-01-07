#pragma once
#include "common.h"
#include "pin.h"
#include "cell.h"
#include "resource.h"
#include "site.h"
#include "node.h"
#include "net.h"
using namespace std;

// a set of useful functions for reading and writing bookshelf format
namespace bookshelf {

// Typedefs to hold the design information read from the design files
typedef map<string, CellType> CellLibrary;
typedef map<string, Resource> ResourceLibrary;
typedef map<string, string> ResourceMap; // maps a CellType to a Resource type
typedef map<string, SiteType> SiteLibrary;
typedef map<Coord, Site> SiteMap;
typedef vector<Net*> NetList;
typedef vector<Node*> NodeList;
typedef map<string, Node*> NodeMap;
typedef map<string, int> CountMap;
typedef map<Node*, Coord> Placement;

string parseExtension(string filename);

class FileParser
{
    private: 
    string filepath;
    ifstream fs;  // reads design files

    public:
    FileParser(string filepath_);
    ~FileParser();
    bool parseNextLine(vector<string> &tokens);
};

class DesignFiles
{
    public:
    string aux_filepath;
    string design_filepath; //path where all design files can be found
    vector<string> design_filenames;
    string lib_filename;
    string nodes_filename;
    string scl_filename;
    string pl_filename;
    string nets_filename;

    DesignFiles();
    DesignFiles(string aux_filepath_);

    void read_aux_file();
    void read_lib_file(CellLibrary &cell_lib);
    void read_scl_file(SiteLibrary &site_lib, ResourceLibrary &resource_lib, SiteMap &site_map, 
                        ResourceMap &resource_map, Coord &layout_size, CountMap &site_counts, Coord &slice_coord_max);
    void read_nodes_file(NodeList &node_list, NodeMap &node_map, CellLibrary &cell_lib, CountMap &node_type_counts);
    void read_pl_file(SiteMap &site_map, NodeList &fixed_nodes, const NodeMap &node_map, ResourceMap &resource_map);
    void read_nets_file(NetList &net_list, NodeMap &node_map, map<Node*, NetList> &node_to_net_list);

    void write_pl_file(string filepath, Placement &p);
    
};

} //end bookshelf