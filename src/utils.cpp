#include "utils.h"

namespace utils {
    
string coord_string(Coord &c)
{
    return "(" + to_string(c.first) + ", " + to_string(c.second) + ")";
}

}