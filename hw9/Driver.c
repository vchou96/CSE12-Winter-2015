/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        3/12/2015 
                                                        cs12xkf 
                                Assignment Nine 
 
File Name:      Driver.c
Description:    This driver program contains the main method that runs the
                Tree.c program. It will ask for command input from the user and
                return and display the information of the binary tree on the
                disk corresponding to the command through cout. The driver
                takes ASCII file input as well as binary file input and output.
                At ^D the cost and number of operations will be displayed, as
                well as the average of accesses over operations.
****************************************************************************/ 
#include <iostream>
#include <cstdio>
#include <string>
#include <getopt.h>
#include "Driver.h"
#include "SymTab.h"
//#include <fstream>

using namespace std;

/* DO NOT CHANGE:  This file is used in evaluation */

#ifdef NULL
#undef NULL
#define NULL 0
#endif

ostream & operator << (ostream & stream, const UCSDStudent & stu) {
    return stream << "name:  " << stu.name
        << " with studentnum:  " << stu.studentnum;
}
/*************************************************************************
* Name: main
* File: Driver.c
* Purpose: Runs the Tree.c program by asking for input from user and directing
* the input to buffer and the UCSDStudent constructor. The 'f' case is to
* switch input to file. Outside the switch cases is a statement to
* handle end of input from file/keyboard. 
************************************************************************/
int main (int argc, char * const * argv) {
    char buffer[BUFSIZ];
    char command;
    long number;
    char option;
    
    istream* is;/*declare istream pointer*/
    ostream* os;/*declare ostream pointer*/
    
    is=&cin;/*assign is to address of cin*/
    os=&cout;/*assign os to address of cout*/
    
    SymTab<UCSDStudent>::Set_Debug_Off ();

    while ((option = getopt (argc, argv, "x")) != EOF) {

        switch (option) {
            case 'x': SymTab<UCSDStudent>::Set_Debug_On ();
            break;
        }
    }
        
    SymTab<UCSDStudent> ST("Driver.datafile");
    ST.Write (cout << "Initial Symbol Table:\n" );

    while (cin) {/*while there is still input*/
        if (!(*is)){/*if at end of file*/
            if (is==&cin){/*if 'is' is equal to the address of cin, so if
                            input is coming from keyboard*/
                break;/*break out of while loop*/
            }
            else{
                delete(is);/*delete istream pointer*/
                delete(os);/*delete ostream pointer*/
                is=&cin;/*assign is to address of cout*/
                os=&cout;/*assign os to address of cout*/
            }
        }


	    command = NULL;         // reset command each time in loop
        *os << "Please enter a command ((f)ile, (i)nsert, "
            << "(l)ookup, (r)emove, (w)rite):  ";/*ask for input from user*/

        *is >> command;/*direct the contents of the input to command*/

        switch (command) {/*switch cases for commands*/

            case 'f': {/*case if 'f' is inputted*/
	        *os << "Please enter file name for commands: ";/*ask for
                                                                 command input
                                                                 from user*/
                *is >> buffer;/*direct input to buffer*/
                if (is!=&cin){/*if no input from keyboard*/
                    delete(is);/*delete istream pointer*/
                    delete(os);/*delete ostream pointer*/
                }
                is=new ifstream(buffer);/*assign pointer to a new ifstream with
                                          buffer passed in*/
                os= new ofstream("/dev/null");/*assign pointer to a new
                                                ofstream so output goes to
                                                "dev/null"*/
                break;/*break out of case*/
            }	
            case 'i': {
                *os << "Please enter UCSD student name to insert:  ";/*ask for
                                                                       name
                                                                       input
                                                                       from
                                                                       user*/

                *is >> buffer;/*direct input to buffer*/

                *os << "Please enter UCSD student number:  ";/*ask for student
                                                               number input
                                                               from user*/
                *is >> number;/*direct input to number*/

                UCSDStudent stu (buffer, number);

                // create student and place in symbol table
                ST.Insert (stu);
                break;
            }
            case 'l': { 
                unsigned long found;    // whether found or not

                *os << "Please enter UCSD student name to lookup:  ";/*ask for
                                                                       name
                                                                       input
                                                                       from
                                                                       user*/
                *is >> buffer;/*direct input to buffer*/

                UCSDStudent stu (buffer, 0);
                found = ST.Lookup (stu);

                if (found)
                    cout << "Student found!!!\n" << stu << "\n";
                else
                    cout << "student " << buffer << " not there!\n";
                break;
            }
            case 'r': { 
                unsigned long removed;

                *os << "Please enter UCSD student name to remove:  ";/*ask for
                                                                       name
                                                                       input
                                                                       from
                                                                       user*/
                *is >> buffer;/*direct input to buffer*/

                UCSDStudent stu (buffer, 0);
                removed = ST.Remove(stu);

                if (removed)
                    cout << "Student removed!!!\n" << stu << "\n";
                else
                    cout << "student " << buffer << " not there!\n";
                break;
            }
            case 'w':
                ST.Write (cout << "The Symbol Table contains:\n");
        }
    }

    ST.Write (cout << "\nFinal Symbol Table:\n");
    
    if (ST.GetOperation() != 0) {/*if the value of operation is not 0*/
        cout << "\nCost of operations: ";
        cout << ST.GetCost();/*prints the cost of operations*/
        cout << " tree accesses";

        cout << "\nNumber of operations: ";
        cout << ST.GetOperation();/*prints the number of operations*/

        cout << "\nAverage cost: ";
        cout << (((float)(ST.GetCost()))/(ST.GetOperation()));/*prints the cost
                                                                divided by
                                                                number of
                                                                operations*/
        cout << " tree accesses/operation\n";
    }

    else
        cout << "\nNo cost information available.\n";

}
