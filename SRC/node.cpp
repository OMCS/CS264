#include <string>
#include "grid.h"
#include "node.h"

int Node::nodeCount = 0;

Node::Node(Grid gridState, Node* parentNode, std::string moveDir)
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

std::string Node::getMoveDir()
{
    return this->moveDir;
}
