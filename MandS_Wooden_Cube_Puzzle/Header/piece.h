#pragma once

#include <iostream>
#include <vector>
#include "element.h"

/* Class to act as a puzzle piece. In particular each piece
is comprised of one or more elements. A piece also has an ID
so as to distinguish its boundary from other pieces.*/
class piece {

	int id;

	std::vector<element> elements;

public:

	friend std::ostream &operator<< (std::ostream &, const piece &);

	piece(int);

	piece(const piece &);

	piece &operator= (const piece &);

	int getId() const;

	// Add elements to this piece after construction.
	// All pieces are constructed "empty" by the base constructor.
	piece &addElement(const element &);

	element getElement(int) const;

	element &updateElement(int);

	// Shifts the piece along the x, y or z axis
	piece &shift(int, int);

	int size() const;

};

std::ostream &operator<< (std::ostream &, const piece &);