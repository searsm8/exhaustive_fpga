#include "cell.h"
using namespace std;

CellType::CellType()
{}

CellType::CellType(string name_)
{
    name = name_;
}

string CellType::getName()
{ return name; }

vector<Pin> CellType::getPinList()
{ return pins; }

string CellType::to_string()
{
    string cell_str = "CELL" + name + ":";
    for(Pin p : pins)
    {
        cell_str += "\n" + p.getName();
    }
    return cell_str;
}

Resource* CellType::getResource()
{
    return resource;
}

void CellType::addPin(Pin new_pin)
{
    pins.push_back(new_pin);
}

void CellType::setResource(Resource* resource_)
{
    resource = resource_;
}