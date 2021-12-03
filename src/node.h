#pragma once
#include "common.h"
#include "cell.h"
using namespace std;

class Node
{
    private:
    string name;
    CellType* type;
    Coord coord;
    int resource_num;
    bool fixed;

    public:
    Node(string name_, CellType* type_);
    Node(string name_, CellType*type_, Coord coord_);

    //accessors
    string getName();
    CellType* getType();
    Coord getCoord();
    int getResourceNum();
    bool isFixed();

    //mutators
    bool setCoord(Coord new_coord);
    void setResourceNum(int num);
    void setFixed(bool fixed_);

};