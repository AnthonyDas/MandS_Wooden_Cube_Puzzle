#include <vector>
#include <cmath> // pow()

#include "cubeSolver.h"
#include "wrappedPiece.h"

cubeSolver::cubeSolver() {
	std::vector<int> row(SIZE);
	std::vector<std::vector<int> > mat(SIZE, row);
	grid.insert(grid.begin(), SIZE, mat);

	count = 0;
}

// Inform the cube of a puzzle piece
cubeSolver& cubeSolver::addPiece(const piece &toAdd) {
	wrappedPieces.push_back(wrappedPiece(toAdd));
	return *this;
}

// Solve the puzzle. Call this after adding all puzzle pieces.
void cubeSolver::solve() {

	// Retrieve the number of shifts/locations each piece permits
	long long int totalLocationPermutations = 1;
	std::cout << "Shifts/locations per piece: ";
	for (int i = 0; i < (int)wrappedPieces.size(); ++i) {
		int locations = wrappedPieces[i].noOfLocations();
		std::cout << "[" << locations << "] ";

		noOfLocations.push_back(locations);

		totalLocationPermutations *= locations;
	}
	std::cout << std::endl << std::endl;

	std::cout << "Total shifts/locations permutations: [" << totalLocationPermutations << "]" << std::endl;
	std::cout << "Total orientation permutations: [" << pow(24, (int)wrappedPieces.size()) << "]" << std::endl;
	std::cout << "The total possible combinations/iterations will be the PRODUCT of the above two numbers." << std::endl;
	std::cout << "However note that if one solution exists, then 24 solutions necessarily exist by " << std::endl;
	std::cout << "re-orientating and rotating the first solution." << std::endl << std::endl;

	bool finished = false;
	outerSolver(0, finished); // Set outerLevel = 0
}

// Cycles through all orientations for every piece
void cubeSolver::outerSolver(const int outerLevel, bool &finished) {
	for (int orientation = 0; orientation < 24; ++orientation) {
		wrappedPieces[outerLevel].orientatePiece(orientation);
		// Only log top level 
		if (outerLevel == 0) {
			std::cout << "Piece [" << (outerLevel + 1) << "] with id [" << wrappedPieces[outerLevel].getId()
				<< "] orientation [" << orientation << "]" << std::endl;
		}

		// If there are levels below us then call outerSolver() recursively
		// to orientate lower pieces. Otherwise move onto innerSolver().
		if (outerLevel + 1 < (int)wrappedPieces.size()) {
			outerSolver(outerLevel + 1, finished);
			if (finished) {
				return;
			}
		}
		else {
			innerSolver(0, finished); // Set innerLevel = 0
			if (finished) {
				return;
			}
		}
	}
}

// Cycles through all locations for every now orientated piece
void cubeSolver::innerSolver(const int innerLevel, bool &finished) {
	for (int location = 0; location < noOfLocations[innerLevel]; ++location) {
		removePiece(wrappedPieces[innerLevel].getId());
		if (!addPieceToGrid(wrappedPieces[innerLevel].relocatePiece(location))) {
			incrementCount();
			continue;
		}

		// If there are levels below us then call innerSolver() recursively
		// to locate lower pieces. Otherwise we must be finished!
		if (innerLevel + 1 < (int)wrappedPieces.size()) {
			innerSolver(innerLevel + 1, finished);
			if (finished) {
				return;
			}
		}
		else {
			finished = true;
			std::cout << "Finished!!! Please print out cube." << std::endl;
			return;
		}
	}
}

// Empties the entire grid of all pieces
void cubeSolver::clearGrid() {
	for (unsigned int z = 0; z < SIZE; ++z) {
		for (unsigned int y = 0; y < SIZE; ++y) {
			for (unsigned int x = 0; x < SIZE; ++x) {
				grid[z][y][x] = 0;
			}
		}
	}
}

// Removes a single piece from the grid
void cubeSolver::removePiece(int id) {
	for (unsigned int z = 0; z < SIZE; ++z) {
		for (unsigned int y = 0; y < SIZE; ++y) {
			for (unsigned int x = 0; x < SIZE; ++x) {
				if (grid[z][y][x] == id) {
					grid[z][y][x] = 0;
				}
			}
		}
	}
}

// Adds a single piece to the grid
bool cubeSolver::addPieceToGrid(piece &toAdd) {
	for (int i = 0; i < toAdd.size(); ++i) {
		element elem = toAdd.getElement(i);
		if (grid[elem.get(2)][elem.get(1)][elem.get(0)] != 0) {
			return false; // Overlap detected
		}
	}

	for (int i = 0; i < toAdd.size(); ++i) {
		element elem = toAdd.getElement(i);
		grid[elem.get(2)][elem.get(1)][elem.get(0)] = toAdd.getId();
	}
	return true;
}

// Increment the iteration counter and output progress logging
void cubeSolver::incrementCount() {
	count++;
	if (count % 1000000 == 0) {
		std::cout << "Iteration: " << count << std::endl;
	}
}


std::ostream &operator<< (std::ostream &os, const cubeSolver &cube) {
	os << "Cube:" << std::endl;
	for (unsigned int y = 0; y < SIZE; ++y) {
		for (unsigned int z = 0; z < SIZE; ++z) {
			for (unsigned int x = 0; x < SIZE; ++x) {
				os << cube.grid[z][y][x] << " ";
			}
			os << " ";
		}
		os << std::endl;
	}
	return os;
}