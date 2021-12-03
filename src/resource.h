#pragma once
#include "common.h"
using namespace std;

class Resource
{
    private:
    public:
    string name;
    vector<string> cell_types;

    Resource();
    Resource(string name);

    void addCellType(string cell_type);
    string getName();
    string to_string();


};