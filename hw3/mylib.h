#ifndef MYLIB_H
#define MYLIB_H

/* DO NOT CHANGE:  This file is used in evaluation */

void clrbuf (int);
long decin (void);
void decout (long number);
void hexout (unsigned long);
void newline (FILE *);
long writeline (const char *, FILE *);
int getopt(int argc, char * const argv[], const char *optstring);

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif

