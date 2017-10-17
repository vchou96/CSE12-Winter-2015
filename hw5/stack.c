/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        1/28/2015 
                                                        cs12xkf 
                                Assignment Five 
 
File Name:      stack.c 
Description:    This file provides the methods to create and delete stacks as
                well as correctly allocate and deallocate memory. The top, pop,
		and push functions are in relation with checking, popping, and
		pushing items to/from a particular stack. The isempty function
		checks if the stack contains any items or not. The functions
		will call on the functions from list.c to perform the actions.

****************************************************************************/ 
#include <stdio.h>
#include "list.h"
#include "stack.h"


/*--------------------------------------------------------------------------
Function Name:         delete_Stack
Purpose:               The function deallocates all memory in stack and sets
                       pointer to null.
Description:           Calls on delete_List to deallocate memory associated
                       with the stack. spp is passed in as a double pointer to
		       the stack that will be deleted. 
Input:                 spp is double pointer to the stack address
Result:                The stack is deleted and memory is freed.
                       No return value.
--------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {
        delete_List (spp);
}

/*--------------------------------------------------------------------------
Function Name:         isempty_Stack
Purpose:               Checks if the stack is empty.
Description:           Calls on isempty_List to check if this_Stack is empty or
                       not. this_stack points to the stack to be checked.
Input:                 this_Stack is a stack pointer 
Result:                Returns true if empty and false for not empty.
                       Return value is long.
--------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_stack) {
        return isempty_List (this_stack);
}


/*--------------------------------------------------------------------------
Function Name:         new_Stack
Purpose:               Initializes a new stack and allocates memory.
Description:           Calls on new_List to allocate and initialize a new
                       stack. Copy func is a pointer to the function that makes
		       copies of this_stack's elements. Delete func is a
		       pointer to the function that frees memory and
		       is_greater_than func is a pointer to the function that
		       compares the elements. A pointer to the new stack is
		       returned. 
Result:                Returns pointer to new stack
                       Return value is stack pointer.
--------------------------------------------------------------------------*/
Stack * new_Stack (void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) {

        return new_List (copy_func, delete_func, is_greater_than_func, write_func);
}


/*--------------------------------------------------------------------------
Function Name:         pop
Purpose:               Pops an item off the top of the stack and sets it as 
                       item.
Description:           Calls on remove_List to remove an element from
                       this_Stack at the end of the stack. this_stack and END
		       are passed in and a void pointer is returned.
Input:                 this_Stack is the pointer to the particular stack.
Result:                Returns a void pointer to the list from which to remove
                       the element.

--------------------------------------------------------------------------*/
void * pop (Stack * this_stack) {
        return remove_List (this_stack, END);
}


/*--------------------------------------------------------------------------
Function Name:         push
Purpose:               Adds item to the top of the stack
Description:           Calls on insert so that a element will be inserted to
                       the end of this_stack. Passes this_stack as the stack to
		       be checked, element as the pointer to the object to be
		       inserted, and END to signify the end of the stack 
Input:                 this_Stack is the pointer to the particular stack
                       element is a void pointer to the object that will be
		       inserted
Result:                The item is pushed on top of the stack.
                       Return value is long for success.
--------------------------------------------------------------------------*/
long push (Stack * this_stack, void * element) {
        return insert (this_stack, element, END);
}


/*--------------------------------------------------------------------------
Function Name:         top
Purpose:               Sends back the top item of the stack but does not pop it
                       off.
Description:           Calls view to return a pointer to the object stored at
                       the end of the stack. this_Stack is the stack to be
		       checked and END is the end of the stack.
Input:                 this_Stack is the pointer to the particular stack
Result:                Return value is void pointer to the object.
--------------------------------------------------------------------------*/
void * top (Stack * this_stack) {
        return view (this_stack, END);
}


/*--------------------------------------------------------------------------
Function Name:         write_Stack
Purpose:               Writes the elements of the stack.
Description:           Calls on write_List to write the elements of the stack.
                       this_stack is passed as the stack to write and stream 
		       is where to display.
Input:                 this_Stack is the pointer to the particular stack
                       stream is where to display and is either stdout or
		       stderr.
Result:                Return value is FILE pointer to stream.
--------------------------------------------------------------------------*/
FILE * write_Stack (Stack * this_stack, FILE * stream) {
        return write_List (this_stack, stream);
}
