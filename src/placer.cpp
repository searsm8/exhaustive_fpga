#include "placer.h"

Placer::Placer(string aux_filepath)
{
    srand(time(nullptr));
    // Read in all the design files
    design_files = bookshelf::DesignFiles(aux_filepath);
    design_files.read_lib_file(cell_lib);
    design_files.read_scl_file(site_lib, resource_lib, site_map, resource_map, layout_size);
    design_files.read_nodes_file(node_list, node_map, cell_lib);
    design_files.read_pl_file(site_map, fixed_nodes, node_map, resource_map);
    design_files.read_nets_file(net_list, node_map);


    for(int i = 0; i < 100; i++) { 
        sm8log::info("***GENERATING RANDOM PLACEMENT " + to_string(i));
        Placement placed_nodes = makeRandomPlacement();
        // write resulting placement to file
        design_files.write_pl_file(design_files.design_filepath + "outputs/" + "random_placement" + to_string(i) + ".pl", placed_nodes);
        sm8log::info("***FINISH RANDOM PLACEMENT***");
        unsigned long hpwl = computeTotalHPWL();
        sm8log::info("Total HPWL: " + to_string(hpwl));
        clearSites();
    }
}


void Placer::placeNodeRandomly(Node* np)
{
    int rand_x = rand()%layout_size.first;
    int rand_y = rand()%layout_size.second;
    Coord rand_coord = make_pair(rand_x, rand_y);
    np->setCoord(rand_coord);
    string res = resource_map.at(np->getType()->getName());
    //sm8log::info("Randomly placed Node at: (" + to_string(rand_x) + ", " + to_string(rand_y) + ") " + res);
    if(site_map.find(np->getCoord()) == site_map.end())
    {
        placeNodeRandomly(np);
        return;
    }
    Site* site = &site_map[np->getCoord()];
    //sm8log::info("Node " + np->getName() + " being placed in Site " + site->type->getName() + " at " + utils::coord_string(site->coord));
    if(!site->placeNode(np, res))
        placeNodeRandomly(np);

}

Placement Placer::makeRandomPlacement()
{
    Placement placed_nodes;
    for(Node* np : node_list)
    {
        placeNodeRandomly(np);
        placed_nodes[np] = np->getCoord();
    }

    return placed_nodes;
}


int Placer::computeTotalHPWL()
{
    int total_hpwl = 0;
    for(Net* net : net_list)
        total_hpwl += net->computeHPWL();
    return total_hpwl;
}


void Placer::clearSites()
{
    // Remove all nodes from sites
    for(auto it = site_map.begin(); it != site_map.end(); it++)
    {
        it->second.placed_nodes.clear();
        it->second.used_resource_counts.clear();
    }

}