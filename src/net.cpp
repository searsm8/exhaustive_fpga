#include "net.h"
#include <limits.h>


Net::Net(string name_)
{
    name = name_;
    x_bound = 0;
    y_bound = 0;
}

Net::Net(string name_, vector<Node*> _nodes)
{ 
    name = name_;
    nodes = _nodes; 
    x_bound = 0;
    y_bound = 0;
}


void Net::addNode(Node* node)
{
    nodes.push_back(node);
}

string Net::getName()
{ return name; }

vector<Node*> Net::getNodes()
{ return nodes; }


int Net::updateXBound()
{
    int min_x = INT_MAX, max_x = 0;
    for(Node* node : nodes)
    {
        int x = node->getCoord().first;
        if(x < min_x)
            min_x = x;
        if(x > max_x)
            max_x = x;
    }
    x_bound = max_x - min_x;
    return x_bound;
}

int Net::updateYBound()
{
    int min_y = INT_MAX, max_y = 0;

    for(Node* node : nodes)
    {
        int y = node->getCoord().second;
        if(y < min_y)
            min_y = y;
        if(y > max_y)
            max_y = y;
    }
    y_bound = max_y - min_y;
    return y_bound;
}

/* For this net, compute the Half-Perimeter Wirelength estimate*/
int Net::updateHPWL()
{
    return updateXBound() + updateYBound();
}

int Net::getXBound() { return x_bound; }

int Net::getYBound() { return y_bound; }

int Net::getHPWL() { return x_bound + y_bound; }
