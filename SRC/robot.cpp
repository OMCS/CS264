#include <iostream>
#include <libplayerc++/playerc++.h>
#include <vector>
#include <queue>
#include <cmath>
#include <unistd.h>
#include "grid.h"

using namespace PlayerCc;

Grid occupancyGrid;

double goalX, goalY;

/* XXX: To be completed in the future when using deliberative path-finding */
player_pose2d_t BFS()
{
    std::queue<Grid> frontier;
    std::vector<Grid> visited; 

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

int main(int argc, char *argv[])
{
    getUserInput();

	PlayerClient    robot("localhost");
	RangerProxy      sp(&robot,0);
	Position2dProxy pp(&robot,0);

    double turnrate;
    pp.SetMotorEnable(false);
    
    robot.Read();

    bool atGoal = false;
    player_pose2d_t newPos = BFS();

    pp.SetMotorEnable(true);

    /* The main loop that while continue until the robot has reached its destination */
	while(!atGoal)
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
            usleep(200000); // Move backwards for 200ms to get a sufficient distance from obstacles
		}

        else 
        {
            pp.SetSpeed(0.300, turnrate); // Standard speed and turn rate
        }

        usleep(150000); // Continue obstacle avoidance behaviour for 150ms

        pp.GoTo(newPos); // Seek goal position

        if (distanceFromGoal(pp.GetXPos(), pp.GetYPos()) < 0.30) // Combined distance from goal
        {
            usleep(200000); // Seek goal position for 200ms (makes movement around goal more accurate)
        }

        else
        {
            usleep(120000); // Continue seeking goal position for 120ms
        }

        if (isGoal(pp.GetXPos(), pp.GetYPos()))
        {
            atGoal = true;
            std::cout << "Goal reached!" << std::endl;
        } 
	} 

    return(0);
}

