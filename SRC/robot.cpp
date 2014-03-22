#include <iostream>
#include <libplayerc++/playerc++.h>
#include <vector>
#include <queue>
#include <cmath>
#include <unistd.h>
#include "grid.h"
#include "node.h"

using namespace PlayerCc;

Grid occupancyGrid;
bool atGoal = false;

double goalX, goalY;

std::vector<Node> spawnSuccessors(Node parentNode)
{
    std::vector<Node> successorList;

    Grid parentGridState = parentNode.getGridState();

    // TODO: Continue here, need to map these correctly
    
    return successorList;
}

/* XXX: To be completed in the future when using deliberative path-finding */
player_pose2d_t BFS()
{
    std::queue<Node> frontier;
    std::vector<Node> explored; 

    Node rootNode = Node(occupancyGrid, NULL, NONE);
    Node currentNode = rootNode;

    bool pathingComplete = false;

    while (!currentNode.isGoalState(goalX, goalY))
    {
        explored.push_back(currentNode);

        std::vector<Node> successors = spawnSuccessors(currentNode);

        for(Node successorNode : successors)
        {
            frontier.push(successorNode);

            if (successorNode.isGoalState(goalX, goalY))
            {
                explored.push_back(currentNode);
                break;
            }
        } 

        currentNode = frontier.front(); // Set new current node to the node at the front of the queue 
        frontier.pop(); // Remove element from front of queue
    }

    player_pose2d_t newPos;

    // XXX: This should be the next section of the path
    newPos.px = goalX;
    newPos.py = goalY;

    return newPos;
}

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

    player_pose2d_t newPos = BFS();

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

