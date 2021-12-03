#pragma once

#include "common.h"
using namespace std;

//////////ESCAPE CODE DEFINITIONS//////////
#define ESC_RESET "\033[0m"
#define ESC_BOLD "\033[1m"
#define ESC_UNDERLINE "\033[4m"
#define ESC_BLINK "\033[5m"
#define ESC_REVERSE "\033[7m"
#define ESC_BLACK "\033[30m"
#define ESC_RED "\033[31m"
#define ESC_GREEN "\033[32m"
#define ESC_YELLOW "\033[33m"
#define ESC_BLUE "\033[34m"
#define ESC_MAGENTA "\033[35m"
#define ESC_CYAN "\033[36m"
#define ESC_WHITE "\033[37m"
#define ESC_DEFAULT "\033[39m"
#define ESC_BG_BLACK "\033[40m"
#define ESC_BG_RED "\033[41m"
#define ESC_BG_GREEN "\033[42m"
#define ESC_BG_YELLOW "\033[43m"
#define ESC_BG_BLUE "\033[44m"
#define ESC_BG_MAGENTA "\033[45m"
#define ESC_BG_CYAN "\033[46m"
#define ESC_BG_WHITE "\033[47m"
#define ESC_BG_DEFAULT "\033[49m"
#define ESC_OVERLINE "\033[53m"

namespace log
{

enum LEVEL{
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

void print(int level, string to_print);
void setLevel(int level);
void setLogFile(string filepath);

void trace(string to_print);
void debug(string to_print);
void info(string to_print);
void warning(string to_print);
void error(string to_print);
void fatal(string to_print);
} //end namespace logging