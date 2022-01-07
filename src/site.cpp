#include "site.h"
#include "sitetype.h"

SiteType::SiteType()
{}

SiteType::SiteType(string name_)
{ name = name_; }

string SiteType::getName()
{ return name; }

map<string, int> SiteType::getSiteResourceCounts()
{ return site_resource_counts; }

void SiteType::addResource(string resource_name, int count)
{
    site_resource_counts[resource_name] = count;
}

Site::Site()
{}

Site::Site(Coord coord_, SiteType* type_)
{
    coord = coord_;
    type = type_;
}

bool Site::placeNode(Node* np, string resource_to_use)
{
    if(type->site_resource_counts.find(resource_to_use) == type->site_resource_counts.end())
    {
        //sm8log::warning("Resource type " + resource_to_use + " is not in this site type!");
        return false;
    }
    // check if placing this node is legal, return false if not
    if(used_resource_counts[resource_to_use] >= type->site_resource_counts[resource_to_use]) {
        // site has max number of this type already assigned, throw warning
        sm8log::warning("Site " + type->getName() + " (" + to_string(coord.first) + ", " + to_string(coord.second) + ")" 
            + " cannot add another Node of type " + np->getType()->getName()
            + "(used " + to_string(used_resource_counts[resource_to_use]) + " of " + to_string(type->getSiteResourceCounts()[resource_to_use]) + ")" 
            );
        return false;
    } else {
        np->setResourceNum(used_resource_counts[resource_to_use]++);
        placed_nodes.push_back(np);
        return true;
    }
}

bool Site::removeNode(Node* np)
// Return false if the node cannot be removed (probably because it hasn't been placed yet)
{

    return true;
}
