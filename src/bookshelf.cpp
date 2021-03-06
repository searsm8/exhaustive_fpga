#include "bookshelf.h"

using namespace std;
// a set of useful functions for reading and writing bookshelf format

namespace bookshelf {

string parseExtension(string filename)
{ return filename.substr(filename.find('.')); }

FileParser::FileParser(string filepath_)
{
    filepath = filepath_;
    sm8log::trace("Begin parsing file: " + filepath);
    fs = ifstream(filepath);
    if(!fs.good()) sm8log::error("CANNOT OPEN FILE!");
}

FileParser::~FileParser()
{
    sm8log::trace("Finished parsing file: " + filepath);
    fs.close();
}

bool FileParser::parseNextLine(vector<string> &tokens)
{
    tokens.clear();
    string next_line, token;
    while(fs && tokens.empty())
    {
        getline(fs, next_line);
        stringstream ss(next_line);
        while(getline(ss, token, ' '))
        {
            if(token.size() > 0)
                tokens.push_back(token);
        }
    }
    return !tokens.empty();
}

DesignFiles::DesignFiles()
{}

DesignFiles::DesignFiles(string aux_filepath_)
{
    aux_filepath = aux_filepath_;

    design_filepath = aux_filepath.substr(0, aux_filepath.find_last_of('/')+1);
    sm8log::out("Reading design from: " + design_filepath);

    read_aux_file();

    for(string filename : design_filenames)
    {
        string extension = parseExtension(filename);
        if(extension == ".lib")
            lib_filename = filename;
        else if(extension == ".nets")
            nets_filename = filename;
        else if(extension == ".nodes")
            nodes_filename = filename;
        else if(extension == ".scl")
            scl_filename = filename;
        else if(extension == ".pl")
            pl_filename = filename;
        else sm8log::warning("Invalid file extension: " + filename);
    }
}

void DesignFiles::read_aux_file()
{
    FileParser parser(aux_filepath);
    vector<string> tokens;
    while(parser.parseNextLine(tokens))
    {
        if(tokens[0] == "#") // skip comments
            continue;
        for(string word : tokens)
            if(word.find('.') != string::npos)
                design_filenames.push_back(word);
    }
} 

void DesignFiles::read_lib_file(CellLibrary &cell_lib)
{
    FileParser parser(design_filepath + lib_filename);
    vector<string> tokens;
    CellType new_celltype;
    while(parser.parseNextLine(tokens))
    {
        if(tokens[0] == "CELL")
            new_celltype = CellType(tokens[1]);
        if(tokens[0] == "PIN")
            new_celltype.addPin(Pin(tokens[1], tokens[2]));
        if(tokens[0] == "END" && tokens[1] == "CELL") 
           cell_lib.insert({new_celltype.getName(), new_celltype});
    }

    // Display info on the library
    sm8log::info("***CELL LIBRARY***");
    for(auto it = cell_lib.begin(); it != cell_lib.end(); it++)
        sm8log::info(it->first + ": Pin count: " + to_string(it->second.getPinList().size()));
}

void DesignFiles::read_scl_file(SiteLibrary &site_lib, ResourceLibrary &resource_lib, SiteMap &site_map, ResourceMap &resource_map, Coord &layout_size, CountMap &site_counts, Coord &slice_coord_max)
{
    sm8log::info("Including only slices less than (" + to_string(slice_coord_max.first) + ", " + to_string(slice_coord_max.second) + ")");
    FileParser parser(design_filepath + scl_filename);
    vector<string> tokens;
    SiteType* new_sitetype;
    int mode = 0;
    while(parser.parseNextLine(tokens))
    {
        if(mode == 1) // if reading a new site type
        {
            if(tokens[0] == "END" && tokens[1] == "SITE") {
                site_lib[new_sitetype->getName()] = (*new_sitetype);
                mode = 0;
            }
            else {
                new_sitetype->addResource(tokens[0], stoi(tokens[1]));
            }
        }
        else if(mode == 2)
        {
            if(tokens[0] == "END" && tokens[1] == "RESOURCES") 
                mode = 0;
            else // read in a new resource
            {
                Resource new_resource(tokens[0]);
                for(int i = 1; i < tokens.size(); i++) {
                    new_resource.addCellType(tokens[i]);
                    resource_map[tokens[i]] = tokens[0];
                }
                resource_lib[tokens[0]] = new_resource;
            }
        }
        else if(mode == 3)
        {
            if(tokens[0] == "END" && tokens[1] == "SITEMAP") 
                mode = 0;
            else // read in a site for the site map
            {
                Coord site_coord = make_pair(stoi(tokens[0]), stoi(tokens[1]));
                Site* site = new Site(site_coord, &site_lib[tokens[2]]);
                if(site->type->getName() != "SLICE" || (site->coord.first < slice_coord_max.first && site->coord.second < slice_coord_max.second)) {
                    site_map[site->coord] = site;
                    if(site_counts[site->type->getName()])
                        site_counts[site->type->getName()]++;
                    else 
                        site_counts[site->type->getName()] = 1;
                }
                site->index = site->type->site_list.size(); // Index to quickly find this site in the list
                site->type->site_list.push_back(site); // add this site to the list for the SiteType
                //cout << "SiteType " << site.type->name << " has " << site.type->site_list.size() << " Sites." << endl;
                //int i = 5;
                //if(site.type->site_list.size() > i)
                //    cout << "SITE: " << "(" << site.type->site_list[i]->coord.first<< ", " << site.type->site_list[i]->coord.second<< ")" << endl;
                //cout << "SITE: " << "(" << site.x << ", " << site.y << ")" << endl;
            }
        }
        else if(tokens[0] == "SITE")
        {
            new_sitetype = new SiteType(tokens[1]);
            mode = 1; // set flag for new site type
        }
        else if(tokens[0] == "RESOURCES")
        {
            mode = 2; // set flag for new site type
        }
        else if(tokens[0] == "SITEMAP")
        {
            layout_size.first  = stoi(tokens[1]);
            layout_size.second = stoi(tokens[2]);
            mode = 3; // set flag for new site type
        }
    }

    // display summaries of scl file
    sm8log::info("***RESOURCES SUMMARY***");
    for(auto it = resource_lib.begin(); it != resource_lib.end(); it++)
        sm8log::info(it->second.to_string());

    sm8log::info("***CELL RESOURCES***");
    for(auto it = resource_map.begin(); it != resource_map.end(); it++)
        sm8log::info(it->first + ": " + it->second);
        //sm8log::info(it->first + ": " + it->second.getResource()->getName());

    sm8log::info("***SITEMAP SUMMARY***");
    sm8log::info("Number of Sites: " + to_string(site_map.size()));
    for(auto it = site_counts.begin(); it != site_counts.end(); it++)
        sm8log::info("Number of " + it->first + " Sites: " + to_string(it->second));
}

void DesignFiles::read_nodes_file(NodeList &node_list, NodeMap &node_map, CellLibrary &cell_lib, CountMap &node_type_counts)
{
    FileParser parser(design_filepath + nodes_filename);
    vector<string> tokens;
    while(parser.parseNextLine(tokens))
    {
        string node_name = tokens[0];
        CellType* type = &cell_lib.at(tokens[1]);
        string type_name = type->getName();
        Node* n = new Node(node_name, type);
        node_list.push_back(n);
        node_map[node_name] = n;
        if(node_type_counts[type_name])
            node_type_counts[type_name]++;
        else 
            node_type_counts[type_name] = 1;

    }

    //display info on the Node list
    sm8log::info("***NODE LIST SUMMARY***");
    sm8log::info("Node count: " + to_string(node_list.size()));
    for(auto it = node_type_counts.begin(); it != node_type_counts.end(); it++)
        sm8log::info(it->first + ": " + to_string(it->second) + " instances");
}

void DesignFiles::read_pl_file(SiteMap &site_map, NodeList &fixed_nodes, const NodeMap &node_map, ResourceMap &resource_map)
{
    FileParser parser(design_filepath + pl_filename);
    vector<string> tokens;
    while(parser.parseNextLine(tokens))
    {
        string name = tokens[0];
        Coord fixed_coord = make_pair(stoi(tokens[1]), stoi(tokens[2]));            
        int resource_num = stoi(tokens[3]);
        bool fixed = false;
        if(tokens.size() > 4)
            if(tokens[4] == "FIXED")
                fixed = true;
        Node* np = node_map.at(name);
        if(!np) {
            sm8log::error("Node in pl file does not exist: " + np->getName());
            continue;
        }
        string res_name = resource_map.at(np->getType()->getName());
        site_map[fixed_coord]->placeNode(np, res_name);
        np->setFixed(fixed);
        if(fixed)
            fixed_nodes.push_back(np);
    }
    
    // print info about the fixed nodes
    sm8log::info("***FIXED NODES SUMMARY***");
    sm8log::info("Design total fixed node count: " + to_string(fixed_nodes.size()));
}

void DesignFiles::read_nets_file(NetList &net_list, NodeMap &node_map, NetListMap &node_to_net_list)
{
    FileParser parser(design_filepath + nets_filename);
    vector<string> tokens;
    while(parser.parseNextLine(tokens))
    {
        if(tokens[0] == "endnet")
            continue;

        if(tokens[0] == "net")
        {
            Net* net = new Net(tokens[1]);
            int net_size = stoi(tokens[2]);
            for(int i = 0; i < net_size; i++)
            {
                parser.parseNextLine(tokens);
                string inst_name = tokens[0].substr(1);
                Node* np = node_map[inst_name];
                if(np)
                {
                    net->addNode(np);
                    node_to_net_list[np->getName()].push_back(net);
                }
                else sm8log::warning("Node does exist:" + inst_name + " " + to_string(tokens[0].size()));
            }
            net_list.push_back(net);
        }
    }

    // print info about the netlist
    sm8log::info("***NETLIST SUMMARY***");
    //for(auto it = net_list.begin(); it != net_list.end(); it++)
    //    sm8log::info((*it)->getName());
    sm8log::info("Number of nets: " + to_string(net_list.size()));
}

void DesignFiles::write_pl_file(string filepath, Placement &p)
{
    sm8log::trace("Begin writing file: " + filepath);
    ofstream ofs(filepath);
    for(auto it = p.begin(); it != p.end(); it++)
    {
        Node* n = it->first;
        Coord c = it->second;
        string fixed_str = "";
        if(n->isFixed())
            fixed_str = " FIXED";
        ofs << n->getName() << " " << c.first << " " << c.second << " " << n->getResourceNum() << fixed_str << endl;
    }
    sm8log::trace("Finished writing file: " + filepath);
    ofs.close();
}


} //end bookshelf