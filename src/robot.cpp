#include <iostream>
#include <libplayerc++/playerc++.h>
#include <vector>
#include <queue>
#include <cmath>
#include <unistd.h>
#include "../include/grid.h"
#include "../include/node.h"

using namespace PlayerCc;

Grid occupancyGrid; // Starting grid

/* Variables for graph searching */
std::queue<Node> frontier; // Holds unexpanded child nodes
std::vector<Node> explored;  // Holds explored nodes

Node rootNode = Node(occupancyGrid, NULL, NONE); // Create the root node, parent node is set to NULL
    
double goalX, goalY; // Store goal position

std::vector<Node> spawnSuccessors(Node parentNode, int curXPos, int curYPos)
{
    std::vector<Node> successorList;

    Grid parentGridState = parentNode.getGrid();

    for (int DIRECTION = UP; DIRECTION < NONE; DIRECTION++)
    {
        Direction moveDir = static_cast<Direction>(DIRECTION); // Cast the integer to its corresponding enum value

        if (parentGridState.canMove(curXPos, curYPos, moveDir))
        {
            Grid newGridState(parentGridState.getContents()); 

            newGridState.moveRobot(curXPos, curYPos, moveDir); // Move the robot within the grid representation, can 'replay' these later

            successorList.push_back(Node (newGridState, &parentNode, moveDir)); // moveDir is defined in grid.h as an enumerated value
        }
    }

    return successorList;
}

void displayPath(Node currentNode)
{
    std::string pathString;

    std::cout << "Start Grid" << std::endl;
    rootNode.getGrid().printGrid();

    std::cout << "End Grid" << std::endl;
    currentNode.getGrid().printGrid();

    for (Node n : explored)
    {
        if (n.isGoalState(goalX, goalY)) // Found the Node which represents the goal state
        {
            Node* ptrNode;
            while (ptrNode != &rootNode) // Compare memory addresses of n and rootNode
            {
                pathString.append(ptrNode->getMoveDirString() + ", ");
                ptrNode = ptrNode->getParentNode(); // Move one level back up the tree
            }
        }
    }

    std::cout << "Steps: " << pathString.length() << std::endl; 

    std::cout << std::string (pathString.rbegin(), pathString.rend()) << std::endl; // Print out path from beginning to end
}

void BFS(int curXPos, int curYPos)
{
    rootNode = Node(occupancyGrid, NULL, NONE);
    Node currentNode = rootNode;

    bool pathFound = false;

    while (!currentNode.isGoalState(goalX, goalY)) 
    {
        explored.push_back(currentNode);

        std::vector<Node> successors = spawnSuccessors(currentNode, curXPos, curYPos);

        for (Node successorNode : successors)
        {
            frontier.push(successorNode);

            if (successorNode.isGoalState(goalX, goalY))
            {
                pathFound = true;
                explored.push_back(currentNode);
                break; // Break out of this inner for loop
            }
        } 

        if (pathFound)
        {
            break; // Found a path to the goal state, exit the outer while loop before altering the currentNode
        }

        currentNode = frontier.front(); // Set new current node to the node at the front of the queue 
        frontier.pop(); // Remove element from front of queue
    }

    displayPath(currentNode);
}

/* This function returns the absolute distance to the goal using grid coordinates, this could be used in a path cost function */
double distanceFromGoal(double curXPos, double curYPos)
{
    return std::abs(curXPos - goalX) + std::abs(curYPos - goalY);
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
        if (goalX < GRID_MIN_Y || goalX > GRID_MAX_X)
        {
            std::cerr << "\nInvalid goal position. Must be on grid\n\n";
            continue;
        }

        std::cout << "Goal Position (Y): ";
        std::cin >> goalY;

        if (goalY < GRID_MIN_Y || goalY > GRID_MAX_Y || occupancyGrid.isObstacle(goalX, goalY))
        {
            std::cerr << "\nInvalid goal position. Must be on grid and not an obstacle\n\n";
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

    BFS(pp.GetXPos(), pp.GetYPos()); // Run a search to find a path to the goal position

    /* The main loop that while continue until the robot has reached its destination */
	while(!isGoal(pp.GetXPos(), pp.GetYPos()))
	{
        robot.Read(); // Update position data

        // FIXME: Move to path found in search results sequentially one square at a time
	} 
}

