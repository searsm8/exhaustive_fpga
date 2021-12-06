#pragma once
#include "common.h"

class SiteType
{
    private:
    public:
    string name;
    map<string, int> site_resource_counts; 

    SiteType();
    SiteType(string name_);

    string getName();
    map<string, int> getSiteResourceCounts();
    void addResource(string, int count);

};