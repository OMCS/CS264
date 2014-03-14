#include <vector>
#include <iostream>

class Grid {

	private:
        std::vector <std::vector <int> > gridContents;
	public:
        Grid();
        std::vector <std::vector <int> > getContents();
		void setGrid(std::vector <std::vector <int> > newGrid);
        void printGrid();
        int getSize();
        bool isObstacle(int theRow, int theCol);
        bool canMoveUp(int curRow, int curCol);
        bool canMoveDown(int curRow, int curCol);

        // Constructor to specify contents of a new grid
        Grid(std::vector <std::vector <int> > newGrid)
        {
           gridContents = newGrid; 
        }
};

Grid::Grid()
{
    // Set the initial gridContents vector to the hard-coded values
    std::vector <std::vector <int> > defaultValues = 
    {
        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0},
        {0,0,3,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0} 
    };

    gridContents = defaultValues;
}

std::vector <std::vector <int> > Grid::getContents()
{
	return gridContents;
}

void Grid::setGrid(std::vector <std::vector <int> > newGrid)
{
    gridContents = newGrid;
}

// Returns number of rows, to find true size multiply by two
int Grid::getSize()
{
    return gridContents.size(); 
}

void Grid::printGrid()
{
    for (int i = 0; i < gridContents.size(); i++)
    {
        for (int j = 0; j < gridContents.size(); j++)
        {

            if (j % gridContents.size() == 0)
            {
                std::cout << "\n";
            }

            std::cout << gridContents[i][j];
        }
    }
}

/* TODO: Test booleans before continuing */
bool Grid::canMoveUp(int curRow, int curCol)
{
    // If move is off the Grid
    if (curRow > 10 || curRow < -10 || curCol > 10 || curCol < -10)
    {
        return false;
    }

    else if (gridContents[curRow + 11][curCol +10] == 1) // Obstacles
    {
        return false;
    }

    return true;
}

bool Grid::canMoveDown(int curRow, int curCol)
{
    // If move is off the Grid
    if (curRow > 10 || curRow < -10 || curCol > 10 || curCol < -10)
    {
        return false;
    }

    else if (gridContents[curRow + 9][curCol + 10] == 1) // Obstacles
    {
        return false;
    }

    return true;
}

