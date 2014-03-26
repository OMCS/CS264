#include <iostream> // std::cout
#include <libplayerc++/playerc++.h> // Player/Stage Functions
#include <vector> 
#include <queue> 
#include <set>
#include "../include/grid.h"
#include "../include/node.h"

using namespace PlayerCc;

double goalX, goalY; // Store goal position

/* This function returns the absolute distance to the goal using grid coordinates, measure of path cost */
const double distanceFromGoal(double curXPos, double curYPos) 
{
    return std::abs(curXPos - goalX) + std::abs(curYPos - goalY);
}

/* This operator determines which of two given nodes is closer to the goal position, dereferences the Node* pointers
 * used to order the priority queue for the best-first search
 */
struct findClosestNode : public std::binary_function<Node*, Node*, bool>
{
    bool operator() (const Node* firstNode, const Node* secondNode) const
    {
        /* Return 'true' if firstNode is closer than secondNode */
        if (distanceFromGoal(firstNode->getXPos(),firstNode->getYPos()) < distanceFromGoal(secondNode->getXPos(), secondNode->getYPos()))
        {
            return false;
        }

        else
        {
            return true;
        }
    }
};

/* Variables for graph searching */
Grid occupancyGrid; // Default grid state
std::priority_queue<Node*, std::vector<Node*>, findClosestNode> frontier; // Holds unexpanded child nodes
std::vector<Node*> explored;  // Holds explored nodes

std::vector<Node*> spawnSuccessors(Node* parentNode, int curXPos, int curYPos)
{
    std::vector<Node*> successorList;

    Grid parentGridState = parentNode->getGrid();

    for (int DIRECTION = UP; DIRECTION < NONE; DIRECTION++)
    {
        Direction moveDir = static_cast<Direction>(DIRECTION); // Cast the integer to its corresponding enum value

        if (parentGridState.canMove(parentNode->getXPos(), parentNode->getYPos(), moveDir))
        {
            Grid newGridState(parentGridState.getContents()); 

            /* Move the robot within the grid representation, can 'replay' these later
             * will return the new position of the robot within the grid
             */
            std::pair<int,int> newCoordinates = newGridState.moveRobot(parentNode->getXPos(), parentNode->getYPos(), moveDir); // newCoordinates.first = x, second = y

            successorList.push_back(new Node (newGridState, parentNode, newCoordinates.first, newCoordinates.second, moveDir)); // moveDir is defined in grid.h as an enumerated value
        }
    }

    return successorList;
}

/* This function returns true if a node with the same x and y position already exists in the explored list */
bool nodeExplored(Node* successorNode)
{
    std::vector<Node*>::iterator it; // Create iterator for Node* container 

    for ( it = explored.begin(); it != explored.end(); ++it)
    {
        if ( (*it)->getXPos() == successorNode->getXPos() && (*it)->getYPos() == successorNode->getYPos())
        {
            return true;
        }
    }

    return false;
}

/* This function displays the path from the root node to the current node
 * takes the vector to add to as a pointer 
 */
void calculateRoute(Node* rootNode, Node* currentNode, std::vector<Node*>* calculatedPath)
{
    std::string pathString;

    //std::cout << "Start Grid" << std::endl;
    //rootNode->getGrid().printGrid();

    //std::cout << "\n\nEnd Grid" << std::endl;
    //currentNode->getGrid().printGrid(); 

    for (Node* n : explored)
    {
        if (n->isGoalState(goalX, goalY)) // Found the node which represents the goal state
        {
            while (n->getParentNode() != NULL) // Traverse upwards until we reach the root node 
            {
                pathString.append(n->getMoveDirString()); // Append the direction to the output string
                calculatedPath->push_back(n); // Add the node to the path the robot will take
                n = n->getParentNode(); // Move one level back up the tree
            }
        }
    }
    std::reverse(pathString.begin(), pathString.end()); // Reverse the string so it displays start -> goal instead of vice-versa
    std::cout << "\n\nPATH: " << pathString << std::endl;

    std::reverse(calculatedPath->begin(), calculatedPath->end()); // Reverse vector storing the path in a similar mannner
}

std::vector<Node*> bestFirstSearch(int curXPos, int curYPos)
{
    /* Clear the data structures */
    explored.clear();
    frontier = std::priority_queue<Node*, std::vector<Node*>, findClosestNode>(); 

    Node* rootNode = new Node(occupancyGrid, NULL, curXPos, curYPos, NONE); // Create the root node, the parent node is set to NULL
    Node* currentNode = rootNode;

    std::vector<Node*> calculatedPath; // Store the calculated path
    bool pathFound = false;

    while (!currentNode->isGoalState(goalX, goalY)) 
    {
        explored.push_back(currentNode);

        std::vector<Node*> successors = spawnSuccessors(currentNode, currentNode->getXPos(), currentNode->getYPos()); 

        for (Node* successorNode : successors)
        {
            // If the node has not already been explored, add it to the nodes to check
            if (!nodeExplored(successorNode))
            {
                frontier.push(successorNode);
            }

            if (successorNode->isGoalState(goalX, goalY))
            {
                pathFound = true;
                explored.push_back(successorNode);
                break; // Break out of this inner for loop
            }
        } 

        currentNode = frontier.top(); // Set new current node to the node at the front of the queue 
        frontier.pop(); // Remove element from top of queue

        if (pathFound) 
        {
            break; // Finished searching
        }
    }

    /* Display and store the calculated route from the start to the goal (last node added to explored list */
    calculateRoute(rootNode, explored.at(explored.size() - 1), &calculatedPath); 

    return calculatedPath;
}

bool isGoal(double curXPos, double curYPos) 
{
    return (distanceFromGoal(curXPos, curYPos) < 0.10); // If combined distance from goal is a negligible value
}

void getUserInput()
{
     bool validInput = false;

    while (!validInput)
    {
        std::cout << "Goal Position (X): ";
        std::cin >> goalX;
        if (std::cin.fail() || std::cin.eof() || goalX < GRID_MIN_X || goalX > GRID_MAX_X)
        {
            std::cin.clear(); std::cin.ignore();
            std::cerr << "\n\nInvalid goal position. Input must be numerical and a valid grid position\n\n";
            continue;
        }

        std::cout << "Goal Position (Y): ";
        std::cin >> goalY;

        if (std::cin.fail() || std::cin.eof() || goalY < GRID_MIN_Y || goalY > GRID_MAX_Y || occupancyGrid.isObstacle(goalX, goalY))
        {
            std::cin.clear(); std::cin.ignore();
            std::cerr << "\nInvalid goal position. Input must be numerical, a valid grid position and unoccupied\n\n";
            continue;
        }

        validInput = true;
    }
}

int main(int argc, char *argv[])
{
    getUserInput();

	PlayerClient    robot("localhost");
	RangerProxy      rp(&robot,0);
	Position2dProxy pp(&robot,0);

    pp.SetMotorEnable(false);
    
    robot.Read();

    pp.SetMotorEnable(true);

    occupancyGrid.setRobotIdx(pp.GetXPos(), pp.GetYPos());

    std::vector<Node*> calculatedPath = bestFirstSearch(pp.GetXPos(), pp.GetYPos()); // Run a search to find a path to the goal position

    player_pose2d_t newPos; // Stores the next position

    /* The main loop that while continue until the robot has reached its destination */
    while(!isGoal(pp.GetXPos(), pp.GetYPos()))
	{
        robot.Read(); // Update position data

        for (Node* n : calculatedPath)
        {
            newPos.px = n->getXPos();
            newPos.py = n->getYPos();
            
            /** Sonar handling
             double turnrate;

            if ( (rp[0] + rp[1] + rp[2]) < (rp[5] + rp[6] + rp[7]) ) 
            {
                turnrate = dtor(-60); // Turn rates decided after experimentation
            } 

            else 
            {
                turnrate = dtor(40);
            }

            if (rp[3] < 1.2 || rp[4] < 1.2) 
            {
                pp.SetSpeed(-0.700, turnrate); // Move backwards
                usleep(300000);
            } 

            */

            //std::cout << n->getMoveDirString() << std::endl; 

            pp.GoTo(newPos);

            if (isGoal(pp.GetXPos(), pp.GetYPos()))
            {
                break;
            }
        }

        /* Recalculate position and run the search again */
        robot.Read();
        calculatedPath.clear();
        calculatedPath = bestFirstSearch(pp.GetXPos(), pp.GetYPos());
	}

    std::cout << "Moved to Goal!" << std::endl;
    return 0;
}

