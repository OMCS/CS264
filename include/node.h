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
        Node(Node* parentNode);

        int getNodeId() const;
        Node* getParentNode();
        Grid getGrid();
        Direction getMoveDir();
        std::string getMoveDirString();
        bool isGoalState(int xPos, int yPos);
};

inline bool operator == (const Node &n1, const Node &n2) {return n1.getNodeId() == n2.getNodeId();}

inline bool operator != (const Node &n1, const Node &n2) {return n1.getNodeId() != n2.getNodeId();}

