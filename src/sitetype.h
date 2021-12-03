#pragma once
#include "common.h"

class SiteType
{
    private:
    string name;
    map<string, int> site_resource_counts; 

    public:
    SiteType();
    SiteType(string name_);

    string getName();
    map<string, int> getSiteResourceCounts();
    void addResource(string, int count);

};