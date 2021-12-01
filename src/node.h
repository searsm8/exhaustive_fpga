#pragma once
#include "common.h"
#include "cell.h"
using namespace std;

class Node
{
    private:
    string name;
    Coord coord;

    public:
    Node(string name_, Coord coord_);

    //accessors
    Coord getCoord();

    //mutators
    void setCoord(Coord new_coord);

};