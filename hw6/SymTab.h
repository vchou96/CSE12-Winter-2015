#ifndef SYMTAB_H
#define SYMTAB_H

/* DO NOT CHANGE:  This file is used in evaluation */

#include <iostream>
#include "Base.h"
#include "Hash.h"
using namespace std;

class SymTab : private HashTable {
public:
	HashTable :: Insert;
	HashTable :: Lookup;
	HashTable :: Write;
	SymTab (int sz) : HashTable (sz) {}
};

#endif

