#include <iostream>
#include <libplayerc++/playerc++.h>
#include <utility>
#include <vector>
#include <queue>
#include "grid.h"

using namespace PlayerCc;

Grid occupancyGrid;

int goalX, goalY;

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

bool isGoal(int curXPos, int curYPos) // Cast to int 
{
	if(curXPos == goalX && curYPos == goalY)
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

    std::cout << "Goal Position (X): ";
    std::cin >> goalX;
    std::cout << "Goal Position (Y): ";
    std::cin >> goalY;

	pp.SetMotorEnable(true);

    pp.SetSpeed(0.200,30);
    
    bool atGoal = false;

    /* TODO: Finish this loop */
	while(!atGoal)
	{
        robot.Read(); // Update position data
        
        player_pose2d_t newPos = BFS();

        pp.GoTo(newPos);

        if (isGoal(pp.GetXPos(), pp.GetYPos()))
        {
            atGoal = true;
            std::cout << "Goal reached!" << std::endl;
            break;
        } 
	} 

    return(0);
}

