#include "node.h"
using namespace std;

Node::Node(string name_, CellType* type_)
{
    name = name_;
    type = type_;
    coord = make_pair(0, 0);
    resource_num = 0;
    fixed = false;
    site = NULL;
}

//accessors
string Node::getName()
{ return name; }

CellType* Node::getType()
{ return type; }

Site* Node::getSite()
{ return site; }

Resource* Node::getResource()
{
    return getType()->getResource();
}

SiteType* Node::getSiteType()
{
    return getResource()->site_type;
}

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

void Node::setSite(Site* site_)
{ site = site_;}

void Node::setResourceNum(int num)
{ resource_num = num; }

void Node::setFixed(bool fixed_)
{ fixed = fixed_; }

