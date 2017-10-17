#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include "Base.h"
#include "SymTab.h"
using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

/**
* Driver.c holds and updates the names and numbers of the students. Its methods
* will return the name and numbers of the students for use in Hash.c and checks
* if a passed in student is the same as this student. main calls methods from
* Hash.c to put the students in a hashTable.
*/


class UCSDStudent : public Base {
	char * name;/*holds student name*/
	long studentnum;/*the student number*/
public:
    UCSDStudent (char * nm, long sn) :/*UCSDStudent constructor*/
	name (strdup (nm)), studentnum (sn) {}/*sets name as nm and student
	                                        as sn*/
    ~UCSDStudent (void) {/*destructor for UCSDStudent*/
        free(name);/*frees the memory associated with name*/
    }
    UCSDStudent (UCSDStudent & ucsdstudent) {/*copy constructor*/
	name = strdup (ucsdstudent.name);/*sets name as ucsdstudent's name*/
	studentnum = ucsdstudent.studentnum;/*sets studentnum as ucsdstudent's
	                                      studentnum*/
    }

    operator char * (void) {/*gets the name of the student and returns*/
        return name;/*returns the pointer to the name of the student*/
    }
    operator long (void);
    long operator == (Base & base) {/*acts as equals method by checking if base
                                      is same as name */
	return ! strcmp (name, base);/*returns if name and base are equivalent
	                               or not*/
    }

        ostream & Write (ostream & stream) {
	return stream << "name: " << name
	    << " Studentnum: " << studentnum;
    }

};

UCSDStudent :: operator long (void) {/*gets the ASCII sum of name and returns
                                       it*/
	long sum = 0;/*holds the numerical value of name*/
	
	for (long index = 0; name[index]; index++)/*loops through the length of
		                                    name*/
		sum += name[index];/*adds the char at index each time*/

	return sum;/*returns the complete numerical value*/
}



int main (int argc, char * const * argv) {
	SymTab ST (5);
	char buffer[128];
	char command;
	long number;
	char option;

	/* initialize debug variable */
	HashTable::Set_Debug(0);

	 /* check command line options for debug display */
	 while ((option = getopt (argc, argv, "x")) != EOF) {

		 switch (option) {
			 case 'x': HashTable::Set_Debug(1);
				 break;
		 	}
	} 

	ST.Write (cout << "Initial Symbol Table:\n");
	while (cin) {
		command = NULL;		// reset command each time in loop
		cout <<
		   "Please enter a command:  ((i)nsert, (l)ookup, (w)rite):  ";
		cin >> command;

		switch (command) {
		case 'i':
			cout << "Please enter UCSD student name to insert:  ";
			cin >> buffer;	// formatted input

			cout << "Please enter UCSD student number:  ";
			cin >> number;

			// create student and place in symbol table
			if(!ST.Insert (new UCSDStudent (buffer, number))){

				ST.Write (cout << "\nFinal Symbol Table:\n");
				exit (0);
			}
			break;

		case 'l': {
			Base * found;	// whether found or not

			cout << "Please enter UCSD student name to lookup:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			found = ST.Lookup (& stu);
			
			if (found)
				found->Write (cout << "Student found!!!\n") <<
				              "\n";
			else
				cout << "Student " << buffer <<
				    " not there!\n";
			}
			break;

		case 'w':
			ST.Write (cout << "The Symbol Table contains:\n");
		}
	}
	ST.Write (cout << "\nFinal Symbol Table:\n");
}

