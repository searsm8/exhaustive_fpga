#include "logging.h"

namespace sm8log
{
vector<string> level_string {"trace", "debug", "info", "WARNING", "ERROR", "FATAL", "OUT"};
ofstream fs;

int print_level = LEVEL::INFO; // by default, print info, warnings, errors, and fatal messages
string file_path = "./";

void print(int level, string to_print)
{
    cout << "[ " << level_string[level] << " ] " << to_print << ESC_RESET << endl;
    fs << "[ " << level_string[level] << " ] " << to_print << endl;
}

void setLevel(int level)
{
    print_level = level;
}

void setlogFile(string filepath_)
{
    file_path = filepath_;
    fs = ofstream(file_path);
}

void trace(string to_print)     { if(print_level <= LEVEL::TRACE) { printf(ESC_BLUE); print(LEVEL::TRACE, to_print); } }
void debug(string to_print)     { if(print_level <= LEVEL::DEBUG) { printf(ESC_GREEN); print(LEVEL::DEBUG, to_print); } }
void info(string to_print)      { if(print_level <= LEVEL::INFO)  { print(LEVEL::INFO, to_print); }}
void warning(string to_print)   { if(print_level <= LEVEL::WARNING) { printf(ESC_YELLOW); print(LEVEL::WARNING, to_print); } }
void error(string to_print)     { if(print_level <= LEVEL::ERROR) { printf(ESC_RED); print(LEVEL::ERROR, to_print); } }
void fatal(string to_print)     { if(print_level <= LEVEL::FATAL) { printf(ESC_BG_RED); print(LEVEL::FATAL, to_print); } }
void out(string to_print)      { print(LEVEL::OUT, to_print); }
} //end namespace sm8logging
