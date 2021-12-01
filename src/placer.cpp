#include "placer.h"

Placer::Placer(DesignFiles design_files)
{
    cell_lib = design_files.read_lib_file();
    design_files.read_nodes_file();
    design_files.read_scl_file();
    design_files.read_pl_file();
    design_files.read_nets_file();
}