#pragma once
#include "common.h"
#include "node.h"
using namespace std;

// A Net connects cells together
class Net
{
    private:
    string name;
    vector<Node*> nodes;
    int x_bound;
    int y_bound;

    public:
    Net(string name_);
    Net(string name_, vector<Node*> _nodes);

    void addNode(Node* node);
    vector<Node*> getNodes();
    string getName();

    int updateXBound();
    int updateYBound();
    int updateHPWL();

    int getXBound();
    int getYBound();
    int getHPWL();
};