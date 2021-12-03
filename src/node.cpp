#include "node.h"
using namespace std;

Node::Node(string name_, CellType* type_)
{
    name = name_;
    type = type_;
    coord = make_pair(0, 0);
    resource_num = 0;
    fixed = false;
}
Node::Node(string name_, CellType* type_, Coord coord_)
{
    name = name_;
    type = type_;
    coord = coord_;
    resource_num = 0;
    fixed = false;
}

//accessors
string Node::getName()
{ return name; }

CellType* Node::getType()
{ return type; }

Coord Node::getCoord()
{ return coord; }

int Node::getResourceNum()
{ return resource_num; }
    
bool Node::isFixed()
{ return fixed; }


//mutators
/* Returns true if the new_coord is able to be set */
bool Node::setCoord(Coord new_coord)
{
    if(fixed)
        return false;
    else {    
        coord = new_coord; 
        return true;
    }
}

void Node::setResourceNum(int num)
{ resource_num = num; }

void Node::setFixed(bool fixed_)
{ fixed = fixed_; }