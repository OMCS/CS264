#include <vector>
#include <iostream>
#include "grid.h"

Grid::Grid()
{
    /* Set the initial gridContents vector to the hard-coded values
     * y = -1, x = 5 is equivalent to gridContents[10][16]
     * Access is [row][col] 
     */
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

/* Constructor to specify contents of a new grid */
Grid::Grid(std::vector <std::vector <int> > newGrid)
{
   gridContents = newGrid; 
}

std::vector <std::vector <int> > Grid::getContents()
{
	return gridContents;
}

void Grid::setGrid(std::vector <std::vector <int> > newGrid)
{
    gridContents = newGrid;
}

/* Returns number of rows, to find number of cells multiply by two */
int Grid::getSize()
{
    return gridContents.size(); 
}

/* This function maps coordinates of the form x,y to row,column
 * this is used for manipulating the internal array representation of the world 
 * e.g. to detect if a given goal position contains an obstacle
 */
std::pair<int,int> Grid::mapToGridArray(int xPos, int yPos)
{
    std::pair<int,int> arrayPosition;

    arrayPosition.first = 9 - yPos ; // The row
    arrayPosition.second = 10 + xPos; // The column

    return arrayPosition; // Returns the 2D vector indices of the given x,y coordinate
}

/* This function returns true if a given coordinate contains an obstacle.
 * Currently used for input validation but in the future could be used for
 * path planning to detect invalid nodes in a search tree
 */
bool Grid::isObstacle(int xPos, int yPos)
{
    std::pair<int,int> gridCell = mapToGridArray(xPos, yPos);

    if (gridContents[gridCell.first][gridCell.second] == 1)
    {
        return true;
    }

    return false;
}

/* This function returns a boolean expression which indicates whether a robot can move in a certain direction */
bool Grid::canMove(int xPos, int yPos, Direction moveDir)
{
    std::pair<int,int> desiredCell = mapToGridArray(xPos, yPos);

    switch (moveDir)
    {
        case UP:
            if (gridContents[desiredCell.first - 1][desiredCell.second] == 1)
            {
                return false;
            }
            break;
        case DOWN:
            if (gridContents[desiredCell.first + 1][desiredCell.second] == 1)
            {
                return false;
            }
            break;
        case LEFT:
            if (gridContents[desiredCell.first][desiredCell.second - 1] == 1)
            {
                return false;
            }
            break;
        case RIGHT:
            if (gridContents[desiredCell.first][desiredCell.second + 1] == 1)
            {
                return false;
            }
            break;
        default:
            return false;
    }

    return true;
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

