#pragma once
#include "common.h"
#include "resource.h"
#include "pin.h"

// A class to represent a cell in the placement.
using namespace std;

class SiteType;

class CellType
{
    private:
    string name;
    Resource* resource;
    vector<Pin> pins;

    public:
    CellType();
    CellType(string name_);

    //accessors
    string getName();
    vector<Pin> getPinList();
    string to_string();
    Resource* getResource();

    //mutators
    void addPin(Pin new_pin);
    void setResource(Resource* resource_);

};