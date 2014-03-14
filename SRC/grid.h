#pragma once

#include <vector>

class Grid
{
	private:
        std::vector < std::vector <int> > gridContents;
        static const int GRID_SIZE = 20;
	public:
        std::vector < std::vector <int> > getContents();
		void setGrid(std::vector <std::vector <int> > newGrid);
};

