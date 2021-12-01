#pragma once
#include "common.h"
#include "pin.h"
#include "cell.h"
#include "resource.h"
#include "node.h"
#include "net.h"
using namespace std;

// a set of useful functions for reading and writing bookshelf format
namespace bookshelf {
// Typedefs
typedef map<string, CellType> CellLibrary;
typedef map<string, Resource> ResourceLibrary;
typedef map<Coord, Resource*> SiteMap;
typedef vector<Net*> NetList;
typedef vector<Node*> NodeList;
typedef map<Node*, Coord> Placement;

string parseExtension(string filename);

class FileParser
{
    private: 
    ifstream fs;

    public:
    FileParser(string filepath);
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

    DesignFiles(string aux_filepath_);

    void read_aux_file();
    CellLibrary read_lib_file();
    void read_nodes_file();
    void read_scl_file();
    void read_pl_file();
    void read_nets_file();
};

} //end bookshelf_io