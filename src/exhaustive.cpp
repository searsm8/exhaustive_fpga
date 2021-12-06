#include "common.h"
#include "placer.h"
#include <boost/filesystem.hpp>
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
    // create output directory
    boost::filesystem::create_directory(design_filepath + "/outputs");
    string log_file = design_filepath + "/outputs/out.log";

    sm8log::setLevel(sm8log::INFO);
    sm8log::setlogFile(log_file);
    sm8log::out("------------------------------");
    sm8log::out("BEGIN FPGA EXHAUSTIVE SEARCH");
    sm8log::out("------------------------------");
    
    Placer placer(aux_filepath);

    sm8log::out("------------------------------");
    sm8log::out("FINISHED FPGA EXHAUSTIVE SEARCH");
    sm8log::out("------------------------------");
    sm8log::out("log file located at: " + log_file);
    return 0;
}
