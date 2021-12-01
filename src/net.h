#pragma once
#include "common.h"
#include "node.h"
using namespace std;

// A Net connects cells together
class Net
{
    private:
    vector<Node*> nodes;

    public:
    Net(vector<Node*> _nodes);

    vector<Node*> getNodes();

    int computeHPWL();
};