#include <iostream>
#include "element.h"


element::element(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {};

element::element(const element &toCopy) {
	x = toCopy.x;
	y = toCopy.y;
	z = toCopy.z;
}

element& element::operator= (const element &toAssign) {
	x = toAssign.x;
	y = toAssign.y;
	z = toAssign.z;
	return *this;
}

int element::get(int dir) const {
	if (dir == 0) {
		return x;
	}
	else if (dir == 1) {
		return y;
	}
	else {
		return z;
	}
}

void element::update(int dir, int val) {
	if (dir == 0) { x = val; }
	if (dir == 1) { y = val; }
	if (dir == 2) { z = val; }
}


std::ostream &operator<< (std::ostream &os, const element &e) {
	os << "x: " << e.x << std::endl << "y: " << e.y << std::endl << "z: " << e.z;
	return os;
}