/*Anthony Das 2013 - CubeSolver inspired by M&S Wooden Puzzle

Compile using g++:

g++ -Wall -Wconversion -g -mtune=native -O3
-I" .\MandS_Wooden_Cube_Puzzle\MandS_Wooden_Cube_Puzzle\Header"
main.cpp cubeSolver.cpp wrappedPiece.cpp piece.cpp element.cpp -o main.exe
*/

#include <iostream>
#include "piece.h"
#include "cubeSolver.h"

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
	std::cout << piece1 << std::endl;

	piece piece2(2);
	piece2.addElement(element(0, 0, 0))
		.addElement(element(0, 0, 1))
		.addElement(element(0, 1, 0))
		.addElement(element(1, 1, 0));
	std::cout << piece2 << std::endl;

	piece piece3(3);
	piece3.addElement(element(0, 0, 0))
		.addElement(element(0, 1, 0))
		.addElement(element(1, 1, 0))
		.addElement(element(2, 1, 0));
	std::cout << piece3 << std::endl;

	piece piece4(4);
	piece4.addElement(element(0, 1, 0))
		.addElement(element(0, 0, 1))
		.addElement(element(1, 0, 1))
		.addElement(element(0, 1, 1))
		.addElement(element(0, 2, 1));
	std::cout << piece4 << std::endl;

	piece piece5(5);
	piece5.addElement(element(0, 0, 1))
		.addElement(element(1, 0, 1))
		.addElement(element(1, 0, 0))
		.addElement(element(1, 1, 0))
		.addElement(element(2, 1, 0));
	std::cout << piece5 << std::endl;

	piece piece6(6);
	piece6.addElement(element(0, 0, 0))
		.addElement(element(0, 1, 0))
		.addElement(element(0, 0, 1))
		.addElement(element(1, 0, 1))
		.addElement(element(2, 0, 1));
	std::cout << piece6 << std::endl;

	cubeSolver cube;
	cube.addPiece(piece1)
		.addPiece(piece2)
		.addPiece(piece3)
		.addPiece(piece4)
		.addPiece(piece5)
		.addPiece(piece6);

	cube.solve();
	std::cout << cube;

	std::cout << "\nFinished - Please type something to quit" << std::endl;
	int dummy; std::cin >> dummy;
	return 0;
}