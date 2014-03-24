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
        Node(Grid gridState, Node* parentNode, Direction moveDir); // Bare constructor
        Node(Grid gridState, Node* parentNode, int xPos, int yPos, Direction moveDir); // Standard constructor
        Node(Node* parentNode); // 'Copy' constructor

        int getNodeId() const;
        Node* getParentNode();
        Grid getGrid();
        int getXPos();
        int getYPos();
        Direction getMoveDir();
        std::string getMoveDirString();
        bool isGoalState(int xPos, int yPos);
};

inline bool operator == (const Node &n1, const Node &n2) {return n1.getNodeId() == n2.getNodeId();}

inline bool operator != (const Node &n1, const Node &n2) {return n1.getNodeId() != n2.getNodeId();}

