#pragma once
#include "common.h"
using namespace std;

class Resource
{
    private:
    public:
    string name;
    vector<string> cell_types;
    int count; // how many of this resource type exist in the design

    Resource();
    Resource(string name);

    void addCellType(string cell_type);
    string getName();
    string to_string();


};