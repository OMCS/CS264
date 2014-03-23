#include <vector>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include "../include/grid.h"

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
 *
 * XXX: In future this should map doubles to integers using the round() function
 */
std::pair<int,int> Grid::mapToGridArray(int xPos, int yPos)
{
    std::pair<int,int> arrayPosition;

    arrayPosition.first = 9 - yPos ; // The row
    arrayPosition.second = 10 + xPos; // The column

    return arrayPosition; // Returns the 2D vector indices of the given x,y coordinate
}

/* This function returns true if a given coordinate contains an obstacle
 * can be used for input validation and path planning
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

/* This function moves the robot in the internal representation of the grid, this must be kept synchronised with actual robot movement
 * returns the new position in grid coordinates 
 */
std::pair<int,int> Grid::moveRobot(int curXPos, int curYPos, Direction moveDir)
{
    std::pair<int,int> curPositionVector = Grid::mapToGridArray(curXPos, curYPos); // Convert to get the correct [row][col] indices
    std::pair<int,int> newPositionGrid; // Pair to store the new grid coordinates in x,y format

    switch (moveDir)
    {
        case UP:
            gridContents[curPositionVector.first - 1][curPositionVector.second] = 2; // The robot's new position is represented with a 2
            gridContents[curPositionVector.first][curPositionVector.second] =  0;

            /* Update the position in terms of grid coordinates
             * the robot will move in the simulator according to these values
             */
            newPositionGrid.first = curXPos;
            newPositionGrid.second = curYPos + 1;

            break;
        case DOWN:
            gridContents[curPositionVector.first + 1][curPositionVector.second] = 2;

            newPositionGrid.first = curXPos;
            newPositionGrid.second = curYPos - 1;

            break;
        case LEFT:
            gridContents[curPositionVector.first][curPositionVector.second - 1] = 2;

            newPositionGrid.first = curXPos - 1;
            newPositionGrid.second = curYPos;

            break;
        case RIGHT:
            gridContents[curPositionVector.first][curPositionVector.second + 1] = 2;

            newPositionGrid.first = curXPos + 1;
            newPositionGrid.second = curYPos;

            break;
        case NONE:
            throw std::invalid_argument ("No move direction provided"); // Throw exception due to logic error
            break;
    }

    gridContents[curPositionVector.first][curPositionVector.second] = 0; // Replace the old position with a 0 (empty)

    return newPositionGrid; // Return the new position, used in the command to move the robot
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

