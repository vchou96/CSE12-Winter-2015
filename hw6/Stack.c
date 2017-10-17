#include <iostream>
#include "Base.h"
#include "Stack.h"
using namespace std;

/* DO NOT CHANGE:  This file is used in evaluation */

Base * Stack :: Pop (void) {
	return Remove (END);
}

Base * Stack :: Push (Base * element) {
	return Insert (element, END);
}

Base * Stack :: Top (void) {
	return View (END);
}
