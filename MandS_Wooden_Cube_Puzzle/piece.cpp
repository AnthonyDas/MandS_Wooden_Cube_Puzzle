#include <iostream>
#include <vector>
#include "piece.h"


piece::piece(int identifier) : id(identifier) {}

piece::piece(const piece &toCopy) {
	id = toCopy.getId();
	elements = toCopy.elements;
}

piece& piece::operator= (const piece &toAssign) {
	id = toAssign.getId();
	elements = toAssign.elements;
	return *this;
}

int piece::getId() const {
	return id;
}

// Add elements to this piece after construction.
// All pieces are constructed "empty" by the base constructor.
piece& piece::addElement(const element &toAdd) {
	elements.push_back(toAdd);
	return *this;
}

element piece::getElement(int i) const {
	return elements[i];
}

element& piece::updateElement(int i) {
	return elements[i];
}

// Shifts the piece along the x, y or z axis
piece& piece::shift(int dir, int adjustment) {
	if (adjustment != 0) {
		for (int k = 0; k < size(); ++k) {
			elements[k].update(dir, getElement(k).get(dir) + adjustment);
		}
	}
	return *this;
}

int piece::size() const {
	return (int)elements.size();
}


std::ostream &operator<< (std::ostream &os, const piece &p) {
	os << "Piece: " << p.id << std::endl;
	for (int i = 0; i < 3; ++i) {
		if (i == 0) { os << "x: "; }
		if (i == 1) { os << "y: "; }
		if (i == 2) { os << "z: "; }
		for (int j = 0; j < p.size(); ++j) {
			os << p.elements[j].get(i) << " ";
		}
		os << std::endl;
	}
	return os;
}