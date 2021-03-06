#include "site.h"
#include "sitetype.h"

SiteType::SiteType()
{}

SiteType::SiteType(string name_)
{
    name = name_;
}

string SiteType::getName()
{ return name; }

map<string, int> SiteType::getSiteResourceCounts()
{ return site_resource_counts; }

void SiteType::addResource(string resource_name, int count)
{
    site_resource_counts[resource_name] = count;
}

bool SiteType::hasResource(string res)
{
    return site_resource_counts.find(res) != site_resource_counts.end();
}

Site::Site()
{}

Site::Site(Coord coord_, SiteType* type_)
{
    coord = coord_;
    type = type_;
}

bool Site::placeNode(Node* np, string res_name)
{
    //cout << "Placing Node " << np->getName() << " at (" << coord.first << ", " << coord.second << ")" << endl;
    // if this site does not have the right kind of resource, unable to place node, return false.
    // NOT NECESSARY since only correct sites are used.
    //if(type->site_resource_counts.find(res_name) == type->site_resource_counts.end())
    //{
    //    sm8log::warning("Resource type " + res_name+ " is not in this site type!");
    //    return false;
    //}

    // NOT NECESSARY since fixed nodes will never be placed.
    //if(np->isFixed()) {
    //    sm8log::warning("Cannot change location of fixed node: " + np->getName());
    //    return false;
    //}

    // check if there is enough space in this site, return false if not
    if(used_resource_counts[res_name] >= type->site_resource_counts[res_name]) {
        // site has max number of this type already assigned, throw warning
        //sm8log::info("Site " + type->getName() + " (" + to_string(coord.first) + ", " + to_string(coord.second) + ")" 
        //    + " cannot add another Node of type " + np->getType()->getName() + " to Resource " + re_names
        //    + "(used " + to_string(used_resource_counts[res_name]) + " of " + to_string(type->getSiteResourceCounts()[res_name]) + ")" );
        return false;
    }

    //remove Node from old site
    if(np->getSite() != NULL) {
        np->getSite()->removeNode(np);
    }
    
    //add Node to this site
    np->setSite(this);
    np->setResourceNum(used_resource_counts[res_name]++);
    np->setCoord(coord);
    placed_nodes.push_back(np);
    return true;
}

bool Site::removeNode(Node* np)
// Return false if the node cannot be removed (probably because it hasn't been placed yet)
{
    placed_nodes.erase(placed_nodes.begin() + np->getResourceNum());
    used_resource_counts[np->getResource()->getName()]--;

    return true;
}
