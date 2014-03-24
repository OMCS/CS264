#include <iostream>
#include <libplayerc++/playerc++.h>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
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

            /* Move the robot within the grid representation, can 'replay' these later */
            std::pair<int,int> newCoordinates = newGridState.moveRobot(curXPos, curYPos, moveDir); 

            successorList.push_back(Node (newGridState, &parentNode, moveDir)); // moveDir is defined in grid.h as an enumerated value

            // FIXME: Do these even matter, don't we need to change the ones kept in BFS?
            curXPos = newCoordinates.first; 
            curYPos = newCoordinates.second;
        }
    }

    return successorList;
}

void displayPath(Node currentNode)
{
    std::string pathString;

    std::cout << "Start Grid" << std::endl;
    //std::cout << rootNode.getNodeId() << std::endl;
    rootNode.getGrid().printGrid();

    std::cout << "\n\nEnd Grid" << std::endl;
    //std::cout << currentNode.getNodeId() << std::endl;
    currentNode.getGrid().printGrid();

    for (Node n : explored)
    {
        if (n.isGoalState(goalX, goalY)) // Found the node which represents the goal state
        {
            while (n.getNodeId() != rootNode.getNodeId()) // Traverse backwards until we reach the root node
            {
                pathString.append(n.getMoveDirString() + " ");
                n = Node(n.getParentNode()); // Move up the tree one node
            }
        }
    }

    std::cout << "\n\nPATH: " << pathString << std::endl;
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
            if (std::find(explored.begin(), explored.end(), successorNode) == explored.end())
            {
                frontier.push(successorNode);
            }

            if (successorNode.isGoalState(goalX, goalY))
            {
                pathFound = true;
                explored.push_back(successorNode);
                break; // Break out of this inner for loop
            }
        } 

        currentNode = frontier.front(); // Set new current node to the node at the front of the queue 
        frontier.pop(); // Remove element from front of queue

        if (pathFound) 
        {
            break; // Finished searching
        }
    }

    displayPath(currentNode); // Display the solution
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
	/*while(!isGoal(pp.GetXPos(), pp.GetYPos()))
	{
        robot.Read(); // Update position data

        // FIXME: Move to path found in search results sequentially one square at a time
	}*/
}

