#pragma once

#include "grid.h"

class Node 
{
    private:
        static int nodeCount;
        int nodeId;

        Node* parentNode;
        Grid gridState;
        int xPos;
        int yPos;
        Direction moveDir;

    public:
        Node(Grid gridState, Node* parentNode, int xPos, int yPos, Direction moveDir); // Standard constructor
        ~Node(); // Destructor 

        int getNodeId() const;
        Node* getParentNode();
        Grid getGrid();
        int getXPos() const;
        int getYPos() const;
        Direction getMoveDir();
        std::string getMoveDirString();
        bool isGoalState(int xPos, int yPos);
};

