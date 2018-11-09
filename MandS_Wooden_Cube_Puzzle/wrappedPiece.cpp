#include <iostream>
#include <vector>
#include "wrappedPiece.h"
#include "element.h"
#include "piece.h"


// Constructor taking in toWrap which is the originalPiece
// We also setup our rotation matrices: rotmatx, rotmaty and rotmatz.
wrappedPiece::wrappedPiece(const piece &toWrap) : originalPiece(toWrap), orientatedPiece(toWrap), locatedPiece(toWrap) {
	std::vector<int> row1, row2, row3;
	row1.push_back(1); row1.push_back(0); row1.push_back(0);
	row2.push_back(0); row2.push_back(0); row2.push_back(-1);
	row3.push_back(0); row3.push_back(1); row3.push_back(0);
	rotmatx.push_back(row1);
	rotmatx.push_back(row2);
	rotmatx.push_back(row3);

	row1.clear(); row2.clear(); row3.clear();
	row1.push_back(0); row1.push_back(0); row1.push_back(1);
	row2.push_back(0); row2.push_back(1); row2.push_back(0);
	row3.push_back(-1); row3.push_back(0); row3.push_back(0);
	rotmaty.push_back(row1);
	rotmaty.push_back(row2);
	rotmaty.push_back(row3);

	row1.clear(); row2.clear(); row3.clear();
	row1.push_back(0); row1.push_back(-1); row1.push_back(0);
	row2.push_back(1); row2.push_back(0); row2.push_back(0);
	row3.push_back(0); row3.push_back(0); row3.push_back(1);
	rotmatz.push_back(row1);
	rotmatz.push_back(row2);
	rotmatz.push_back(row3);

	updateAvailableLocations();
}

int wrappedPiece::getId() const {
	return originalPiece.getId();
}

int wrappedPiece::noOfLocations() {
	return (int)locations.size();
}

void wrappedPiece::printOrientatedPiece(std::ostream &os) const {
	os << orientatedPiece;
}

void wrappedPiece::printLocatedPiece(std::ostream &os) const {
	os << locatedPiece;
}

// This orientates the originalPiece to produce intermediary output orientatedPiece.
// Argument orientation should be in the range 0 to 23 inclusive. To see this think
// of an "L" shaped piece. There are the 3 mayor x, y, z axis along which the piece
// may be pointing in the positive or negative axis direction, this gives a total of
// 6 possibilities. (Alternatively think of the 6 faces of the cube). Secondly by
// rotating the L along an axis running through the longest straight the "thumb"
// of the L can be made to point up, down, left or right. (Think of the four sides
// to a square. This gives a total of 6 x 4 = 24 possibilities.  
piece& wrappedPiece::orientatePiece(int orientation) {
	orientatedPiece = originalPiece;

	int rotations, direction;

	// Work out the intended direction and rotation
	rotations = orientation % 4;
	direction = (orientation - rotations) / 4;

	// First set the piece in the intended direction,
	// then perform the necessary rotations
	if (direction == 0) {
		// Already in the correct direction
		rotatePiece(dirx, rotations);
	}
	else if (direction == 1) {
		rotatePiece(diry, 1);
		rotatePiece(dirz, rotations);
	}
	else if (direction == 2) {
		rotatePiece(diry, 2);
		rotatePiece(dirx, rotations);
	}
	else if (direction == 3) {
		rotatePiece(diry, 3);
		rotatePiece(dirz, rotations);
	}
	else if (direction == 4) {
		rotatePiece(dirz, 1);
		rotatePiece(diry, rotations);
	}
	else {
		rotatePiece(dirz, 3);
		rotatePiece(diry, rotations);
	}

	startAtOrigin();
	updateAvailableLocations();
	return orientatedPiece;
}

// This shifts intermediate orientatedPiece to produce final output locatedPiece.
// Argument location should be within the range 0 to locations.size().
piece& wrappedPiece::relocatePiece(int location) {
	locatedPiece = orientatedPiece;

	locatedPiece.shift(0, locations[location].get(0));
	locatedPiece.shift(1, locations[location].get(1));
	locatedPiece.shift(2, locations[location].get(2));

	return locatedPiece;
}

// Returns the lowest occupied position by the orientatedPiece
// along the dir axis
int wrappedPiece::getMinPosition(int dir) const {
	int min = 99, pos;
	for (int j = 0; j < orientatedPiece.size(); ++j) {
		pos = orientatedPiece.getElement(j).get(dir);
		if (pos < min) { min = pos; }
	}
	return min;
}

// Returns the highest occupied position by the orientatedPiece
// along the dir axis
int wrappedPiece::getMaxPosition(int dir) const {
	int max = -99, pos;
	for (int j = 0; j < orientatedPiece.size(); ++j) {
		pos = orientatedPiece.getElement(j).get(dir);
		if (pos > max) { max = pos; }
	}
	return max;
}

// After rotatePiece() has been called it may nolonger start at the origin.
// This relocated the piece to start at the origin, ie. the minimum position 
// occupied by the piece along all 3 major axis is 0. In particular this
// ensure that the rotated piece remains inside the cube.
void wrappedPiece::startAtOrigin() {
	for (int i = 0; i < 3; ++i) {
		int adjustment = 0 - getMinPosition(i);
		if (adjustment != 0) {
			orientatedPiece.shift(i, adjustment);
		}
	}
}

// Rotate an element using matrix multiplications with a rotation matrix.
void wrappedPiece::rotateElement(const std::vector<std::vector<int> > &matrot, element &toRotate) {
	int x = 0, y = 0, z = 0;

	// Matrix multiplication
	for (int i = 0; i < 3; ++i) {
		x += matrot[0][i] * toRotate.get(i);
		y += matrot[1][i] * toRotate.get(i);
		z += matrot[2][i] * toRotate.get(i);
	}

	toRotate.update(0, x);
	toRotate.update(1, y);
	toRotate.update(2, z);
}

// Rotate a piece by rotating each element in turn.
void wrappedPiece::rotatePiece(direction dir, int howManyTimes) {
	if (howManyTimes == 0) {
		return;
	}

	for (int count = 0; count < howManyTimes; ++count) {
		for (int i = 0; i < orientatedPiece.size(); ++i) {
			if (dir == dirx) {
				rotateElement(rotmatx, orientatedPiece.updateElement(i));
			}
			else if (dir == diry) {
				rotateElement(rotmaty, orientatedPiece.updateElement(i));
			}
			else {
				rotateElement(rotmatz, orientatedPiece.updateElement(i));
			}
		}
	}
}

// After a piece has been orientated we recalculate how it can be
// shifted along each of the 3 major axis. The number of possible
// shifts/locations won't change, but what each possible location
// is will change. 
void wrappedPiece::updateAvailableLocations() {
	locations.clear();

	int maxxshift, maxyshift, maxzshift;

	maxxshift = (SIZE - 1) - getMaxPosition(0);
	maxyshift = (SIZE - 1) - getMaxPosition(1);
	maxzshift = (SIZE - 1) - getMaxPosition(2);

	for (int i = 0; i <= maxxshift; ++i) {
		for (int j = 0; j <= maxyshift; ++j) {
			for (int k = 0; k <= maxzshift; ++k) {
				locations.push_back(element(i, j, k));
			}
		}
	}
}


std::ostream &operator<< (std::ostream &os, const wrappedPiece &pw) {
	pw.printLocatedPiece(os);
	return os;
}