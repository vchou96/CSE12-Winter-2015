/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        1/20/2015 
                                                        cs12xkf 
                                Assignment Three 
 
File Name:      driver.c 
Description:    This file will call the functions of stack.c to manipulate
                stacks and associate different commands with different switch
		cases. The possible commands are allocate, deallocate, push,
		pop, top, isempty, empty, isfull, num_elements, write to
		stdout, Write to stderr.
   
****************************************************************************/  
#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "stack.h"

/*1. The address is 0x804b008.
 *2. The address returned by new_Stack is 0x804b014.
 *3. The address is on the heap.
 *4. The value of spp is 0xbfffecdc. The value is on the run time stack.
 *5. The value of *spp is 0x804b014. The address is on the heap.
 *6. 0x804b008 is passed to free().
 *7. Yes they are both the same: 0x804b008.*/



int main (int argc, char * const * argv) {

    Stack * main_Stack = 0;         /* the test stack */
    unsigned long amount;        /* numbers of items possible go on stack */
    long command;                   /* stack command entered by user */
    long item = 0;                  /* item to go on stack */
    char option;                    /* the command line option */
    long status;                    /* return status of stack functions */
        
    /* initialize debug states */
    debug_off ();

    /* check command line options for debug display */
    while ((option = getopt (argc, argv, "x")) != EOF) {
                
        switch (option) {
            case 'x': debug_on (); break;
        }
    }

    while (1) {
        command = 0;            /* initialize command, need for loops */
        writeline ("\nPlease enter a command:", stdout);
        writeline ("\n\t(a)llocate, (d)eallocate, ", stdout);
        writeline ("p(u)sh, (p)op, (t)op, (i)sempty, (e)mpty, ",stdout);
        writeline ("\n\tis(f)ull, (n)um_elements, (w)rite to stdout, "
                                                                , stdout);
        writeline ("(W)rite to stderr.\n", stdout);
        writeline ("Please enter choice:  ", stdout);
	command = getchar ();
        if (command == EOF)     /* are we done? */
            break;
        clrbuf (command);       /* get rid of extra characters */

        switch (command) {      /* process commands */

        case 'a':  /*new stack creation*/
            writeline(
	        "\nPlease enter the number of objects to be able to store: ",
	                                                             stdout);
	                 /*asks for the user to input the size of the stack*/
	    amount= decin();/*sets item as the number inputted*/
	    
	    /*if a stack has already been created, delete the old one since
	     * there can only be one at a time*/
            if (main_Stack!= 0){/*if main_Stack is not 0 and so a stack exists
		                  already*/
		delete_Stack(&main_Stack);/*call the delete method*/
		main_Stack=0;/*set main_Stack back to 0*/
	    }

	    main_Stack= new_Stack(amount);/*sets main_Stack as the new
		                                    stack with size item*/
	    clrbuf(0);/*clears the buffer*/
            break;/*breaks from case*/
		
	case 'd':  /*deallocate stack*/
	    delete_Stack(&main_Stack);/*calls delete method and passes in
	                                address to main_Stack*/
	    writeline ("Stack has been deleted.\n", stdout);/*prints that the
	                                              Stack is empty*/

	    break;/*breaks from case*/
	        
        case 'e':  /*empty stack*/
	    empty_Stack(main_Stack);/*calls the empty stack method and
		                              passes in the main stack*/
	    writeline ("Stack is empty.\n", stdout);/*prints that the
		                                      Stack is empty*/
	    break;/*breaks from case*/

        case 'f':               /* isfull */
            if (isfull_Stack (main_Stack))
                writeline ("Stack is full.\n",stdout);
            else
                writeline ("Stack is not full.\n", stdout);
            break;

        case 'i':               /* isempty */
            if (isempty_Stack (main_Stack))
                writeline ("Stack is empty.\n", stdout);
            else
                writeline ("Stack is not empty.\n", stdout);
            break;

        case 'n':               /* num_elements */
            writeline ("Number of elements on the stack is:  ",
                                                       stdout);
            decout (num_elements (main_Stack));
            newline (stdout);
            break;

        case 'p':               /* pop */
            status = pop (main_Stack, &item);
            if (! status)
                fprintf (stderr,"\nWARNING:  pop FAILED\n");
            else {
                writeline (
                       "Number popped from the stack is:  ",
                                                    stdout);
                decout (item);
                newline (stdout);
            }
            break;

        case 't':               /* top */
            status = top (main_Stack, &item);
            if (! status)
                fprintf (stderr,"\nWARNING:  top FAILED\n");
            else {
                writeline (
                         "Number at top of the stack is:  ",
                                                    stdout);
                decout (item);
                newline (stdout);
            }
            break;

        case 'u':               /* push */
            writeline (
                     "\nPlease enter a number to push to stack:  ",
                                                           stdout);
	    item = decin ();
            clrbuf (0);     /* get rid of extra characters */
            status = push (main_Stack, item);
            if (! status)
                fprintf(stderr,"\nWARNING:  push FAILED\n");
            break;

        case 'w':               /* write */
            writeline ("\nThe Stack contains:\n", stdout);
            write_Stack (main_Stack, stdout);
            break;

        case 'W':               /* write */
            writeline ("\nThe Stack contains:\n", stdout);
            write_Stack (main_Stack, stderr);
            break;
        }

               if (item == EOF) /* check if done */
                        break;
        }

        if (main_Stack)
            delete_Stack (&main_Stack);     /* deallocate stack */
        newline (stdout);
        return 0;
}

