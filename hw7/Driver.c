#include <cstdlib>
#include <iostream>
#include <string.h>
#include "Base.h"
#include "SymTab.h"
using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

/**
* Driver.c holds and updates the names and numbers of the students. Its methods
* will return the name and numbers of the students for use in Tree.c and checks
* if a passed in student is the same as this student.*/

class UCSDStudent : public Base {
    char * name;/*holds student name*/
    long studentnum;/*holds student number*/
public:

    UCSDStudent (char * nm, long val = 0) :
        name (strdup (nm)), studentnum (val) {}/*UCSD constructor*/
    UCSDStudent (const UCSDStudent & ucsdstudent) {/*copy constructor*/
        name = strdup (ucsdstudent.name);/*sets name as ucsdstudent's name*/
        studentnum = ucsdstudent.studentnum;/*sets studentnum as ucsdstudent's
	                                      studentnum*/
    }
    ~UCSDStudent (void) {/*destructor for UCSDStudent*/
        free (name);/*frees name memory*/
    }
    operator char * (void) const {/*gets name of student*/
        return name;/*returns the name*/
    }
    long operator == (const Base & bbb) const {/*checks if parameters are
                                                 equal*/
        return ! strcmp (name, bbb);/*returns if equal or not*/
    }
    long operator > (const Base & bbb) const {/*checks which one is greater*/
        return (strcmp (name, bbb) > 0) ? 1 : 0;/*returns if greater or not*/
    }
    ostream & Write (ostream & stream) const{
	return stream << "name:  " << name
            << "  studentnum:  " << studentnum;
    }
};

int main (int argc, char * const * argv) {
	char buffer[80];
	char command;
	long number;

	Tree::Set_Debug(0);

	if (argc != 1 && strcmp("-x", argv[1]) == 0) {
		Tree::Set_Debug(1);
	}
	
	SymTab * ST;
	ST = new SymTab ("UCSDStudentTree");
	ST->Write (cout << "Initial Symbol Table:\n");

	while (cin) {
		command = NULL;		// reset command each time in loop
		cout <<  "Please enter a command:  ((a)llocate, (i)nsert, " <<
			"(l)ookup, (r)emove, (w)rite):  ";
		cin >> command;

		switch (command) {

		case 'a':
			cout << "Please enter name of new Tree to allocate:  ";
			cin >> buffer; 

			delete ST;
			ST = new SymTab (buffer);

			break;

		case 'i':
			cout << "Please enter UCSD student name to insert:  ";
			cin >> buffer;	// formatted input

			cout << "Please enter UCSD student number:  ";
			cin >> number;

			// create student and place in symbol table
			ST->Insert (new UCSDStudent (buffer, number));
			break;

		case 'l': {
			const Base * found;	// whether found or not

			cout << "Please enter UCSD student name to lookup:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			found = ST->Lookup (&stu);
			
			if (found)
				found->Write (cout << "Student found!\n") << "\n";
			else
				cout << "student " << buffer << " not there!\n";
			}
			break;
		
		case 'r': {
			Base * removed;	// data to be removed

			cout << "Please enter UCSD student name to remove:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			removed = ST->Remove (&stu);

			if (removed)
				removed->Write (cout << "Student removed!\n") << "\n";
			else
				cout << "student " << buffer << " not there!\n";
			}
			break;


		case 'w':
			ST->Write (cout << "The Symbol Table contains:\n");
		}
	}

	ST->Write (cout << "\nFinal Symbol Table:\n");
	delete ST;
}
