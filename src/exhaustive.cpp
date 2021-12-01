#include "common.h"
#include "placer.h"
using namespace std;

string parse_args(int argc, char** argv)
{
    string aux_filepath = argv[1];
    return aux_filepath;
}

int main(int argc, char** argv)
{

    log::setLevel(log::DEBUG);
    log::setLevel(log::TRACE);
    log::info("------------------------------");
    log::info("BEGIN FPGA EXHAUSTIVE SEARCH");
    log::info("------------------------------\n");
    
    // Read in design files
    string aux_filepath = parse_args(argc, argv);
    bookshelf::DesignFiles design_files(aux_filepath);
    Placer placer(design_files);

    return 0;
}
