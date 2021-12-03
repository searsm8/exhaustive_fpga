#pragma once
#include "common.h"
#include "sitetype.h"
#include "resource.h"
#include "node.h"
using namespace std;


class Site
{
    private:
    public:
    Coord coord;
    SiteType* type;
    map<string, int> used_resource_counts;
    vector<Node*> placed_nodes;

    Site();
    Site(Coord coord_, SiteType* type_);
    
    bool placeNode(Node* node, string resource_to_use);
    void removeNode(Node* node);
};