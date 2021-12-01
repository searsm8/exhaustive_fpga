#include "node.h"
using namespace std;

Node::Node(string name_, Coord coord_)
{
    name = name_;
    coord = coord_;
}

//accessors
Coord Node::getCoord()
{
    return coord;
}

//mutators
void Node::setCoord(Coord new_coord)
{
    coord = new_coord;
}