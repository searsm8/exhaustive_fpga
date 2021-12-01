#include "pin.h"

Pin::Pin(string name_, string type_)
{
    name = name_;
    type = type_;
}

string Pin::getName()
{ return name; }

string Pin::getType()
{ return type; }