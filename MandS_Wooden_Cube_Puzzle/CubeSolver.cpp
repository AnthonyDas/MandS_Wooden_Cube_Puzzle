/*Anthony Das 2013 - CubeSolver inspired by M&S Wooden Puzzle

Compile using g++:

cd C:\...\Visual Studio Projects\CubeSolver\CubeSolver
g++ -Wall -O3 CubeSolver.cpp -o CubeSolver.exe
*/

#include <iostream> // include for command window I/O
#include <vector> // include for vector
#include <cmath> // include for pow()

using namespace std;

// Define the size of the cube, i.e. SIZE = 3 means a 3 x 3 x 3 cube
#define SIZE 3


/* Class to act as a single (x,y,z)-point in 3D space.*/
class element { 

	int x, y, z;

public:

	friend ostream &operator<< (ostream &, const element &);

	element(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {};

	element(const element &toCopy) {
		x = toCopy.x;
		y = toCopy.y;
		z = toCopy.z;
	}

	element &operator= (const element &toAssign) {
		x = toAssign.x;
		y = toAssign.y;
		z = toAssign.z;
		return *this;
	}

	int get(int dir) const {
		if (dir == 0) {
			return x;
		} else if (dir == 1) {
			return y;
		} else {
			return z;
		}
	}

	void update(int dir, int val) {
		if (dir == 0) {x = val;}
		if (dir == 1) {y = val;}
		if (dir == 2) {z = val;}
	}

};

ostream &operator<< (ostream &os, const element &e) {
	cout << "x: " << e.x << endl << "y: " << e.y << endl << "z: " << e.z;
	return os;
}


/* Class to act as a puzzle piece. In particular each piece
is comprised of one or more elements. A piece also has an ID
so as to distinguish its boundary from other pieces.*/
class piece {

	int id;

	vector<element> elements;

public:

	friend ostream &operator<< (ostream &, const piece &);

	piece(int identifier) : id(identifier) {}

	piece(const piece &toCopy) {
		id = toCopy.getId();
		elements = toCopy.elements;
	}

	piece &operator= (const piece &toAssign) {
		id = toAssign.getId();
		elements = toAssign.elements;
		return *this;
	}

	int getId() const {
		return id;
	}

	// Add elements to this piece after construction.
	// All pieces are constructed "empty" by the base constructor.
	piece &addElement(const element &toAdd) {
		elements.push_back(toAdd); 
		return *this;
	}

	element getElement(int i) const {
		return elements[i];
	}

	element &updateElement(int i)  {
		return elements[i];
	}

	// Shifts the piece along the x, y or z axis
	piece &shift(int dir, int adjustment) {
		if (adjustment != 0) {
			for (int k = 0; k < size(); ++k) {
				elements[k].update(dir, getElement(k).get(dir) + adjustment);
			}
		}
		return *this;
	}

	int size() const {
		return (int)elements.size();
	}

};

ostream &operator<< (ostream &os, const piece &p) {
	cout << "Piece: " << p.id << endl;
	for (int i = 0; i < 3; ++i) {
		if (i == 0) { cout << "x: "; }
		if (i == 1) { cout << "y: "; }
		if (i == 2) { cout << "z: "; }
		for (int j = 0; j < p.size(); ++j) {
			cout << p.elements[j].get(i) << " ";
		}
		cout << endl;
	}
	return os;
}


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
	vector<element> locations; 

	// Direction enum along x, y, or z axis
	enum direction {dirx, diry, dirz};

	// TODO: Arrays may be less bulky than vector<vector<int> >
	// Rotation matrices along x-axis, y-axis and z-axis
	vector<vector<int> > rotmatx, rotmaty, rotmatz;

public:

	// Constructor taking in toWrap which is the originalPiece
	// We also setup our rotation matrices: rotmatx, rotmaty and rotmatz.
	wrappedPiece(const piece &toWrap) : originalPiece(toWrap), orientatedPiece(toWrap), locatedPiece(toWrap) {
		vector<int> row1, row2, row3;
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

	int getId() const {
		return originalPiece.getId();
	}

	int noOfLocations() {
		return (int)locations.size();
	}

	void printOrientatedPiece(ostream &os) const {
		cout << orientatedPiece;
	}

	void printLocatedPiece(ostream &os) const {
		cout << locatedPiece;
	}

	// This orientates the originalPiece to produce intermediary output orientatedPiece.
	// Argument orientation should be in the range 0 to 23 inclusive. To see this think
	// of an "L" shaped piece. There are the 3 mayor x, y, z axis along which the piece
	// may be pointing in the positive or negative axis direction, this gives a total of
	// 6 possibilities. (Alternatively think of the 6 faces of the cube). Secondly by
	// rotating the L along an axis running through the longest straight the "thumb"
	// of the L can be made to point up, down, left or right. (Think of the four sides
	// to a square. This gives a total of 6 x 4 = 24 possibilities.  
	piece &orientatePiece(int orientation) {
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
		} else if (direction == 1) {
			rotatePiece(diry, 1);
			rotatePiece(dirz, rotations);
		} else if (direction == 2) {
			rotatePiece(diry, 2);
			rotatePiece(dirx, rotations);
		} else if (direction == 3) {
			rotatePiece(diry, 3);
			rotatePiece(dirz, rotations);
		} else if (direction == 4) {
			rotatePiece(dirz, 1);
			rotatePiece(diry, rotations);
		} else {
			rotatePiece(dirz, 3);
			rotatePiece(diry, rotations);
		} 

		startAtOrigin();
		updateAvailableLocations();
		return orientatedPiece;
	}

	// This shifts intermediate orientatedPiece to produce final output locatedPiece.
	// Argument location should be within the range 0 to locations.size().
	piece &relocatePiece(int location) {
		locatedPiece = orientatedPiece;

		locatedPiece.shift(0, locations[location].get(0));
		locatedPiece.shift(1, locations[location].get(1));
		locatedPiece.shift(2, locations[location].get(2));

		return locatedPiece;
	}

private:

	// Returns the lowest occupied position by the orientatedPiece
	// along the dir axis
	int getMinPosition(int dir) const {
		int min = 99, pos;
		for (int j = 0; j < orientatedPiece.size(); ++j) {
			pos = orientatedPiece.getElement(j).get(dir);
			if (pos < min) {min = pos;}
		}
		return min;
	}

	// Returns the highest occupied position by the orientatedPiece
	// along the dir axis
	int getMaxPosition(int dir) const {
		int max = -99, pos;
		for (int j = 0; j < orientatedPiece.size(); ++j) {
			pos = orientatedPiece.getElement(j).get(dir);
			if (pos > max) {max = pos;}
		}
		return max;
	}

	// After rotatePiece() has been called it may nolonger start at the origin.
	// This relocated the piece to start at the origin, ie. the minimum position 
	// occupied by the piece along all 3 major axis is 0. In particular this
	// ensure that the rotated piece remains inside the cube.
	void startAtOrigin() {
		for (int i = 0; i < 3; ++i) {
			int adjustment = 0 - getMinPosition(i);
			if (adjustment != 0) {
				orientatedPiece.shift(i, adjustment);
			}
		}
	}

	// Rotate an element using matrix multiplications with a rotation matrix.
	void rotateElement(const vector<vector<int> > &matrot, element &toRotate) {
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
	void rotatePiece(direction dir, int howManyTimes) {
		if (howManyTimes == 0) {
			return;
		}

		for (int count = 0; count < howManyTimes; ++count) {
			for (int i = 0; i < orientatedPiece.size(); ++i) {
				if (dir == dirx) {
					rotateElement(rotmatx, orientatedPiece.updateElement(i));
				} else if (dir == diry) {
					rotateElement(rotmaty, orientatedPiece.updateElement(i));
				} else {
					rotateElement(rotmatz, orientatedPiece.updateElement(i));
				}
			}
		}
	}

	// After a piece has been orientated we recalculate how it can be
	// shifted along each of the 3 major axis. The number of possible
	// shifts/locations won't change, but what each possible location
	// is will change. 
	void updateAvailableLocations() {
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
};

ostream &operator<< (ostream &os, const wrappedPiece &pw) {
	pw.printLocatedPiece(os);
	return os;
}


/* Class to represent the puzzle cube itself. After construction 
call addPiece() to add each piece in turn, finally call solve().
This class has been designed to be independent of both the number
and shape of the puzzle pieces. The size of the cube can be adjusted
by changing "#define SIZE".*/ 
class cubeSolver {

	// Grid to track which cube positions are occupied
	vector<vector<vector<int> > > grid;

	// The wrapped pieces
	vector<wrappedPiece> wrappedPieces;

	// The number of locations/shifts per piece with a 1-to-1
	// correspondence with each wrappedPiece within wrappedPieces
	vector<int> noOfLocations;

	// Iterations performed whilst solving
	long int count;

public:

	friend ostream &operator<< (ostream &, const cubeSolver &);

	cubeSolver() {
		vector<int> row(SIZE);
		vector<vector<int> > mat(SIZE, row);
		grid.insert(grid.begin(), SIZE, mat);

		count = 0;
	}

	// Inform the cube of a puzzle piece
	cubeSolver &addPiece(const piece &toAdd) {
		wrappedPieces.push_back(wrappedPiece(toAdd));
		return *this;
	}

	// Solve the puzzle. Call this after adding all puzzle pieces.
	void solve() {

		// Retrieve the number of shifts/locations each piece permits
		long long int totalLocationPermutations = 1;
		cout << "Shifts/locations per piece: ";
		for (int i = 0; i < (int)wrappedPieces.size(); ++i) {
			int locations = wrappedPieces[i].noOfLocations();
			cout << "[" << locations << "] ";

			noOfLocations.push_back(locations);

			totalLocationPermutations *= locations;
		}
		cout << endl << endl;

		cout << "Total shifts/locations permutations: [" << totalLocationPermutations << "]" << endl;
		cout << "Total orientation permutations: [" << pow(24, wrappedPieces.size()) << "]" << endl;
		cout << "The total possible combinations/iterations will be the PRODUCT of the above two numbers." << endl;
		cout << "However note that if one solution exists, then 24 solutions necessarily exist by " << endl;
		cout << "re-orientating and rotating the first solution." << endl << endl;

		bool finished = false;
		outerSolver(0, finished); // Set outerLevel = 0
	}

private :

	// Cycles through all orientations for every piece
	void outerSolver(const int outerLevel, bool &finished) {
		for (int orientation = 0; orientation < 24; ++orientation) {
			wrappedPieces[outerLevel].orientatePiece(orientation);
			// Only log top level 
			if (outerLevel == 0) {
				cout << "Piece [" << (outerLevel + 1) << "] with id [" << wrappedPieces[outerLevel].getId() << "] orientation [" << orientation << "]" << endl;
			}

			// If there are levels below us then call outerSolver() recursively
			// to orientate lower pieces. Otherwise move onto innerSolver().
			if (outerLevel + 1 < (int)wrappedPieces.size()) {
				outerSolver(outerLevel + 1, finished);
				if (finished) {
					return;
				}
			} else {
				innerSolver(0, finished); // Set innerLevel = 0
				if (finished) {
					return;
				}
			}
		}
	}

	// Cycles through all locations for every now orientated piece
	void innerSolver(const int innerLevel, bool &finished) {
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
			} else {
				finished = true;
                cout << "Finished!!! Please print our cube." << endl;
				return;	
			}
		}
	}

	// Empties the entire grid of all pieces
	void clearGrid() {
		for (unsigned int z = 0; z < SIZE; ++z) {
			for (unsigned int y = 0; y < SIZE; ++y) {
				for (unsigned int x = 0; x < SIZE; ++x) {
					grid[z][y][x] = 0;
				}
			}
		}
	}

	// Removes a single piece from the grid
	void removePiece(int id) {
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
	bool addPieceToGrid(piece &toAdd) {
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
	void incrementCount() {
		count++;
		if (count % 1000000 == 0) {
			cout << "Iteration: " << count << endl;
		}
	}
};

ostream &operator<< (ostream &os, const cubeSolver &cube) {
	os << "Cube:" << endl;
	for (unsigned int y = 0; y < SIZE; ++y) {
		for (unsigned int z = 0; z < SIZE; ++z) {
			for (unsigned int x = 0; x < SIZE; ++x) {
				os << cube.grid[z][y][x] << " ";
			}
			os << " ";
		}
		os << endl;
	}
	return os;
}

/*
void testing() {
cout << "Testing..." << endl;

piece piece1(1);
piece1.addElement(0, 0, 0).addElement(1, 0, 0).addElement(0, 1, 0).addElement(0, 1, 1);
cout << "Piece: " << piece1 << endl;

piece outx = rotation(dirx, piece1);
cout << "Rotate x: " << outx << endl;

piece outy = rotation(diry, piece1);
cout << "Rotate y: "<< outy << endl;

piece outz = rotation(dirz, piece1);
cout << "Rotate z: "<< outz << endl;

cout << "End Testing" << endl;
}*/

int main() {
	// testing();

	piece piece1(1);
	piece1.addElement(element(0, 0, 0))
		.addElement(element(1, 0, 0))
		.addElement(element(0, 1, 0))
		.addElement(element(0, 1, 1));
	cout << piece1 << endl;

	piece piece2(2);
	piece2.addElement(element(0, 0, 0))
		.addElement(element(0, 0, 1))
		.addElement(element(0, 1, 0))
		.addElement(element(1, 1, 0));
	cout << piece2 << endl;

	piece piece3(3);
	piece3.addElement(element(0, 0, 0))
		.addElement(element(0, 1, 0))
		.addElement(element(1, 1, 0))
		.addElement(element(2, 1, 0));
	cout << piece3 << endl;

	piece piece4(4);
	piece4.addElement(element(0, 1, 0))
		.addElement(element(0, 0, 1))
		.addElement(element(1, 0, 1))
		.addElement(element(0, 1, 1))
		.addElement(element(0, 2, 1));
	cout << piece4 << endl;

	piece piece5(5);
	piece5.addElement(element(0, 0, 1))
		.addElement(element(1, 0, 1))
		.addElement(element(1, 0, 0))
		.addElement(element(1, 1, 0))
		.addElement(element(2, 1, 0));
	cout << piece5 << endl;

	piece piece6(6);
	piece6.addElement(element(0, 0, 0))
		.addElement(element(0, 1, 0))
		.addElement(element(0, 0, 1))
		.addElement(element(1, 0, 1))
		.addElement(element(2, 0, 1));
	cout << piece6 << endl;

	cubeSolver cube;
	cube.addPiece(piece1)
		.addPiece(piece2)
		.addPiece(piece3)
		.addPiece(piece4)
		.addPiece(piece5)
		.addPiece(piece6);

	cube.solve();
	cout << cube;

	cout << "\nFinished - Please type something to quit" << endl;
	int dummy; cin >> dummy;
	return 0;
}