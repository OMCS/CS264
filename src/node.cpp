#include <string>
#include "../include/grid.h"
#include "../include/node.h"

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

Grid Node::getGrid()
{
    return this->gridState;
}

Direction Node::getMoveDir()
{
    return this->moveDir;
}

std::string Node::getMoveDirString()
{
    std::string moveDirStr;

    switch (this->moveDir)
    {
        case UP:
            moveDirStr = "UP";
            break;
        case DOWN:
            moveDirStr = "DOWN";
            break;
        case LEFT:
            moveDirStr = "LEFT";
            break;
        case RIGHT:
            moveDirStr = "RIGHT";
            break;
        case NONE:
            moveDirStr = "NONE";
            break;
    }

    return moveDirStr;
}

bool Node::isGoalState(int xPos, int yPos)
{
    std::pair<int,int> goalCoordinates = Grid::mapToGridArray(xPos, yPos); // Map the grid coordinates to vector indices in the internal representation of the world

    return (gridState.getContents()[goalCoordinates.first][goalCoordinates.second] == '3'); // True if the robot is at goal, false otherwise
}


