#include "grid.h"
#include "node.h"

int Node::nodeCount = 0;

Node::Node(Grid gridState, Node* parentNode, Direction moveDir)
{
    this->nodeId = nodeCount++;
    this->gridState = gridState;
    this->parentNode = parentNode;
    this->moveDir = moveDir; 
}

int Node::getNodeId()
{
    return this->nodeId;
}

Node* Node::getParentNode()
{
    return this->parentNode;
}

Grid Node::getGridState()
{
    return this->gridState;
}

Direction Node::getMoveDir()
{
    return this->moveDir;
}

bool Node::isGoalState(double curX, double curY)
{
    std::pair<int,int> goalCoordinates = gridState.mapToGridArray(curX, curY); // Map the grid coordinates to vector indices in the internal representation of the world

    if (gridState.getContents()[goalCoordinates.first][goalCoordinates.second] == '3') // If the robot has entered the goal state...
    {
        return true;   
    }

    return false;
}
