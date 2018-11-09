#pragma once

#include <iostream>
#include <vector>
#include "element.h"
#include "piece.h"

// Define the size of the cube, i.e. SIZE = 3 means a 3 x 3 x 3 cube
#define SIZE 3

/* Class wrappedPiece wraps a plain piece, originalPiece, and provides mechanisms
to obtain rotations and shifts of the originalPiece, through outputs
orientatedPiece and locatedPiece whilst never modifiying originalPiece.
The intended ordering of calls is:
- orientatePiece() to set orientatedPiece (intermediate result) using originalPiece as the starting piece.
- relocatePiece() to set locatedPiece (final result) using orientatedPiece as the starting piece.*/
class wrappedPiece {

	// TODO: Make originalPiece const
	// const piece originalPiece;

	piece originalPiece, orientatedPiece, locatedPiece;

	// Re-using element class to store possible locations/shifts 
	// (This is really a misuse of the element class' intended purpose)
	std::vector<element> locations;

	// Direction enum along x, y, or z axis
	enum direction { dirx, diry, dirz };

	// TODO: Arrays may be less bulky than vector<vector<int> >
	// Rotation matrices along x-axis, y-axis and z-axis
	std::vector<std::vector<int> > rotmatx, rotmaty, rotmatz;

public:

	// Constructor taking in toWrap which is the originalPiece
	// We also setup our rotation matrices: rotmatx, rotmaty and rotmatz.
	wrappedPiece(const piece &);

	int getId() const;

	int noOfLocations();

	void printOrientatedPiece(std::ostream &) const;

	void printLocatedPiece(std::ostream &) const;

	// This orientates the originalPiece to produce intermediary output orientatedPiece.
	// Argument orientation should be in the range 0 to 23 inclusive. To see this think
	// of an "L" shaped piece. There are the 3 mayor x, y, z axis along which the piece
	// may be pointing in the positive or negative axis direction, this gives a total of
	// 6 possibilities. (Alternatively think of the 6 faces of the cube). Secondly by
	// rotating the L along an axis running through the longest straight the "thumb"
	// of the L can be made to point up, down, left or right. (Think of the four sides
	// to a square. This gives a total of 6 x 4 = 24 possibilities.  
	piece &orientatePiece(int);

	// This shifts intermediate orientatedPiece to produce final output locatedPiece.
	// Argument location should be within the range 0 to locations.size().
	piece &relocatePiece(int);

private:

	// Returns the lowest occupied position by the orientatedPiece
	// along the dir axis
	int getMinPosition(int) const;

	// Returns the highest occupied position by the orientatedPiece
	// along the dir axis
	int getMaxPosition(int) const;

	// After rotatePiece() has been called it may nolonger start at the origin.
	// This relocated the piece to start at the origin, ie. the minimum position 
	// occupied by the piece along all 3 major axis is 0. In particular this
	// ensure that the rotated piece remains inside the cube.
	void startAtOrigin();

	// Rotate an element using matrix multiplications with a rotation matrix.
	void rotateElement(const std::vector<std::vector<int> > &, element &);

	// Rotate a piece by rotating each element in turn.
	void rotatePiece(direction, int);

	// After a piece has been orientated we recalculate how it can be
	// shifted along each of the 3 major axis. The number of possible
	// shifts/locations won't change, but what each possible location
	// is will change. 
	void updateAvailableLocations();
};

std::ostream &operator<< (std::ostream &, const wrappedPiece &);