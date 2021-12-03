#include "resource.h"


Resource::Resource()
{}

Resource::Resource(string name_)
{ name = name_; }

void Resource::addCellType(string cell_type)
{
    cell_types.push_back(cell_type);
}

string Resource::getName()
{ return name; }

string Resource::to_string()
{
    string s = name + ":";
    for(int i = 0; i < cell_types.size(); i++)
        s += " " + cell_types[i];
    return s;
}