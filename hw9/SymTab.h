#ifndef SYMTAB_H
#define SYMTAB_H

/* DO NOT CHANGE:  This file is used in evaluation */

#include <iostream>
#include "Tree.h"
using namespace std;

template <class Whatever>
class SymTab : private Tree<Whatever> {
public:
        SymTab (const char * datafile) : Tree<Whatever> (datafile) {}
        Tree<Whatever> :: GetCost;
        Tree<Whatever> :: GetOperation;
        Tree<Whatever> :: Insert;
        Tree<Whatever> :: Lookup;
        Tree<Whatever> :: Remove;
        Tree<Whatever> :: Set_Debug_On;
        Tree<Whatever> :: Set_Debug_Off;
        Tree<Whatever> :: Write;
};

#endif
