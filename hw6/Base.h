#ifndef BASE_H
#define BASE_H

/* DO NOT CHANGE:  This file is used in evaluation */

#include <iostream>
using namespace std;

struct Base {
	virtual operator char * (void) {
		return 0;
	}
	virtual operator long (void) {		// hash function
		return 0;
	}
	virtual long operator == (Base & base) {// isequal function
		return *this == base;
	}
	Base (void) {}				// new_element
	virtual ~Base (void) {}			// delete_element
	virtual ostream & Write (ostream & stream) = 0;// write_element
};

#endif
