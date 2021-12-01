#include "common.h"
#include "resource.h"
using namespace std;

class Site
{
    private:
    Coord coord;
    Resource* resource;

    public:
    Site(Coord coord_, Resource* resource_);
};