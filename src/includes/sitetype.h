#pragma once
#include "common.h"

class Site;

class SiteType
{
    private:
    public:
    string name;
    vector<Site*> site_list; // List of sites which fit into this SiteType
    map<string, int> site_resource_counts; 

    SiteType();
    SiteType(string name_);

    string getName();
    map<string, int> getSiteResourceCounts();
    void addResource(string, int count);
    bool hasResource(string);

};