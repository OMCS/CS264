#pragma once

#include <vector>

static const int GRID_MIN_X = -10;
static const int GRID_MAX_X = 9;

static const int GRID_MIN_Y = -9;
static const int GRID_MAX_Y = 9;

class Grid
{
	private:
        std::vector < std::vector <int> > gridContents;
        static const int GRID_SIZE = 20;
	public:
        Grid();
        Grid(std::vector <std::vector <int> > newGrid);
        std::vector < std::vector <int> > getContents();
		void setGrid(std::vector <std::vector <int> > newGrid);
        int getSize();
        std::pair<int,int> mapToGridArray(int xPos, int yPos);
        bool isObstacle(int xPos, int yPos); 
        void printGrid();
};

