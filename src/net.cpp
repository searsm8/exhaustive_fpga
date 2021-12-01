#include "net.h"
#include <limits.h>



Net::Net(vector<Node*> _nodes)
{ nodes = _nodes; }


vector<Node*> Net::getNodes()
{ return nodes; }

/* For this net, compute the Half-Perimeter Wirelength estimate
*/
int Net::computeHPWL()
{
    int min_x = INT_MAX, max_x = 0;
    int min_y = INT_MAX, max_y = 0;

    for(Node* node : nodes)
    {
        Coord coord = node->getCoord();
        int x = coord.first;
        int y = coord.second;
        if(x < min_x)
            min_x = x;
        if(x > max_x)
            max_x = x;
        if(y < min_y)
            min_y = y;
        if(y > max_y)
            max_y = y;
    }
        
    int hpwl = (max_x - min_x) + (max_y - min_y);
    return hpwl;
}