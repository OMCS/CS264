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
std::queue<Node> frontier;
std::vector<Node> explored; 

double goalX, goalY; // Store goal position

// FIXME: This needs to be completed and debugged 
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

            newGridState.moveRobot(curXPos, curYPos, moveDir); // FIXME: Move the robot within the vector - will also need to move it in the sim

            successorList.push_back(Node (newGridState, &parentNode, moveDir)); // moveDir is defined in grid.h as an enumerated value
        }
    }

    return successorList;
}

// FIXME: Make sure this works properly
void BFS(int curXPos, int curYPos)
{
    Node rootNode = Node(occupancyGrid, NULL, NONE);
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
}

/* This function returns the absolute distance to the goal using grid coordinates, this could be used in a path cost function */
double distanceFromGoal(double curXPos, double curYPos)
{
    return std::abs(curXPos - goalX) + std::abs(curYPos - goalY);
}

bool isGoal(double curXPos, double curYPos) 
{
    if (distanceFromGoal(curXPos, curYPos) < 0.10) // If combined distance from goal is a negligible value
    {
		return true;
	}

	return false;			
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

    double turnrate;
    pp.SetMotorEnable(false);
    
    robot.Read();

    /* FIXME: These 3 lines are debug code */ 
    player_pose2d_t newPos;

    newPos.px = goalX;
    newPos.py = goalY; 

    pp.SetMotorEnable(true);

    /* The main loop that while continue until the robot has reached its destination */
	while(true)
	{
        robot.Read(); // Update position data

        if ( (sp[0] + sp[1] + sp[2]) < (sp[5] + sp[6] + sp[7]) ) 
        {
			turnrate = dtor(-60); // Turn rates decided after experimentation
		} 

        else 
        {
			turnrate = dtor(40);
		}

		if (sp[3] < 0.5 || sp[4] < 0.5) 
        {
			pp.SetSpeed(-0.700, turnrate); // Move backwards
            continueNavigating(200000, pp); // Check if goal is reached immediately after sleeping and before receiving new command
		}

        else 
        {
            pp.SetSpeed(0.300, turnrate); // Standard speed and turn rate
        }

        continueNavigating(150000, pp); // Continue obstacle avoidance behaviour for 150ms

        if (distanceFromGoal(pp.GetXPos(), pp.GetYPos()) < 0.30) // Combined distance from goal
        {
            pp.GoTo(newPos); // Seek goal position...
            continueNavigating(200000, pp); // for 200ms (makes movement around goal more accurate)
        }

        else
        {
            pp.GoTo(newPos); // Seek goal position...
            continueNavigating(120000, pp); // for 120ms
        }
	} 
}

