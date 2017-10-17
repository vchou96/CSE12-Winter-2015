/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        3/12/2015 
                                                        cs12xkf 
                                Assignment Nine 
 
File Name:      Driver.h 
Description:    This driver program contains UCSDStudent class methods to
                create the nodes that will be inserted into the binary tree of
                Tree.c. The << > == and const char * methods are provided to
                compare the name values of UCSDStudent objects in order for the
                nodes to be inserted in the correct order. Each UCSDStudent
                object has its own name of up to 8 chars and a studentnum.
****************************************************************************/ 
#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;
/****************************************************************************
* Name: UCSDStudent (class)
* File: Driver.h
* Purpose: Contains constructors and member methods for UCSDStudent objects.
* Initializes the name and studentnum attributes of UCSDStudent objects and
* provdes << > == (const char *) methods.
*****************************************************************************/ 
class UCSDStudent {
    friend ostream & operator << (ostream &, const UCSDStudent &);
    char name[8];/*char array to hold name of UCSDStudent object. Limit of 8
                   chars*/
    long studentnum;/*the student number of the UCSDStudent object*/

public:
    UCSDStudent (char * nm, long val = 0) : studentnum (val) {/*constructor for
                                                                UCSDStudent*/
        memset (name, '\0', sizeof (name));/*fills bytes of memory for name*/
	    strcpy (name, nm);/*copies nm to name*/
    }
    UCSDStudent (void) : studentnum (0) {/*constructor for UCSDStudent*/
	    memset (name, '\0', sizeof (name));/*fills bytes of memory for name*/
    }
    UCSDStudent (const UCSDStudent & ucsdstudent) {/*copy constructor for
                                                     UCSDStudent*/
        memset (name, '\0', sizeof (name));/*fills bytes of memory for name*/
        strcpy (name, ucsdstudent.name);/*copies ucsdstudent's name to name*/
        studentnum = ucsdstudent.studentnum;/*sets studentnum as ucsdstudent's
                                              studentnum*/
    }
    operator const char * (void) const {/*const char * method*/
        return name;/*returns the name of UCSDStudent object*/
    }
    long operator == (const UCSDStudent & uuu) const {/*== method to compare if
                                                        2 names are the same*/
        return ! strcmp (name, uuu.name);/*returns whether name values for
                                           current UCSDStudent and uuu are the
                                           same*/
    }
    long operator > (const UCSDStudent & uuu) const {/*> method to compare
                                                       which name's value is
                                                       greater*/
        return (strcmp (name, uuu.name) > 0) ? 1 : 0;/*returns whether value
                                                       for name of current
                                                       UCSDStudent is greater
                                                       than that of uuu*/
    }
};

#endif
