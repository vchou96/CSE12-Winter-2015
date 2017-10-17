/****************************************************************************

                                                        Vanessa Chou
                                                        CSE12, Winter 2015
                                                        1/9/2015
                                                        cs12xkf
                                Assignment One

File Name:      hw1.c
Description:    This program tests functions to display output strings and 
                numbers. 
****************************************************************************/
#include <stdio.h>

#define COUNT 8		/* number of hex digits to display */
#define DECIMAL 10	/* to indicate base 10 */
#define HEX 16		/* to indicate base 16 */

/* array used for ASCII conversion */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*--------------------------------------------------------------------------
Function Name:         baseout
Purpose:               Converts a number into a base passed in and displays it
                       to stream.
Description:           The algorithm converts the number to its new base by
                       modding the number by the base and then dividing the
                       number by the base. This process is repeated until the
                       result of dividing is 0. All the remainders from the
		       modding are put into the digits array to find the
		       corresponding characters and then are put into a new
		       array. If the base is 16, the algorithm will fill up
		       remaining spaces in the array with 0's so there are 8
		       digits. The characters of the array are then printed
		       backward so that they are in the right order.
Input:                 number is the number to be converted.
                       base the new base of the number
		       stream is where to display, usually stdout or stderr
Result:                The number converted into its new base. No return value.
--------------------------------------------------------------------------*/
void baseout (int number, int base, FILE * stream) {
    int newInt; /*the new value of number each time it is divided by base*/
    char str[] = "0"; /*used to fill in empty spaces with 0's for HEX case*/
    char chars[COUNT]; /*after conversion each character of the new number
                         is put here*/
    int hexCounter = 0; /*index for chars[]*/
    int index=0;//index for chars[]

    newInt= number;/*newInt takes the value of number*/

    /*newInt is modded by base and hexCounter increments*/
    chars[hexCounter]=digits[newInt%base];
    hexCounter++;
    
    /*divides newInt by base, mods the newInt by base, and loops until
     * newInt is 0*/
    while ((newInt/base!=0)){
        newInt= newInt/base;
        chars[hexCounter]=digits[newInt%base];
        hexCounter++;
    }

    /*if the base is 16 then 0's will be added to make 8 digits*/
    if (base==HEX){
	/*fills the array with 0's and loops until there are 8 digits*/
        while(hexCounter<COUNT){
            chars[hexCounter]= str[0];
            hexCounter++;
        }
    }

    /*loops to print the chars[] backward so the characters are in the right
    order*/
    while(hexCounter!=0){
        fputc(chars[hexCounter-1], stream);
        hexCounter--;
    }
}


/*--------------------------------------------------------------------------
Function Name:         decout
Purpose:               Displays a number in decimal form.
Description:           decout calls on baseout and passes in the number, base 
                       (DECIMAL) and stream. Baseout will convert the number 
		       into decimal form.
Input:                 number is the number to display.
                       stream is where to display, usually in stdout or stderr.
Result:                Number in base 10 is displayed.
                       No return value.
--------------------------------------------------------------------------*/
void decout (unsigned int number, FILE * stream) {
    baseout(number, DECIMAL, stream);/*calls on baseout and passes in the 
                                       number, 10 as base, and stream*/
}


/*--------------------------------------------------------------------------
Function Name:         writeline
Purpose:               Displays the message passed in to string.
Description:           This function first prints the character at messages's 0
                       index and loops this process with the index incrementing
		       each time until the entire message is printed.
Input:                 message is what to display.
                       stream is where to display, usually stdout or stderr.
Result:                All chars of message are printed.
                       Return value is int.
--------------------------------------------------------------------------*/
int writeline (const char * message, FILE * stream) {
    int index=0; /*index for the message char array*/
    
    /*loops to print a character at a time until the end of message is reached*/
    while (message[index]){
        fputc(message[index], stream);
        index++;
    }
    return index;/*returns index as the int*/
}


/*--------------------------------------------------------------------------
Function Name:         hexout
Purpose:               Prints a number in base 16 to the parameter FILE stream
Description:           Goal is achieved via delegating to the baseout function
Input:                 number:  the number to display
                       stream:  where to display, likely stdout or stderr
Result:                Number in base 16 is displayed.
                       No return value.
--------------------------------------------------------------------------*/
void hexout (unsigned int number, FILE * stream) {

    /* Output "0x" for hexidecimal. */
    writeline ("0x", stream);
    baseout (number, HEX, stream);/*calls on baseout and passes in the number,
                                    16 for base, and stream*/
}


/*--------------------------------------------------------------------------
Function Name:         newline
Purpose:               Prints a newline character to stream.
Description:           The newline char '\n' is added to stream.
Input:                 stream is where to display, usually in stdout or stderr.
Result:                A newline is printed.
                       No return value.
--------------------------------------------------------------------------*/
void newline (FILE * stream) {
    fputc('\n', stream);/*adds a newline to stream*/
}


int main (int argc, char *const* argv) {
    writeline ("Hello World", stdout);/*calls writeline and passes 
	                                "Hello World" as message*/
    fprintf (stderr, "Zdravei Sviat\n");
    newline(stdout);/*calls newline*/ 
    decout (123, stdout);/*calls decout and passes in 123 as number*/
    newline(stdout);/*calls newline*/
    decout (0, stdout);/*calls decout and passes in 0 as number*/
    newline(stdout);/*calls newline*/
    hexout (0xFEEDDAD, stdout);/*calls hexout and passes in 0xFEEDDAD 
	                         as number*/
    newline(stdout);/*calls newline*/
    return 0;
}
