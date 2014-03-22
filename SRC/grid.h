#pragma once

#include <vector>

/* CONSTANTS */
static const int GRID_MIN_X = -10;
static const int GRID_MAX_X = 9;

static const int GRID_MIN_Y = -9;
static const int GRID_MAX_Y = 9;

/* Enumerated value for directions, used in canMove() function */
enum Direction
{
    UP, DOWN, LEFT, RIGHT
};

class Grid
{
	private:
        std::vector < std::vector <int> > gridContents;
	public:
        Grid();
        Grid(std::vector <std::vector <int> > newGrid);
        std::vector < std::vector <int> > getContents();
		void setGrid(std::vector <std::vector <int> > newGrid);
        int getSize();
        std::pair<int,int> mapToGridArray(int xPos, int yPos);
        bool isObstacle(int xPos, int yPos); 
        bool canMove(int xPos, int yPos, Direction moveDir);
        void printGrid();
};

