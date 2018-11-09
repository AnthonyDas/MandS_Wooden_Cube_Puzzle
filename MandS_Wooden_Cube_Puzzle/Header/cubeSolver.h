#pragma once

#include <vector>
#include "wrappedPiece.h"

/* Class to represent the puzzle cube itself. After construction
call addPiece() to add each piece in turn, finally call solve().
This class has been designed to be independent of both the number
and shape of the puzzle pieces. The size of the cube can be adjusted
by changing "#define SIZE".*/
class cubeSolver {

	// Grid to track which cube positions are occupied
	std::vector<std::vector<std::vector<int> > > grid;

	// The wrapped pieces
	std::vector<wrappedPiece> wrappedPieces;

	// The number of locations/shifts per piece with a 1-to-1
	// correspondence with each wrappedPiece within wrappedPieces
	std::vector<int> noOfLocations;

	// Iterations performed whilst solving
	long int count;

public:

	friend std::ostream &operator<< (std::ostream &, const cubeSolver &);

	cubeSolver();

	// Inform the cube of a puzzle piece
	cubeSolver &addPiece(const piece &);

	// Solve the puzzle. Call this after adding all puzzle pieces.
	void solve();

private:

	// Cycles through all orientations for every piece
	void outerSolver(const int, bool &);

	// Cycles through all locations for every now orientated piece
	void innerSolver(const int, bool &);

	// Empties the entire grid of all pieces
	void clearGrid();

	// Removes a single piece from the grid
	void removePiece(int);

	// Adds a single piece to the grid
	bool addPieceToGrid(piece &);

	// Increment the iteration counter and output progress logging
	void incrementCount();
};

std::ostream &operator<< (std::ostream &, const cubeSolver &);