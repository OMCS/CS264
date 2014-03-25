#include <iostream>
#include <libplayerc++/playerc++.h>
#include <vector>
#include <queue> 
#include <functional>
#include <algorithm>
#include <unistd.h>
#include "../include/grid.h"
#include "../include/node.h"

using namespace PlayerCc;

double goalX, goalY; // Store goal position

/* This function returns the absolute distance to the goal using grid coordinates, this could be used in a path cost function */
const double distanceFromGoal(double curXPos, double curYPos) 
{
    return std::abs(curXPos - goalX) + std::abs(curYPos - goalY);
}

struct findClosestNode : public std::binary_function<Node*, Node*, bool>
{
    bool operator() (const Node* firstNode, const Node* secondNode) const
    {
        // Return 'true' if firstNode is closer than secondNode
        if (distanceFromGoal(firstNode->getXPos(),firstNode->getYPos()) < distanceFromGoal(secondNode->getXPos(), secondNode->getYPos()))
        {
            return false;
        }

        else
        {
            return true;
        }
    }
};

/* Variables for graph searching */
Grid occupancyGrid; // Default grid state
std::priority_queue<Node*, std::vector<Node*>, findClosestNode> frontier; // Holds unexpanded child nodes
std::vector<Node*> explored;  // Holds explored nodes

std::vector<Node*> spawnSuccessors(Node* parentNode, int curXPos, int curYPos)
{
    std::vector<Node*> successorList;

    Grid parentGridState = parentNode->getGrid();

    for (int DIRECTION = UP; DIRECTION < NONE; DIRECTION++)
    {
        Direction moveDir = static_cast<Direction>(DIRECTION); // Cast the integer to its corresponding enum value

        if (parentGridState.canMove(parentNode->getXPos(), parentNode->getYPos(), moveDir))
        {
            Grid newGridState(parentGridState.getContents()); 

            /* Move the robot within the grid representation, can 'replay' these later
             * will return the new position of the robot within the grid
             */
            std::pair<int,int> newCoordinates = newGridState.moveRobot(parentNode->getXPos(), parentNode->getYPos(), moveDir); // newCoordinates.first = x, second = y

            successorList.push_back(new Node (newGridState, parentNode, newCoordinates.first, newCoordinates.second, moveDir)); // moveDir is defined in grid.h as an enumerated value
        }
    }

    return successorList;
}

void displayPath(Node* rootNode, Node* currentNode)
{
    std::string pathString;

    std::cout << "Start Grid" << std::endl;
    rootNode->getGrid().printGrid();

    std::cout << "\n\nEnd Grid" << std::endl;
    currentNode->getGrid().printGrid();

    for (Node* n : explored)
    {
        if (n->isGoalState(goalX, goalY)) // Found the node which represents the goal state
        {
            while (n->getParentNode() != NULL) // Traverse upwards until we reach the root node 
            {
                pathString.append(n->getMoveDirString() + " "); // Append the direction to the string
                n = n->getParentNode(); // Move one level back up the tree
            }
        }
    }

    std::cout << "\n\nPATH: " << pathString << std::endl;
}

void BestFirstSearch(int curXPos, int curYPos)
{
    Node* rootNode = new Node(occupancyGrid, NULL, curXPos, curYPos, NONE); // Create the root node, the parent node is set to NULL
    Node* currentNode = rootNode;

    bool pathFound = false;

    while (!currentNode->isGoalState(goalX, goalY)) 
    {
        explored.push_back(currentNode);

        std::vector<Node*> successors = spawnSuccessors(currentNode, currentNode->getXPos(), currentNode->getYPos()); 

        for (Node* successorNode : successors)
        {
            if (std::find(explored.begin(), explored.end(), successorNode) == explored.end())
            {
                frontier.push(successorNode);
            }

            if (successorNode->isGoalState(goalX, goalY))
            {
                pathFound = true;
                explored.push_back(successorNode);
                break; // Break out of this inner for loop
            }
        } 

        currentNode = frontier.top(); // Set new current node to the node at the front of the queue 
        frontier.pop(); // Remove element from top of queue

        if (pathFound) 
        {
            break; // Finished searching
        }
    }

    displayPath(rootNode, explored.at(explored.size() - 1)); // Display the solution from the rootNode to the goal node (last node added to the explored list)
}

bool isGoal(double curXPos, double curYPos) 
{
    return (distanceFromGoal(curXPos, curYPos) < 0.10); // If combined distance from goal is a negligible value
}

void getUserInput()
{
     bool validInput = false;

    while (!validInput)
    {
        std::cout << "Goal Position (X): ";
        std::cin >> goalX;
        if (std::cin.fail() || goalX < GRID_MIN_Y || goalX > GRID_MAX_X)
        {
            std::cin.clear(); std::cin.ignore();
            std::cerr << "\n\nInvalid goal position. Input must be numerical and a valid grid position\n\n";
            continue;
        }

        std::cout << "Goal Position (Y): ";
        std::cin >> goalY;

        if (std::cin.fail() || goalY < GRID_MIN_Y || goalY > GRID_MAX_Y || occupancyGrid.isObstacle(goalX, goalY))
        {
            std::cin.clear(); std::cin.ignore();
            std::cerr << "\nInvalid goal position. Input must be numerical, a valid grid position and not an obstacle\n\n";
            continue;
        }

        validInput = true;
    }
}

/* This function continues executing the previous command passed to the robot. 
 * It then checks to see if the goal state has been reached before 
 * Allowing the next function to execute
 */
void continueNavigating(int delayMicroseconds, Position2dProxy &pp)
{
    usleep (delayMicroseconds);

    if (isGoal(pp.GetXPos(), pp.GetYPos()))
    {
                std::cout << "Goal reached!" << std::endl;
                std::exit(0);
    }
}

int main(int argc, char *argv[])
{
    getUserInput();

	PlayerClient    robot("localhost");
	RangerProxy      sp(&robot,0);
	Position2dProxy pp(&robot,0);

    pp.SetMotorEnable(false);
    
    robot.Read();

    pp.SetMotorEnable(true);

    BestFirstSearch(pp.GetXPos(), pp.GetYPos()); // Run a search to find a path to the goal position

    /* The main loop that while continue until the robot has reached its destination */
	/*while(!isGoal(pp.GetXPos(), pp.GetYPos()))
	{
        robot.Read(); // Update position data

        // FIXME: Move to path found in search results sequentially one square at a time, will need display to output a vector or similar
	}*/
}

