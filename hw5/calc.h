#ifndef CALC_H
#define CALC_H

/* DO NOT CHANGE:  This file is used in evaluation */

#include <stdio.h>
#include "stack.h"

/* declare public functions for stack words */
void delete_CalcWord (void *);
FILE * write_CalcWord (void *, FILE *);

/* basic calculator functions */
long eval (Stack *);
long intopost (Stack *);

#endif
