#ifndef TREE_H
#define TREE_H

/* YOU CAN ADD CLASS HEADER AND FILE HEADER
 * DO NOT CHANGE ANYTHING ELSE:  This file is used in evaluation */

#include <iostream>
#include "Base.h"
using namespace std;

struct TNode;

class Tree : public Base {
	TNode * root;
	const char * tree_name;
	static bool debug_on;

public:
	static void Set_Debug (bool);
	Tree (const char *);
	~Tree (void);
	unsigned long Insert (Base *);
	const Base * Lookup (const Base *) const;
	Base * Remove (const Base *);
	ostream & Write (ostream &) const ;
};

#endif
