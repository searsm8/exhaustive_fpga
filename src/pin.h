#pragma once
#include "common.h"
using namespace std;

class Pin
{
    private:
    string name;
    string type;

    public:
    Pin(string name_, string type_);    
    string getName();
    string getType();
};