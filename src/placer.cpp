#include "placer.h"

Placer::Placer(string aux_filepath)
{
    // Read in all the design files
    bookshelf::DesignFiles design_files(aux_filepath);
    design_files.read_lib_file(cell_lib);
    design_files.read_scl_file(site_lib, resource_lib, site_map, resource_map);
    design_files.read_nodes_file(node_list, node_map, cell_lib);
    design_files.read_pl_file(site_map, fixed_nodes, node_map, resource_map);
    design_files.read_nets_file(net_list, node_map);
}