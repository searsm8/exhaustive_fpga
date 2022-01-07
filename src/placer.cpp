#include "placer.h"
#include <thread>

Placer::Placer(string aux_filepath)
{
    srand(time(nullptr));
    // Read in all the design files
    Coord slice_coord_max = std::make_pair(42, 120);
    design_files = bookshelf::DesignFiles(aux_filepath);
    design_files.read_lib_file(cell_lib);
    design_files.read_scl_file(site_lib, resource_lib, site_map, resource_map, layout_size, site_counts, slice_coord_max);
    design_files.read_nodes_file(node_list, node_map, cell_lib, node_type_counts);
    design_files.read_pl_file(site_map, fixed_nodes, node_map, resource_map);
    design_files.read_nets_file(net_list, node_map, node_to_net_list);

    total_x_wl = 0;
    total_y_wl = 0;
    total_xy_wl = 0;

    //sm8log::info("(X, Y) Wirelength: (" + to_string(x_wl) + ", " + to_string(y_wl) + ") = " + to_string(hpwl));
    //clearSites();
    std::thread th(&Placer::makeRandomPlacement, this);
    th.join(); // wait for the thread to finish

    //Placement placed_nodes = makeRandomPlacement();
    for(int i = 0; i < 5; i++) { 
        sm8log::info("***GENERATING RANDOM PLACEMENT " + to_string(i));
        clearSites();
        //Placement placed_nodes = makeRandomPlacement();

        Node* np = node_list[2000+i];
        NetList modified_list;
        placeNodeRandomly(np, modified_list);
        total_xy_wl = updateHPWL(modified_list);
        modified_list.clear();

        placeNodeRandomly(np, modified_list);
        total_xy_wl = updateHPWL(modified_list);
        modified_list.clear();

        // write resulting placement to file
        //design_files.write_pl_file(design_files.design_filepath + "outputs/" + "random_placement" + to_string(i) + ".pl", placed_nodes);
        sm8log::info("***FINISH RANDOM PLACEMENT***");
        sm8log::info("(X, Y) Wirelength: (" + to_string(total_x_wl) + ", " + to_string(total_y_wl) + ") = " + to_string(total_xy_wl));
    }

    for(auto it = resource_lib.begin(); it != resource_lib.end(); it++) {
        computeNumPossiblePlacements(&it->second);
    }
}

/*
 * Primary driver for exhaustive search.
 * Recursive function.
 * node_num tracks which node to move. Any nodes previous to node_num should be locked.
 */
void Placer::placeNodeInNextCoord(int node_num)
{
    Node* np = node_list[node_num];
}

Coord Placer::getNextValidCoord(Node* np)
{
    return np->getCoord();
}

void Placer::placeNodeRandomly(Node* np, NetList &modified_list)
{
    int rand_x = rand()%layout_size.first;
    int rand_y = rand()%layout_size.second;
    Coord rand_coord = make_pair(rand_x, rand_y);
    np->setCoord(rand_coord);
    string res = resource_map.at(np->getType()->getName());
    //sm8log::info("Randomly placed Node at: (" + to_string(rand_x) + ", " + to_string(rand_y) + ") " + res);
    if(site_map.find(np->getCoord()) == site_map.end())
    {
        placeNodeRandomly(np, modified_list);
        return;
    }
    Site* site = &site_map[np->getCoord()];
    //sm8log::debug("Node " + np->getName() + " being placed in Site " + site->type->getName() + " at " + utils::coord_string(site->coord));
    if(!site->placeNode(np, res))
    {
        placeNodeRandomly(np, modified_list);
        return;
    }

    for(Net* net : node_to_net_list[np])
        modified_list.push_back(net);
}

//NOT USED
Placement Placer::makeRandomPlacement()
{
    Placement placed_nodes;
    for(Node* np : node_list)
    {
        //placeNodeRandomly(np, modified_list);
        placed_nodes[np] = np->getCoord();
    }

    return placed_nodes;
}

/*
    Recompute the wirelengths of the nets specified in nets_to_update
*/
unsigned long Placer::updateXWirelength(NetList nets_to_update)
{
    for(Net* net : nets_to_update) {
        total_x_wl -= net->getXBound();
        total_x_wl += net->updateXBound();
    }
    return total_x_wl;
}

unsigned long Placer::updateYWirelength(NetList nets_to_update)
{
    for(Net* net : nets_to_update) {
        total_y_wl -= net->getYBound();
        total_y_wl += net->updateYBound();
    }
    return total_y_wl;
}

/*
 * Update the HPWL of all the nets in the list.
 * Useful to only update nets which changed.
*/
unsigned long Placer::updateHPWL(NetList nets_to_update)
{
    sm8log::debug("Placer::updateHPWL on " + to_string(nets_to_update.size()) + " nets.");
    total_xy_wl = updateXWirelength(nets_to_update) + updateYWirelength(nets_to_update);
    return total_xy_wl;
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

void Placer::computeNumPossiblePlacements(Resource* resource)
{
    int cell_type_count = 0;
    for(string s : resource->cell_types) {
        cell_type_count += node_type_counts[cell_lib[s].getName()];
    }
    int num_valid_sites = getNumValidSites(resource);
    int possible_placements = 0;
    sm8log::info("Resource " + resource->getName() + ": " + to_string(cell_type_count) + " instances, " \
            + to_string(num_valid_sites) + " valid sites.\t" + to_string(possible_placements) + " possible placements.");
}

int Placer::getNumValidSites(Resource* resource)
{
    int num_valid = 0;
    // get the resource that this CellType can be placed into
    string resource_type = resource->getName();
    Resource* rp = &resource_lib[resource_type];
    
    // get the number of sites that have this resource
    for(auto it = site_lib.begin(); it != site_lib.end(); it++)
    {
        SiteType* site_type = &it->second;
        for(auto it2 = site_type->site_resource_counts.begin(); it2 != site_type->site_resource_counts.end(); it2++)
            if(it2->first == resource_type)
                num_valid += site_counts[site_type->getName()];
    }
    return num_valid;
}


void Placer::appendNetsWhichBelongToNode(Node* np, NetList &nets)
{

}