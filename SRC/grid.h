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
        int getSize();
        std::pair<int,int> mapToGridArray(int xPos, int yPos);
        bool isObstacle(int xPos, int yPos); 
        void printGrid();
};

