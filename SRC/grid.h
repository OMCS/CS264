#pragma once

#include <vector>

class Grid
{
	private:
        std::vector < std::vector <int> > gridContents;
	public:
        std::vector < std::vector <int> > getContents();
		void setGrid(std::vector <std::vector <int> > newGrid);
};

