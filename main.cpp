#include <iostream>
#include <stack>
#include <cstdlib>  // atoi

#include "Maze.h"
#include "MazeDefinitions.h"
#include "PathFinder.h"




unsigned int Manhattan(unsigned int cellX, unsigned int cellY, unsigned int cenX, unsigned int cenY){
	unsigned int dist = 0;
	
	// Absolute difference between x coordinates
	if(cellX < cenX)
		dist = cenX - cellX;
	else dist = cellX - cenX;

	// Absolute difference between y coordinates
	if(cellY < cenY)
		dist += cenY - cellY;
	else dist += cellY - cenY;

	return dist;
}

struct cell{
	unsigned int x;
	unsigned int y;
};

class FloodFill : public PathFinder {
public:
	// Constructor
	FloodFill(): heading(NORTH){ 
		cells.clearAll();
		horizontal.clearAll();
		vertical.clearAll();
		infinity = ~0;

		// Initialize the Manhattan distances of each cell with respect to the center 4 cells
		for(int i = 0; i < MazeDefinitions::MAZE_LEN/2; i++){
			for(int j = 0; j < MazeDefinitions::MAZE_LEN/2; j++)
				dist[i][j] = Manhattan(i, j, MazeDefinitions::MAZE_LEN/2 - 1, MazeDefinitions::MAZE_LEN/2 - 1);
		}		
		for(int i = 0; i < MazeDefinitions::MAZE_LEN/2; i++){
			for(int j = MazeDefinitions::MAZE_LEN/2; j < MazeDefinitions::MAZE_LEN; j++)
				dist[i][j] = Manhattan(i, j, MazeDefinitions::MAZE_LEN/2 - 1, MazeDefinitions::MAZE_LEN/2);
		}		
		for(int i = MazeDefinitions::MAZE_LEN/2; i < MazeDefinitions::MAZE_LEN; i++){
			for(int j = 0; j < MazeDefinitions::MAZE_LEN/2; j++)
				dist[i][j] = Manhattan(i, j, MazeDefinitions::MAZE_LEN/2, MazeDefinitions::MAZE_LEN/2 - 1);
		}		
		for(int i = MazeDefinitions::MAZE_LEN/2; i < MazeDefinitions::MAZE_LEN; i++){
			for(int j = MazeDefinitions::MAZE_LEN/2; j < MazeDefinitions::MAZE_LEN; j++)
				dist[i][j] = Manhattan(i, j, MazeDefinitions::MAZE_LEN/2, MazeDefinitions::MAZE_LEN/2);
		}	
//		for(int i = 0; i < 16; i++){
//			for(int j = 0; j < 16; j++)
//				std::cout << dist[i][j] << " ";
//			std::cout << std::endl;
//		}

		for(int i = 0; i < MazeDefinitions::MAZE_LEN; i++)
			horizontal.set(0, i);
//		for(int j = 0; j < MazeDefinitions::MAZE_LEN; j++)
//			horizontal.set(MazeDefinitions::MAZE_LEN - 1, j);
		for(int k = 0; k < MazeDefinitions::MAZE_LEN; k++)
			vertical.set(k, 0);
//		for(int l = 0; l < MazeDefinitions::MAZE_LEN; l++)
//			vertical.set(l, MazeDefinitions::MAZE_LEN - 1);

	}

	// getDist returns the dist. value of the cell.
	// If it is out of bounds, then return infinity.
	unsigned int getDist(unsigned int x, unsigned int y){
		if(x < 0 || x >= MazeDefinitions::MAZE_LEN || y < 0 || y >= MazeDefinitions::MAZE_LEN)
			return infinity;
		else return dist[x][y];
	}

	// x is the column
	// y is the row
	void floodFill(cell block, unsigned int dist[MazeDefinitions::MAZE_LEN][MazeDefinitions::MAZE_LEN], 
		BitVector256 horizontal, BitVector256 vertical){
		std::stack<cell> cells;
		BitVector256 processedCells;
//		cells.push(block);
		cells.push(block); // Push initial location
		processedCells.clearAll();

		while(!cells.empty()){
		   // Record position and remove from stack
		   unsigned int row = (cells.top()).y;
		   unsigned int col = (cells.top()).x;
		   cells.pop();

		   processedCells.set(row,col);

		   if(dist[col][row] == 0 || dist[col][row] == infinity)
			  continue;

		   // shortest is the shortest Manhattan distance from center
		   unsigned int shortest = ~0;

		   // Loop through every neighbor
		   Dir face = NORTH;
		   for(int i = 0; i < 4; i++, face = clockwise(face)){
			  unsigned int neighborX = col;
			  unsigned int neighborY = row;
			  bool hasWall;
//			  std::cout << "Checking face" << std::endl;
			  switch(face){
				 case NORTH:
					neighborY += 1;
					hasWall = horizontal.get(neighborY, neighborX);
					break;
				 case EAST:
					neighborX += 1;
					hasWall = vertical.get(neighborY, neighborX);
					break;
				 case SOUTH:
					neighborY -= 1;
					hasWall = horizontal.get(row,col);
					break;
				 case WEST:
					neighborX -= 1;
					hasWall = vertical.get(row,col);
					break;
			  }

			  if(neighborX < 0 || neighborY < 0 || neighborX >= MazeDefinitions::MAZE_LEN || neighborY >= MazeDefinitions::MAZE_LEN)
				 continue;

			  // if there is no wall between current and neighbor

			  if(!hasWall){
				 // take shorter distance
				 if(dist[neighborX][neighborY] < shortest)
					shortest = dist[neighborX][neighborY];
				 // if neighbor hasn't been processed, push onto stack
				 if(!processedCells.get(neighborY, neighborX)){
					cell neighbor;
					neighbor.x = neighborX;
					neighbor.y = neighborY;
					cells.push(neighbor);   
				 }
			  }
		   }

		   // cell has no opening, skip
		   if(shortest == infinity)
			  continue;
		   // nothing was updated, skip
		   if(dist[col][row] == (shortest + 1))
			  continue;
//		   std::cout << "We are changing the Manhattan distance" << std::endl;
//		   std::cout << "And the shortest is " << shortest << std::endl;
		   dist[col][row] = shortest + 1;

		   // push neighbors so we can update their distances
		   cell neighbor;
		   neighbor.x = col;
		   neighbor.y = row+1;
		   cells.push(neighbor);

		   neighbor.x = col+1;
		   neighbor.y = row;
		   cells.push(neighbor);

		   neighbor.x = col;
		   neighbor.y = row-1;
		   cells.push(neighbor);

		   neighbor.x = col-1;
		   neighbor.y = row;
		   cells.push(neighbor);

		}	
		std::cout << "Floodfill finished" << std::endl;
	}

	// (x,y) is the mouse's current position
	MouseMovement nextMovement(unsigned x, unsigned y, const Maze& maze){

		// Print statements to debug
		// Check walls and Manhattan dist
		//std::cout << "This is the horizontal walls" << std::endl;
		//for(int i = 15; i >= 0; i--){
		//	for(int j = 0; j < 16; j++)
		//		std::cout << horizontal.get(i,j);
		//	std::cout << std::endl;
		//}
		//
		//std::cout << "This is the vertical walls" << std::endl;
		//for(int i = 15; i >= 0; i--){
		//	for(int j = 0; j < 16; j++)
		//		std::cout << vertical.get(i,j);
		//	std::cout << std::endl;
		//}

		//for(int i = 0; i < 16; i++){
		//	for(int j = 0; j < 16; j++)
		//		std::cout << dist[i][j] << " ";
		//	std::cout << std::endl;
		//}
		//std::cout << "The heading is " << heading << std::endl;

		// Print maze
        std::cout << "Hit enter to continue..." << std::endl;
        std::cin.ignore(10000, '\n');
        std::cin.clear();
        std::cout << maze.draw(5) << std::endl << std::endl;

		// Put coordinates in block
		// Check walls around mouse
		cell block;
		block.x = x;
		block.y = y;
		bool front = maze.wallInFront();
		bool left = maze.wallOnLeft();
		bool right = maze.wallOnRight();
		
		// If we're at the center,
		// then return victory!
		if(x == MazeDefinitions::MAZE_LEN/2 && y == MazeDefinitions::MAZE_LEN/2 
			|| x == MazeDefinitions::MAZE_LEN/2 - 1 && y == MazeDefinitions::MAZE_LEN/2 - 1
			|| x == MazeDefinitions::MAZE_LEN/2 && y == MazeDefinitions::MAZE_LEN/2 - 1
			|| x == MazeDefinitions::MAZE_LEN/2 - 1 && y == MazeDefinitions::MAZE_LEN/2)
			return Finish;

		// While moving through the maze, record any horizontal/vertical walls
		if(heading == NORTH){
			if(front)
				horizontal.set(y+1,x);
			if(left)
				vertical.set(y,x);
			if(right)
				vertical.set(y,x+1);
		}
		else if(heading == EAST){
			if(front)
				vertical.set(y,x+1);
			if(left)
				horizontal.set(y+1,x);
			if(right)
				horizontal.set(y,x);
		}
		else if(heading == SOUTH){
			if(front)
				horizontal.set(y,x);
			if(left)
				vertical.set(y,x+1);
			if(right)
				vertical.set(y,x);
		}
		else{
			if(front)
				vertical.set(y,x);
			if(left)
				horizontal.set(y,x);
			if(right)
				horizontal.set(y+1,x);
		}

		// If we're stuck, run floodfill
		// e.g. If cells around us have bigger dist (only count reachable cells),
		// or we're boxed in (wall in front, to left and to right)
		if((getDist(x,y) <= getDist(x,y+1) || horizontal.get(y+1,x))
			&& (getDist(x,y) <= getDist(x,y-1) || horizontal.get(y,x))
			&& (getDist(x,y) <= getDist(x+1,y) || vertical.get(y,x+1))
			&& (getDist(x,y) <= getDist(x-1,y) || vertical.get(y,x)) ){
			floodFill(block, dist, horizontal, vertical); // Run floodfill
		}
		else if(front && left && right)
			floodFill(block, dist, horizontal, vertical);

		// If there is a wall in front,
		// turn accordingly if there are side walls
		if(maze.wallInFront()){
			if(left && right){
				heading = opposite(heading);
				return TurnAround;
			}
			else if(left){
				heading = clockwise(heading);
				return TurnClockwise;
			}
			else{
				heading = counterClockwise(heading);
				return TurnCounterClockwise;
			}
		}

		// Check for unexplored cells
		if(!cells.get(y,x)){
			// Mark cell as explored
			cells.set(y,x);
			return MoveForward;
		}

		// If the current cell has been explored already,
		// go to the block with the smallest dist.
		left = maze.wallOnLeft();
		right = maze.wallOnRight();
		unsigned int shortest;
		MouseMovement dir = MoveForward;
		switch(heading){
		case NORTH:
			shortest = dist[x][y+1];
			if(!left){
				if(shortest > dist[x-1][y]){
					shortest = dist[x-1][y];
					heading = counterClockwise(heading);
					dir = TurnCounterClockwise;
				}
			}
			if(!right){
				if(shortest > dist[x+1][y]){
					shortest = dist[x+1][y];
					heading = clockwise(heading);
					dir = TurnClockwise;
				}				
			}
			break;
		case EAST:
			shortest = dist[x+1][y];
			if(!left){
				if(shortest > dist[x][y+1]){
					shortest = dist[x][y+1];
					heading = counterClockwise(heading);
					dir = TurnCounterClockwise;
				}
			}
			if(!right){
				if(shortest > dist[x][y-1]){
					shortest = dist[x][y-1];
					heading = clockwise(heading);
					dir = TurnClockwise;
				}				
			}
			break;
		case SOUTH:
			shortest = dist[x][y-1];
			if(!left){
				if(shortest > dist[x+1][y]){
					shortest = dist[x+1][y];
					heading = counterClockwise(heading);
					dir = TurnCounterClockwise;
				}
			}
			if(!right){
				if(shortest > dist[x-1][y]){
					shortest = dist[x-1][y];
					heading = clockwise(heading);
					dir = TurnClockwise;
				}				
			}
			break;
		case WEST:
			shortest = dist[x-1][y];
			if(!left){
				if(shortest > dist[x][y-1]){
					shortest = dist[x][y-1];
					heading = counterClockwise(heading);
					dir = TurnCounterClockwise;
				}
			}
			if(!right){
				if(shortest > dist[x][y+1]){
					shortest = dist[x][y+1];
					heading = clockwise(heading);
					dir = TurnClockwise;
				}				
			}
			break;
		}

		return dir;

	/*
		if(!maze.wallInFront())
			return MoveForward;

		if(!maze.wallOnLeft())
			return TurnCounterClockwise;

		if(!maze.wallOnRight())
			return TurnClockwise;

		return TurnAround;
	*/
	}

private:
	// cells records whether or not (1/0) the cell has been explored
	BitVector256 cells;
	// vertical/horizontal keeps track of walls
	BitVector256 vertical;
	BitVector256 horizontal;
	Dir heading;
	// dist keeps track of the Manhattan distance from center
	unsigned int dist[MazeDefinitions::MAZE_LEN][MazeDefinitions::MAZE_LEN];
	unsigned int infinity;
};

/**
 * Demo of a PathFinder implementation.
 *
 * Do not use a left/right wall following algorithm, as most
 * Micromouse mazes are designed for such algorithms to fail.
 */
class LeftWallFollower : public PathFinder {
public:
    LeftWallFollower(bool shouldPause = false) : pause(shouldPause) {
        shouldGoForward = false;
        visitedStart = false;
    }

    MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze) {
        const bool frontWall = maze.wallInFront();
        const bool leftWall  = maze.wallOnLeft();

        // Pause at each cell if the user requests it.
        // It allows for better viewing on command line.
        if(pause) {
            std::cout << "Hit enter to continue..." << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.clear();
        }

        std::cout << maze.draw(5) << std::endl << std::endl;

        // If we somehow miraculously hit the center
        // of the maze, just terminate and celebrate!
        if(isAtCenter(x, y)) {
            std::cout << "Found center! Good enough for the demo, won't try to get back." << std::endl;
            return Finish;
        }

        // If we hit the start of the maze a second time, then
        // we couldn't find the center and never will...
        if(x == 0 && y == 0) {
            if(visitedStart) {
                std::cout << "Unable to find center, giving up." << std::endl;
                return Finish;
            } else {
                visitedStart = true;
            }
        }

        // If we have just turned left, we should take that path!
        if(!frontWall && shouldGoForward) {
            shouldGoForward = false;
            return MoveForward;
        }

        // As long as nothing is in front and we have
        // a wall to our left, keep going forward!
        if(!frontWall && leftWall) {
            shouldGoForward = false;
            return MoveForward;
        }

        // If our forward and left paths are blocked
        // we should try going to the right!
        if(frontWall && leftWall) {
            shouldGoForward = false;
            return TurnClockwise;
        }

        // Lastly, if there is no left wall we should take that path!
        if(!leftWall) {
            shouldGoForward = true;
            return TurnCounterClockwise;
        }

        // If we get stuck somehow, just terminate.
        std::cout << "Got stuck..." << std::endl;
        return Finish;
    }

protected:
    // Helps us determine that we should go forward if we have just turned left.
    bool shouldGoForward;

    // Helps us determine if we've made a loop around the maze without finding the center.
    bool visitedStart;

    // Indicates we should pause before moving to next cell.
    // Useful for command line usage.
    const bool pause;

    bool isAtCenter(unsigned x, unsigned y) const {
        unsigned midpoint = MazeDefinitions::MAZE_LEN / 2;

        if(MazeDefinitions::MAZE_LEN % 2 != 0) {
            return x == midpoint && y == midpoint;
        }

        return  (x == midpoint     && y == midpoint    ) ||
        (x == midpoint - 1 && y == midpoint    ) ||
        (x == midpoint     && y == midpoint - 1) ||
        (x == midpoint - 1 && y == midpoint - 1);
    }
};

int main(int argc, char * argv[]) {
    MazeDefinitions::MazeEncodingName mazeName = MazeDefinitions::MAZE_CAMM_2012;
    bool pause = true;

    // Since Windows does not support getopt directly, we will
    // have to parse the command line arguments ourselves.

    // Skip the program name, start with argument index 1
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-m") == 0 && i+1 < argc) {
            int mazeOption = atoi(argv[++i]);
            if(mazeOption < MazeDefinitions::MAZE_NAME_MAX && mazeOption > 0) {
                    mazeName = (MazeDefinitions::MazeEncodingName)mazeOption;
            }
        } else if(strcmp(argv[i], "-p") == 0) {
            pause = true;
        } else {
            std::cout << "Usage: " << argv[0] << " [-m N] [-p]" << std::endl;
            std::cout << "\t-m N will load the maze corresponding to N, or 0 if invalid N or missing option" << std::endl;
            std::cout << "\t-p will wait for a newline in between cell traversals" << std::endl;
            return -1;
        }
    }
	FloodFill testRun;
    LeftWallFollower leftWallFollower(pause);
    Maze maze(mazeName, &testRun);
    std::cout << maze.draw(5) << std::endl << std::endl;

    maze.start();
}
