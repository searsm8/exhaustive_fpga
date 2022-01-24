#include "common.h"
#include "placer.h"
#include <thread>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
using namespace std;

string parse_args(int argc, char** argv)
{
    string aux_filepath = argv[1];
    return aux_filepath;
}

//pass placer by value so it calls copy constructor
void runPlacerSearch(string aux_filepath, int node_index)
{
    cout << "BEGIN PLACER THREAD: " << this_thread::get_id() << endl;
    Placer placer(aux_filepath);
    placer.exhaustiveNodePlacement(placer.node_list.size()-2);
    cout << "FINISH PLACER THREAD: " << this_thread::get_id() << endl;
}

int main(int argc, char** argv)
{
    string aux_filepath = parse_args(argc, argv);
    string design_filepath = aux_filepath.substr(0, aux_filepath.find_last_of('/'));
    // create output directory
    boost::filesystem::create_directory(design_filepath + "/outputs");
    string log_file = design_filepath + "/outputs/out.log";

    sm8log::setLevel(sm8log::DEBUG);
    sm8log::setlogFile(log_file);
    sm8log::out("------------------------------");
    sm8log::out("BEGIN FPGA EXHAUSTIVE SEARCH");
    sm8log::out("------------------------------");
    
    //Launch threads
    int thread_site_start = 0;
    vector<thread> threads;
    for(int i = 0; i < 20; i++) {
        threads.push_back(thread(runPlacerSearch, aux_filepath, 0));
    }

    for(int i = 0; i < threads.size(); i++) {
        threads[i].join(); // wait for the thread to finish
    }

    sm8log::out("------------------------------");
    sm8log::out("FINISHED FPGA EXHAUSTIVE SEARCH");
    sm8log::out("log file located at: " + log_file);
    return 0;
}
