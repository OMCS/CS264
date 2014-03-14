#pragma once

#include <vector>

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
        void printGrid();
        int getSize();
        bool isObstacle(int theRow, int theCol);
        bool canMoveUp(int curRow, int curCol);
        bool canMoveDown(int curRow, int curCol);
};

