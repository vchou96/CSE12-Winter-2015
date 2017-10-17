/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        1/20/2015 
                                                        cs12xkf 
                                Assignment Three 
 
File Name:      stack.c 
Description:    This file provides the functions that driver.c will call to 
                create and manipulate stacks. new_Stack creates the 
		stack/allocates memory; isempty_Stack and isfull_Stack check
		if the stack is empty or full, respectively; empty_Stack
		empties the stack; num_elements returns the number of items in
		the stack; push places an item on top of the stack; top sends
		back the item on top of the stack; pop removes the top item
		from the stack; and delete_Stack deletes the stack/deallocates
		memory.

****************************************************************************/  
#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of next available space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "Num_elements check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                     "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                        "Attempt to write to a non-existent stack!!!\n";

/* Debug messages */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) {
    debug = FALSE;/*debug is false*/
}

void debug_on (void) {
    debug = TRUE;/*debug is true*/
}

/* start of true stack code */
/*--------------------------------------------------------------------------
Function Name:         delete_Stack
Purpose:               The function deallocates all memory in stack and sets
                       pointer to null.
Description:           The function first checks if the address exists or not.
                       If not, the nonexist error message is displayed. The
		       memory is deallocated with free and then the print
		       message prints which stack is deallocated. The stack
		       counter is then decremented. 
Input:                 *spp is the address of the stack
Result:                The stack is deleted, memory is freed and the stack
                       counter is decremented.
                       No return value.
--------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {  
    Stack ** freeMemory;

    /* if the pointer does not exist then there is nothing to delete and it 
     * will return as false*/  
    if (*spp==NULL|| spp==NULL){
	fprintf(stderr, DELETE_NONEXIST);/*nonexist error message displayed*/
    }
    freeMemory= (Stack **)(*spp-STACK_OFFSET);
    free(freeMemory);/*frees the memory*/
    *spp= NULL;/*sets pointer to NULL, removes authority*/
    
    /*if debug is on then debug message will print*/    
    if (debug){
	fprintf(stderr, DEALLOCATE, (long)stack_counter);/*prints which stack
	                                                     is deallocated*/
    }
    --stack_counter;/*decrement stack counter*/
}

/*--------------------------------------------------------------------------
Function Name:         empty_Stack
Purpose:               This function empties the stack.
Description:           To empty the stack, the function sets the index back to
                       0 so it will overwrite the previous stack
Input:                 this_Stack is a stack pointer
Result:                The stack is emptied and index set back to 0.
                       No return value.
--------------------------------------------------------------------------*/
void empty_Stack (Stack * this_Stack) {
    /* if the stack does not exist then there is nothing to empty*/  
    if (this_Stack==NULL){
        fprintf(stderr, EMPTY_NONEXIST);/*nonexist error message is displayed*/
    }
    this_Stack[STACK_POINTER_INDEX]=0;/*empties stack by setting index to 0*/ 
}
        
/*--------------------------------------------------------------------------
Function Name:         isempty_Stack
Purpose:               Checks if the stack is empty or not.
Description:           The function first checks if the stack exists. If not,
                       then it will return . If the index is at 0, then
		       there are no values in the stack and so it is empty,
		       returning true. Else, it returns as false. 
Input:                 this_Stack is a stack pointer 
Result:                Returns true if empty and false for not empty.
                       Return value is long.
--------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_Stack) {
    /* if the stack does not exist then there is nothing to check and it will
     * return as false*/  
    if (this_Stack==NULL){
        fprintf(stderr, ISEMPTY_NONEXIST);/*nonexist error message displayed*/
	return 0;/*returns false*/
    }
    /* if the index is equal to 0 then there are no items in the stack and
     * it will return as true*/
    if (this_Stack[STACK_POINTER_INDEX]==0){/*if next free index is at 0, 
	                                    the stack is empty*/
        return 1; /*return true since it is empty*/
    }
    else{
	return 0;/*returns false since it is not empty*/
    }
}

/*--------------------------------------------------------------------------
Function Name:         isfull_Stack
Purpose:               Checks if the stack is full or not.
Description:           The function first checks if the stack exists. If not,
                       then it will return as false. If the index is equal to
		       the size of the stack, then it is full and returns true.
		       Else, it returns as false. 
Input:                 this_Stack is a stack pointer
Result:                Returns true if full and false for not full.
                       Return value is long.
--------------------------------------------------------------------------*/
long isfull_Stack (Stack * this_Stack) {
    /* if the stack does not exist then there is nothing to check and it will
     * return as false*/  
    if (this_Stack==NULL){
        fprintf(stderr, ISFULL_NONEXIST);/*nonexist error message displayed*/
	return 0;/*returns false since it is not full*/
    }

    /* if the index is equal to the size of the stack then the stack is full
     * and it will return as true*/
    if (this_Stack[STACK_SIZE_INDEX]==this_Stack[STACK_POINTER_INDEX] &&
	this_Stack[STACK_POINTER_INDEX]!=0){
        return 1; /*return true since it is full*/
    }
    else{
	return 0;/*returns false since it is not full*/
    }
}

/*--------------------------------------------------------------------------
Function Name:         new_Stack
Purpose:               Initializes a new stack and allocates memory.
Description:           The stack is created by putting the count index 
                       information in index -3, size information in index -2,
		       and pointer information in index -1. The offset is to
		       have room for these 3. Memory is allocated by adding the
		       size of the stack with offset and then multiplying it by
		       the size of bytes. The debug message shows which stack
		       is allocated.
Input:                 stacksize is the size of the stack
Result:                Returns pointer to stack
                       Return value is stack pointer.
--------------------------------------------------------------------------*/
Stack * new_Stack (unsigned long stacksize) {
    void * memory;/*holds the return value of malloc*/
    Stack * this_stack;/*the Stack* to be returned*/

    memory=malloc((stacksize+STACK_OFFSET)*sizeof(stacksize));/*allocating 
                                    memory by adding the size of the stack with
				    offset and then multiplying it by the size
				    of bytes*/
    this_stack=(Stack *)memory + STACK_OFFSET;/*creates stack*/
    this_stack[STACK_POINTER_INDEX]=0;/*pointer information is put in index
                                       -1*/
    this_stack[STACK_SIZE_INDEX]=stacksize;/*size information is put in 
                                             index -2*/
    stack_counter++;/*increment stack counter*/
    this_stack[STACK_COUNT_INDEX]=stack_counter;/*stack counter put in index
                                                  -3*/
    /*if debug is on then debug message will print*/    
    if (debug){
        fprintf(stderr, ALLOCATED, (long)stack_counter);/*debug message showing
	                                            which stack was allocated*/
    }
    return this_stack;/*stack * is returned*/
}

/*--------------------------------------------------------------------------
Function Name:         num_elements
Purpose:               Shows the number of elements in the stack.
Description:           The function first checks if the stack exists. If not,
                       then it will return . If the stack exists, the value
		       of the pointer AKA the number of elements is returned 
Input:                 this_Stack is a stack pointer
Result:                The number of elements is returned.
                       Return value is long.
--------------------------------------------------------------------------*/
long num_elements (Stack * this_Stack) {
    /* if the stack does not exist then there are no elements and so it will
     * return as false*/  
    if (this_Stack==NULL){
        fprintf(stderr, NUM_NONEXIST);/*nonexist error message is displayed*/
    }
    return this_Stack[STACK_POINTER_INDEX];/*returns the number of elements*/
}

/*--------------------------------------------------------------------------
Function Name:         pop
Purpose:               Pops an item off the top of the stack and sets it as 
                       item.
Description:           The function first checks if the stack exists and if the
                       stack is empty. If either case is true, then there is no
		       item to pop off the stack, and so the function returns
		       as false. Else, the pointer to item is set as the item
		       on the previous index (since there is nothing on the
		       current index)and the print message displays the stack
		       number and *item. The index decrements and the function
		       returns true for success. 
Input:                 this_Stack is the pointer to the particular stack
                       item is the pointer to a long with the popped item's 
		       value
Result:                The return value is true for success and false for 
                       failure.
                       Return value is long.
--------------------------------------------------------------------------*/
long pop (Stack * this_Stack, long * item) {
    /* if the stack does not exist then there is nothing to pop and it will
     * return as false*/  
    if (this_Stack==NULL){
	fprintf(stderr, POP_NONEXIST);/*nonexist error message is displayed*/
	return 0;/*returns false for failure*/
    }
    /*if the item parameter does not point to anything it will return as
     * false*/
    if (item== NULL){
	fprintf(stderr, INCOMING_NONEXIST);/*print the error message*/
	return 0;/*returns false for failure*/
    }

    /* if the stack is empty then there is no item to pop and it will return as
     * false*/
    if (isempty_Stack(this_Stack)==1){
	fprintf(stderr, POP_EMPTY);/*prints the empty error message*/
        return 0;/*returns false for failure*/
    }
    *item= this_Stack[(this_Stack[STACK_POINTER_INDEX])-1];/*sets pointer to 
                                        item as the item on top of the stack*/
    /*if debug is on then debug message will print*/    
    if (debug){
        fprintf(stderr, POP, (long)stack_counter, *item);/*prints thes stack 
	                        being affected and the top item being popped*/
    }
    this_Stack[STACK_POINTER_INDEX]--;/*item is popped; index decrements*/
    return 1;/*returns true for success*/
}

/*--------------------------------------------------------------------------
Function Name:         push
Purpose:               Adds item to the top of the stack
Description:           The function first checks if the stack exists and if the
                       stack is full. If either of these two cases is true, it
		       is impossible to push and add an item so the function
		       returns as false. Else, item is added to the stack's
		       current index and then the index increments. The print
		       message prints the stack number and the value of the
		       item being pushed. It then returns true for success.
Input:                 this_Stack is the pointer to the particular stack
                       item is the item to be pushed onto stack
Result:                The item is pushed on top of the stack
                       Return value is long.
--------------------------------------------------------------------------*/
long push (Stack * this_Stack, long item) {
    /* if the stack does not exist then there is no stack to push and it will
     * return as false*/  
    if (this_Stack==NULL){
	fprintf(stderr, PUSH_NONEXIST);/*nonexist error message is displayed*/
	return 0;/*returns false for failure*/
    }
    /* if the stack is full then no more items can be pushed on top and it will
     * return as false*/
    if (isfull_Stack(this_Stack)==1){
	fprintf(stderr, PUSH_FULL);/*prints the full error message*/
	return 0;/*returns false for failure*/
    }

    this_Stack[this_Stack[STACK_POINTER_INDEX]]= item;/*item is put onto the
                                                            top of the stack*/
    (this_Stack[STACK_POINTER_INDEX])++;/*index increments*/
    /*if debug is on then debug message will print*/    
    if (debug){
        fprintf(stderr, PUSH, (long)stack_counter, item);/*prints the stack 
	       being affected and the item that was pushed on top of the
	       stack*/
    }
    return 1;/*returns true for success*/

}

/*--------------------------------------------------------------------------
Function Name:         top
Purpose:               Sends back the top item of the stack but does not pop it
                       off.
Description:           The function first checks if the stack exists and if it
                       is empty. If it is either of the two, there is no top
		       item and so the function returns as false. Else, the
		       pointer to item is set as the top item of the stack and
		       the number of the stack and value of the top item is
		       displayed. It then returns true for success.
Input:                 this_Stack is the pointer to the particular stack
                       item is the pointer to a long with the topped item's 
		       value.
Result:                Returns true for success and false for failure.
                       Return value is long.
--------------------------------------------------------------------------*/
long top (Stack * this_Stack, long * item) {
    /* if the stack does not exist then there is nothing to top and it will
     * return as false*/  
    if (this_Stack==NULL){
	fprintf(stderr, TOP_NONEXIST);/*nonexist error message is displayed*/
	return 0;/*returns false for failure*/
    }
    /*if the item parameter does not point to anything it will return as
     * false*/
    if (item== NULL){
	fprintf(stderr, INCOMING_NONEXIST);/*print the error message*/
	return 0;/*returns false for failure*/
    }

    /* if the stack is empty then there is no value at the top of the stack
     * and it will return as false*/
    if (isempty_Stack(this_Stack)==1){
	fprintf(stderr, TOP_EMPTY);/*empty error message is displayed*/
        return 0;/*returns false for failure*/

    }
    *item= this_Stack[(this_Stack[STACK_POINTER_INDEX])-1];/*pointer to item is
                                          set as the item on top of the stack*/
    /*if debug is on then debug message will print*/
    if (debug){
        fprintf(stderr, TOP, (long)stack_counter, *item);/*prints the stack and
	                                                           top number*/
    }
    return 1;/*returns true for success*/

}

FILE * write_Stack (Stack * this_Stack, FILE * stream) {

    long index = 0;         /* index into the stack */

    if (this_Stack == NULL) {
        fprintf (stderr, WRITE_NONEXIST_STACK);
        return stream;
    }

    if (stream == NULL) {
        fprintf (stderr, WRITE_NONEXIST_FILE);
        return stream;
    }
                
    if (stream == stderr)
        fprintf (stream, "Stack has %ld items in it.\n",
                 num_elements (this_Stack));

    for (index = STACK_COUNT_INDEX + STACK_OFFSET;
        index < num_elements (this_Stack); index++) {

        if (stream == stderr)
            fprintf (stream, "Value on stack is |0x%lx|\n",
                    this_Stack[index]);
        else {
            if (this_Stack[index] < 0)
                fprintf (stream, "%c ",
                        (char) this_Stack[index]);
            else
                fprintf (stream, "%ld ", this_Stack[index]);
        }
    }
        return stream;
}
