#include <iostream>
#include <libplayerc++/playerc++.h>
#include <utility>
#include <vector>
#include <queue>
#include "grid.h"

using namespace PlayerCc;


// FIXME: Get these on stdin
int endX = 8;
int endY = 8;
    
Grid occupancyGrid;

// TODO: Come up with successor function
void BFS()
{

std::queue<Grid> frontier;
std::vector<Grid> visited; 

}

bool isGoal(int curXPos, int curYPos)
{
	if(curXPos == endX && curYPos == endY)
	{
		return true;
	}
	
	return false;			
}

std::pair<int,int> getRobotPos()
{
	std::pair<int,int> robotPos;
	
	for(int posX = 0; posX < 20; posX++)
	{
		for(int posY = 0; posY < 20; posY++)
		{
            std::vector< std::vector <int> > contents = occupancyGrid.getContents();
			
			if(contents[posX][posY] == 2)
			{
				robotPos.first = posX;
				robotPos.second = posY;
			}
		}
	}
	
	return robotPos;
}

int main(int argc, char *argv[])
{
	PlayerClient    robot("localhost");
	RangerProxy      sp(&robot,0);
	Position2dProxy pp(&robot,0);

	pp.SetMotorEnable(true);

    pp.SetSpeed(0.200,30);
    
    bool atGoal = false;

	while(!atGoal)
	{
        // TODO: Populate this

        if (isGoal(pp.GetXPos(), pp.GetYPos()))
        {
            atGoal = true;
            break;
        } 
	} 
}

