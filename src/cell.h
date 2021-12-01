#pragma once
#include "common.h"
#include "pin.h"

// A class to represent a cell in the placement.
using namespace std;

class CellType
{
    private:
    string name;
    vector<Pin> pins;

    public:
    CellType();
    CellType(string name_);

    //accessors
    string getName();
    vector<Pin> getPinList();
    string toString();

    //mutators
    void addPin(Pin new_pin);

};