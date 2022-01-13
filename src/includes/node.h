#pragma once
#include "common.h"
#include "cell.h"
using namespace std;

class Site;

class Node
{
    private:
    string name;
    CellType* type;
    Site* site; // the site this node is currently assigned to
    Coord coord;
    int resource_num;
    bool fixed;

    public:
    Node(string name_, CellType* type_);

    //accessors
    string getName();
    CellType* getType();
    Site* getSite();
    Resource* getResource();
    SiteType* getSiteType();
    Coord getCoord();
    int getResourceNum();
    bool isFixed();

    //mutators
    bool setCoord(Coord new_coord);
    void setSite(Site* site_);
    void setResourceNum(int num);
    void setFixed(bool fixed_);

};