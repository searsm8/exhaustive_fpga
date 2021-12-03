#include "logging.h"

namespace log
{
vector<string> level_string {"trace", "debug", "info", "WARNING", "ERROR", "FATAL"};
ofstream fs;

int print_level = LEVEL::INFO; // by default, print info, warnings, errors, and fatal messages
string file_path = "./";

void print(int level, string to_print)
{
    if(level >= print_level)
    {
        cout << "[ " << level_string[level] << " ] " << to_print << ESC_RESET << endl;
        fs << "[ " << level_string[level] << " ] " << to_print << endl;
    }
}

void setLevel(int level)
{
    print_level = level;
}

void setLogFile(string filepath_)
{
    file_path = filepath_;
    fs = ofstream(file_path);
}

void trace(string to_print)     { printf(ESC_BLUE); print(LEVEL::TRACE, to_print); }
void debug(string to_print)     { printf(ESC_GREEN); print(LEVEL::DEBUG, to_print); }
void info(string to_print)      { print(LEVEL::INFO, to_print); }
void warning(string to_print)   { printf(ESC_YELLOW); print(LEVEL::WARNING, to_print); }
void error(string to_print)     { printf(ESC_RED); print(LEVEL::ERROR, to_print); }
void fatal(string to_print)     { printf(ESC_BG_RED); print(LEVEL::FATAL, to_print); }
} //end namespace logging
