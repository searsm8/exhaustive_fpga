#include "placer.h"
#include <thread>

Placer::Placer(string aux_filepath)
{
    srand(time(nullptr));
    // Read in all the design files
    slice_coord_max = Coord(168, 480);
    design_files = bookshelf::DesignFiles(aux_filepath);
    design_files.read_lib_file(cell_lib);
    design_files.read_scl_file(site_lib, resource_lib, site_map, resource_map, layout_size, site_counts, slice_coord_max);
    design_files.read_nodes_file(node_list, node_map, cell_lib, node_type_counts);
    design_files.read_pl_file(site_map, fixed_nodes, node_map, resource_map);
    design_files.read_nets_file(net_list, node_map, node_to_net_list);

    // update the CellTypes in cell_lib to point to the correct SiteType which can accept them
    for(auto it = cell_lib.begin(); it != cell_lib.end(); it++) {
        string res_name = resource_map[it->second.getName()];
        cout << "CellType name: " << it->second.getName() << "\t";
        Resource* res = &resource_lib[res_name];
        it->second.setResource(res);
        cout << "Resource name: " << it->second.getResource()->getName() << endl;
    }

    // update Resources with the corresponding SiteType
    for(auto it = resource_lib.begin(); it != resource_lib.end(); it++) {
        string res_name = it->first;
        for(auto it2 = site_lib.begin(); it2 != site_lib.end(); it2++) {
            if(it2->second.site_resource_counts.find(res_name) != it2->second.site_resource_counts.end()){
                it->second.site_type = &it2->second;
                break;
            }
        }
        cout << "Resource name: " << it->second.getName() << "\t";
        cout << "SiteType name: " << it->second.site_type->getName() << endl;
    }

    //print SiteTypes
    for(auto it = site_lib.begin(); it != site_lib.end(); it++) {
        cout << "SiteType: " << it->second.getName() << " has " <<  it->second.site_list.size() << " sites." << endl;
    }

    //print Site coords
    for(auto it = site_map.begin(); it != site_map.end(); it++) {
        Site* sp = it->second;
        cout << "Site @ (" << it->first.first << ", " << it->first.second << ") is of type " << sp->type->name << " index=" << sp->index << endl;
        if(it->first.first > 0)
        break;
    }

    total_x_wl = 0;
    total_y_wl = 0;
    total_xy_wl = 0;

    //for(auto it = resource_lib.begin(); it != resource_lib.end(); it++) {
    //        computeNumPossiblePlacements(&it->second);
    //}

    //sm8log::info("(X, Y) Wirelength: (" + to_string(x_wl) + ", " + to_string(y_wl) + ") = " + to_string(hpwl));
    //clearSites();
    //std::thread th(&Placer::makeRandomPlacement, this);
    //th.join(); // wait for the thread to finish

    // compute the HPWL for the initial placement
    total_xy_wl = updateHPWL(net_list);

    clearSites();
    sm8log::debug("****Begin exhaustiveNodePlacement()");
    exhaustiveNodePlacement(0, 0);
    sm8log::debug("****Finished exhaustiveNodePlacement()");
}

/*
 * Primary driver for exhaustive search.
 * Recursive function.
 * node_num tracks which node to move. Any nodes previous to node_num should be locked.
 */
int count = 0;
int placements_evaluated = 0;
void Placer::exhaustiveNodePlacement(int node_index, int site_index_start)
{

    //end criteria
    if(node_index >= node_list.size()) {
        cout << "LAST NODE: evaluating..." << ++placements_evaluated << " placements evaluated" << endl;
        count--;
        return;
    }

    Node* np = node_list[node_index];

    if(np->isFixed()) {
       // cout << node_index << ": " << np->getName() << " is Fixed! Skipping..." << endl;
        exhaustiveNodePlacement(node_index+1, 0);
        count--;
        return;
    }
    cout << node_index << ": " << np->getName() << " computing." << endl;
    SiteType* stp = np->getSiteType();
    cout << "Recursive Count: " << ++count << endl;

    for(int site_index = site_index_start; site_index < stp->site_list.size(); site_index++)
    {
        //cout << node_index << ": " << stp->name << " @ site: " << site_index << endl;;
        Site* site = stp->site_list[site_index];
        //cout << "Trying " << np->getName() << ":" << np->getResource()->name << " in Site " << site->type->name << " (" << site->coord.first << ", " << site->coord.second << ")" << endl;
        exhaustiveNodePlacement(node_index+1, 0);
    }

    count--;

}

/*
 * Increase the coord by one position.
 * Increment the first (x position), unless it is at the edge
 * then reset x to 0 and increment y position
 */
void Placer::incrementCoord(Coord &c, Coord &max)
{
    if(c.first < max.first-1) {
        c.first++;
    } else if(c.second < max.second-1) {
        c.first = 0;
        c.second++;   
    } else {
        c.first = -1;
        c.second = -1;
    sm8log::debug("c is: (" + to_string(c.first) + ", " + to_string(c.second) + ")");
    }
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
    Site* site = site_map[np->getCoord()];
    //sm8log::debug("Node " + np->getName() + " being placed in Site " + site->type->getName() + " at " + utils::coord_string(site->coord));
    if(!site->placeNode(np, res))
    {
        placeNodeRandomly(np, modified_list);
        return;
    }

    for(Net* net : node_to_net_list[np->getName()])
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
    //sm8log::debug("Placer::updateHPWL on " + to_string(nets_to_update.size()) + " nets.");
    total_xy_wl = updateXWirelength(nets_to_update) + updateYWirelength(nets_to_update);
    return total_xy_wl;
}

void Placer::clearSites()
{
    // Remove all nodes from sites
    for(auto it = site_map.begin(); it != site_map.end(); it++)
    {
        it->second->placed_nodes.clear();
        it->second->used_resource_counts.clear();
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