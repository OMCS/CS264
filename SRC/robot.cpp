#include <iostream>
#include <libplayerc++/playerc++.h>
#include <vector>
#include <queue>
#include <cmath>
#include "grid.h"
#include <unistd.h>

using namespace PlayerCc;

Grid occupancyGrid;

double goalX, goalY;

/* TODO: Come up with successor function and complete this */
player_pose2d_t BFS()
{
    std::queue<Grid> frontier;
    std::vector<Grid> visited; 

    player_pose2d_t newPos;

    // FIXME: This should be the next section of the path
    newPos.px = goalX;
    newPos.py = goalY;

    return newPos;
}

bool isGoal(double curXPos, double curYPos) 
{
	if(std::abs(curXPos - goalX) < 0.15 && std::abs(curYPos - goalY) < 0.15)
    {
		return true;
	}

	return false;			
}

int main(int argc, char *argv[])
{
	PlayerClient    robot("localhost");
	RangerProxy      sp(&robot,0);
	Position2dProxy pp(&robot,0);

    double turnrate;

    pp.SetMotorEnable(false);
    
    robot.Read();

    std::cout << "Goal Position (X): ";
    std::cin >> goalX;
    std::cout << "Goal Position (Y): ";
    std::cin >> goalY;

	pp.SetMotorEnable(true);

    bool atGoal = false;
    player_pose2d_t newPos = BFS();

   /* TODO: Finish this loop */
	while(!atGoal)
	{
        robot.Read(); // Update position data

        if ( (sp[0] + sp[1] + sp[2]) < (sp[5] + sp[6] + sp[7]) ) 
        //if ( (sp[0] + sp[1] ) < (sp[5] + sp[6]) ) 
        {
			turnrate = dtor(-60); // Experimental Turn rates
		} 

        else 
        {
			turnrate = dtor(30);
		}

		if (sp[3] < 0.6 || sp[4] < 0.6) 
        {
			pp.SetSpeed(-0.700, turnrate); // Move backwards
		}

        else 
        {
            pp.SetSpeed(0.300, turnrate);
        }

        usleep(150000);

        pp.GoTo(newPos);

        usleep(100000);

        //pp.SetSpeed(speed, turnrate);

        if (isGoal(pp.GetXPos(), pp.GetYPos()))
        {
            atGoal = true;
            std::cout << "Goal reached!" << std::endl;
        } 
	} 

    return(0);
}

