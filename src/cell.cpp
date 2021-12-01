#include "cell.h"
using namespace std;

CellType::CellType()
{}

CellType::CellType(string name_)
{
    name = name_;
}

string CellType::getName()
{
    return name;
}

vector<Pin> CellType::getPinList()
{
    return pins;
}

string CellType::toString()
{
    string cell_str = "CELL" + name + ":";
    for(Pin p : pins)
    {
        cell_str += "\n" + p.getName();
    }
    return cell_str;
}

void CellType::addPin(Pin new_pin)
{
    pins.push_back(new_pin);
}