#pragma once

#include "grid.h"

class Node 
{
    private:
        static int nodeCount;
        int nodeId;

        Node* parentNode;
        Grid gridState;
        Direction moveDir;

    public:
        Node(Grid gridState, Node* parentNode, Direction moveDir);

        int getNodeId();
        Node* getParentNode();
        Grid getGrid();
        Direction getMoveDir();
        std::string getMoveDirString();
        bool isGoalState(int xPos, int yPos);
};

