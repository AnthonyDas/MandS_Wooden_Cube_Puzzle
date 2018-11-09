#pragma once

#include <iostream>

/* Class to act as a single (x,y,z)-point in 3D space.*/
class element {

	int x, y, z;

public:

	friend std::ostream &operator<< (std::ostream &, const element &);

	element(int, int, int);

	element(const element &);

	element &operator= (const element &);

	int get(int) const;

	void update(int, int);

};

std::ostream &operator<< (std::ostream &, const element &);