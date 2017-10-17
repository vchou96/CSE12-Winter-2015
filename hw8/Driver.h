#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;

class UCSDStudent {
    friend ostream & operator << (ostream &, const UCSDStudent &);
    char name[20];/**/
    long studentnum;

public:
    UCSDStudent (char * nm, long val = 0) : studentnum (val) {
        strcpy (name, nm);
    }
    operator const char * (void) const {
        return name;
    }
    long operator == (const UCSDStudent & uuu) const {
        return ! strcmp (name, uuu.name);
    }
    long operator > (const UCSDStudent & uuu) const {
        return (strcmp (name, uuu.name) > 0) ? 1 : 0;
    }
};

#endif
