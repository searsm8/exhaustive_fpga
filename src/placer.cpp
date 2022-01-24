#include "placer.h"
#include <thread>
#include <boost/thread/thread.hpp>

Placer::Placer(string aux_filepath)
{
    srand(time(nullptr));
    // Read in all the design files
    //slice_coord_max = Coord(42, 120);
    slice_coord_max = Coord(168, 480);
    design_files = bookshelf::DesignFiles(aux_filepath);
    design_files.read_lib_file(cell_lib);
    design_files.read_scl_file(site_lib, resource_lib, site_map, resource_map, layout_size, site_counts, slice_coord_max);
    design_files.read_nodes_file(node_list, node_map, cell_lib, node_type_counts);
    design_files.read_pl_file(site_map, fixed_nodes, node_map, resource_map);
    design_files.read_nets_file(net_list, node_map, node_to_net_list);

    //move all fixed nodes to the front of the node_list
    for(auto it = node_list.begin(); it != node_list.end(); it++) {
        if((*it)->isFixed()) {
            Node* np = (*it);
            node_list.erase(it);
            node_list.insert(node_list.begin(), np);
        }
    }

    // update the CellTypes in cell_lib to point to the correct SiteType which can accept them
    for(auto it = cell_lib.begin(); it != cell_lib.end(); it++) {
        string res_name = resource_map[it->second.getName()];
        //cout << "CellType name: " << it->second.getName() << "\t";
        Resource* res = &resource_lib[res_name];
        it->second.setResource(res);
        //cout << "Resource name: " << it->second.getResource()->getName() << endl;
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
        //cout << "Resource name: " << it->second.getName() << "\t";
        //cout << "SiteType name: " << it->second.site_type->getName() << endl;
    }

    total_x_wl = 0;
    total_y_wl = 0;
    total_xy_wl = 0;
    placements_evaluated = 0;

    //for(auto it = resource_lib.begin(); it != resource_lib.end(); it++) {
    //        computeNumPossiblePlacements(&it->second);
    //}

    //sm8log::info("(X, Y) Wirelength: (" + to_string(x_wl) + ", " + to_string(y_wl) + ") = " + to_string(hpwl));
    clearSites();
    //std::thread th(&Placer::makeRandomPlacement, this);
    //th.join(); // wait for the thread to finish

    // compute the HPWL for the initial placement
    total_xy_wl = updateHPWL(net_list);

}



/*
 * Primary driver for exhaustive search.
 * Recursive function.
 * node_num tracks which node to move. Any nodes previous to node_num should be locked.
 */
int recur_count = 0;
unsigned long long total_placements_evaluated = 0;
int progress = 0;
long best_HPWL = numeric_limits<long>::max();
vector<thread*> threads;
bool started = false;
void Placer::exhaustiveNodePlacement(int node_index)
{
    recur_count++;

    //end criteria
    if(node_index >= node_list.size()) {
        //cout << "LAST NODE: evaluating..." << " HPWL = " << total_xy_wl << "\t" << ++placements_evaluated 
        //    << " placements evaluated" << " : Progress " << progress << "/" << node_list[0]->getSiteType()->site_list.size() << endl;
        //if HPWL is good enough, put in top 100 best placements
        placements_evaluated++; 
        total_placements_evaluated++; 
        if(total_xy_wl < best_HPWL)
            best_HPWL = total_xy_wl;
        recur_count--;
        return;
    }

    Node* np = node_list[node_index];

    if(np->isFixed()) {
        //cout << node_index << ": " << np->getName() << " is Fixed! Skipping..." << endl;
        exhaustiveNodePlacement(node_index+1);
        recur_count--;
        return;
    }
    //cout << node_index << ": " << np->getName() << " computing." << endl;
    SiteType* stp = np->getSiteType();
    //cout << "Recursive Count: " << recur_count << endl;

    for(int site_index = 0; site_index < stp->site_list.size(); site_index++)
    {
        //cout << node_index << ": " << stp->name << " @ site: " << site_index << endl;
        Site* site = stp->site_list[site_index];
        if(!site->placeNode(np, np->getResource()->name)) {
            //cout << "ILLEGAL NODE PLACEMENT: " << site->coord.first << ", " << site->coord.second << endl;
            continue;
        }

        if(node_index == node_list.size()-2) {
            progress++;
            cout << "Thread: " << hex << this_thread::get_id() << " | " << dec << np->getName() << " updated:"<< " HPWL = " << total_xy_wl << "\t | " 
                << " placements evaluated: " << placements_evaluated  << " | Progress " << progress << "/" << node_list[node_list.size()-2]->getSiteType()->site_list.size()
                << "\tBest HPWL: " << best_HPWL << " total_placements: " << total_placements_evaluated << endl;
        }

        // after placing this node in a new site, update all net's HPWL that this node is on
        // get all nets that this node np is on
        NetList modified_list;
        for(Net* net : node_to_net_list[np->getName()])
            modified_list.push_back(net);
        total_xy_wl = updateHPWL(modified_list);
        modified_list.clear();

        //cout << "Trying " << np->getName() << ":" << np->getResource()->name << " in Site " << site->type->name << " (" << site->coord.first << ", " << site->coord.second << ")" << endl;
        //if(!started && node_index == node_list.size()-2) {
        if(false) {
            Placer thread_placer = *this; //copy constructor
            Placer thread_placer2 = *this; //copy constructor
            
            //thread thr(&Placer::exhaustiveNodePlacement, this, node_index+1);
            //std::thread thr( [&] { this->exhaustiveNodePlacement(node_index+1); });
            // add a new thread
            //threads.push_back( boost::thread([&] { this->exhaustiveNodePlacement(node_index+1); }) );
            //thread* new_thread = new thread([&] { thread_placer.exhaustiveNodePlacement(node_index+1); }) ;
            thread* new_thread = new thread(&Placer::exhaustiveNodePlacement, &thread_placer, node_index+1);
            thread* new_thread2 = new thread(&Placer::exhaustiveNodePlacement, &thread_placer2, node_index+1);
            cout << "STARTED THREAD " << new_thread->get_id() << endl;
            threads.push_back(new_thread);
            threads.push_back(new_thread2);
            //if(threads.size() > 3)
                for(auto t : threads) {
                    auto id = t->get_id();
                    cout << "WAITING TO FINISH THREAD " << id << endl;
                    t->join();
                    cout << "FINISHED THREAD " << id << endl << endl;
                }
            started = true;
            //exhaustiveNodePlacement(node_index+1);
        } else {
            exhaustiveNodePlacement(node_index+1);
        }
    }

    recur_count--;
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

unsigned long Placer::updateHPWL()
{
    return updateHPWL(net_list);
}

void Placer::clearSites()
{
    // Remove all nodes from sites
    for(auto it = site_map.begin(); it != site_map.end(); it++) {
        it->second->placed_nodes.clear();
        it->second->used_resource_counts.clear();
    }

    for(auto it = node_list.begin(); it != node_list.end(); it++) {
        Node* np = *it;
        if(!np->isFixed()) {
            np->setSite(nullptr);
            np->setResourceNum(0);
        }
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