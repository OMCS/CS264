#include <string>
#include "../include/grid.h"
#include "../include/node.h"

int Node::nodeCount = 0;

/* Constructor allows setting grid state, parent node, current path cost, current position and the direction the move was in */
Node::Node(Grid gridState, Node* parentNode, int pathCost, int xPos, int yPos, Direction moveDir)
{ 
    this->nodeId = nodeCount++;
    this->gridState = gridState;
    this->parentNode = parentNode;
    this->moveDir = moveDir; 
    this->xPos = xPos;
    this->yPos = yPos; 

    /* Give nodes that are adjacent to obstacles a lower priority in the list */
    if (this->gridState.adjacentToObstacle(this->xPos,this->yPos))
    {
        this->pathCost = pathCost + 5;
    }

    else
    {
        this->pathCost = pathCost + 1;
    }
}

Node::~Node()
{
    delete parentNode;
}

int Node::getNodeId() const
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

int Node::getPathCost() const
{
    return this->pathCost;
}

int Node::getXPos() const
{
    return this->xPos;
}

int Node:: getYPos() const
{
    return this->yPos;
}

Direction Node::getMoveDir()
{
    return this->moveDir;
}

/* This function returns the instance move direction as a string for output purposes */
std::string Node::getMoveDirString()
{
    std::string moveDirStr = "";
    
    switch (this->moveDir)
    {
        case UP:
            moveDirStr = "U";
            break;
        case DOWN:
            moveDirStr = "D";
            break;
        case LEFT:
            moveDirStr = "L";
            break;
        case RIGHT:
            moveDirStr = "R";
            break;
        case NONE:
            break;
    }

    return moveDirStr;
}

bool Node::isGoalState(int xPos, int yPos)
{
    std::pair<int,int> goalCoordinates = Grid::mapToGridArray(xPos, yPos); // Map the grid coordinates to vector indices in the internal representation of the world

    return (gridState.getContents()[goalCoordinates.first][goalCoordinates.second] == 2); // True if the robot is at goal, false otherwise
}


