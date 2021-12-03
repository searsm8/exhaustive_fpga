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
    string aux_filepath = parse_args(argc, argv);
    string design_filepath = aux_filepath.substr(0, aux_filepath.find_last_of('/'));
    string log_file = design_filepath + "/out.log";

    log::setLevel(log::DEBUG);
    log::setLevel(log::TRACE);
    log::setLogFile(log_file);
    log::info("------------------------------");
    log::info("BEGIN FPGA EXHAUSTIVE SEARCH");
    log::info("------------------------------");
    
    // Read in design files
    Placer placer(aux_filepath);

    log::info("------------------------------");
    log::info("FINISHED FPGA EXHAUSTIVE SEARCH");
    log::info("------------------------------");
    log::info("Log file located at: " + log_file);
    return 0;
}
