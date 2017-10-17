#ifndef CALC_H
#define CALC_H

/* DO NOT CHANGE:  This file is used in evaluation */

#include <iostream>
#include "Base.h"
#include "Stack.h"
#include "SymTab.h"
using namespace std;

class Calculator : public Base {
	SymTab symtab;
	Stack stack1, stack2;
public:
	Calculator (long table_size) : symtab (table_size) {}
	~Calculator (void) {}
	long Eval (void);
	long InToPost (void);
	ostream & Write_Postfix (ostream & s) {
		return stack1.Write (s);
	}
	ostream & Write (ostream & s) {
		stack1.Write (s << "Stack 1 is:\n");
		stack2.Write (s << "Stack 2 is:\n");
		return symtab.Write (s << "SymTab is:  ");
	}
};

#endif
