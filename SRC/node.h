#pragma once

#include <string>
#include "grid.h"

class Node 
{
    private:
        static int nodeCount;
        int nodeId;

        Node* parentNode;
        Grid gridState;
        std::string moveDir;

    public:
        Node(Grid gridState, Node* parentNode, std::string moveDir);

        int getNodeId();
        Node* getParentNode();
        Grid getGridState();
        std::string getMoveDir();
};

