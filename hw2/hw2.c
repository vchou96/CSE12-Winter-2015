/****************************************************************************

                                                        Vanessa Chou
                                                        CSE 12, Winter 2015
                                                        January 8, 2015
                                                        cs12xkf
                                Assignment Two

File Name:      hw2.c
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.  At termination, program
                outputs sizes of various types of C/C++ pre defined types.

****************************************************************************/

/* declare fputc/fgetc */
#include <stdio.h>
#include <getopt.h>

/* define some program-wide constants */
#define ASCII_ZERO '0'
#define COUNT ((int) (sizeof (int) << 1))
#define ERROR -1
#define FALSE 0
#define MAXLENGTH 80
#define OFFSET ('a' - 'A')
#define SPACE ' '
#define TRUE 1

#define DECIMAL 10
#define HEX 16

/* define the keyword "NULL" as 0 */
#ifdef NULL
#undef NULL
#endif
#define NULL 0

/* declarations for functions defined in this file */
void baseout (int number, int base, FILE *stream); 
void clrbuf (int);
int decin (void);
void decout (unsigned int, FILE *);
void digiterror (int, int *, const char *);
int getaline (char *, int);
void hexout (unsigned int, FILE *);
void newline (FILE *);
int writeline (const char *, FILE *);

/* array for input checking and for output */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* messages */
const char CAUSED_OVERFLOW[] =  " caused overflow!!!\n";
const char DIGIT_STRING[] = "digit ";
const char REENTER_NUMBER[] = "\nPlease reenter number: ";
const char OUT_OF_RANGE[] = " out of range!!!\n";

/* debug messages */
const char DEBUG_GETALINE[] =
        "[*DEBUG:  The length of the string just entered is ";
const char DEBUG_WRITELINE[] =
        "\n[*DEBUG:  The length of the string displayed is ";
static int debug_on = FALSE;


/*--------------------------------------------------------------------------
Function Name:         baseout
Purpose:               Converts a number into a base passed in and displays 
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
Result:                The number converted in its new base. No return value.
--------------------------------------------------------------------------*/
void baseout (int number, int base, FILE *stream){
    int newInt; /*the new value of number each time it is divided by base*/
    char str[] = "0"; /*used to fill in empty spaces with 0's for HEX case*/
    char chars[BUFSIZ]; /*after conversion each character of the new number
                         is put here*/
    int index=0;/*index for chars[]*/


    newInt= number;/*newInt takes the value of number*/

    /*loop that divides the number by base, finds the remainder and the
    corresponding char in digits[]. It is then placed in chars[].*/
    while ( newInt != 0 ){
        chars[index] = digits[newInt % base];
        newInt = newInt / base;
        index++;
    }
    /*if base is hexidecimal, 0's will be substituted to ensure 8 digits.*/
    if (base==HEX){
        /*loops until 8 values have been reached in the chars[]*/
        while(index<COUNT){
            chars[index]= str[0];
            index++;
        }
    }

    /*loops to print the chars[] backward so the characters are in the right
    order*/
    while(index!=0){
        fputc(chars[index-1], stream);
        index--;
    }
}



/*-------------------------------------------------------------------------- 
Function Name:          clrbuf
Purpose:                When called, this function will clear stdin.
Description:            This function checks to see if the incoming
                        parameter is already '\n' indicating that stdin
                        is already clear.  If stdin is already clear, this
                        function does nothing.  Otherwise, this function
                        calls "fgetc" in a loop until stdin is clear.
Input:                  character:  the most recent character received from a
                                previous call to fgetc.
Result:                 stdin cleared.  Nothing returned. 
--------------------------------------------------------------------------*/  
void clrbuf (int character) {
    /*loops until newline is reached*/
    while (character!='\n'){
        character=fgetc(stdin);/*clears a character at a time*/
    }
}


/*--------------------------------------------------------------------------
Function Name:          decin
Purpose:                This function accepts integer input from the user.
Description:            This function processes user input in a loop that ends
                        when the user enters either a valid number or EOF.
                        If EOF is entered, EOF is returned.  Otherwise each
                        character entered is checked to verify that it is
                        numeric.  Non-numeric input is identified, the user
                        is notified, reprompted, and the loop begins again.
                        Once the input is verified to be valid, a series
                        of multiplication by 10 and addition can take
                        place to convert the ASCII characters entered into
                        a numeric quantity.
Input:                  None.
Result:                 The number entered or EOF.
--------------------------------------------------------------------------*/
int decin (void) {
    int nextInt=0;/*used to update the numbers inputted*/
    int sum=0;/*the overall sum to be returned*/
    int oldsum=0;/*the old sum used to check for overflow*/

    
    do{
	nextInt= (char)fgetc(stdin);/*sets nextInput to the first
                                      input for decimal*/
        /*if EOF is called while stdin is empty the function returns as
	EOF*/
        if (nextInt==(char)EOF){
            return EOF;
        }
        /*if nextInt is not newline this will loop*/
	if (nextInt!= '\n'){
	    /*if nextInt is in the range of ASCII characters for decimal
	    digits. AKA if nextInt is greater or equal to ASCII character
	    for 0 and less than the one for 9*/
            if (nextInt>=ASCII_ZERO && nextInt < ASCII_ZERO+DECIMAL){
                /*if this is the first input and sum is still 0*/
	        if (sum==0){
		    sum= nextInt-'0';/*nextInt is subtracted by '0' to
		                     convert from ASCII to decimal and
				     set as sum.*/
		    oldsum= sum;/*old sum set as sum*/
	        }
		/*if the number has multiple digits and is not on its first
		input*/
		else{
		    sum=(sum*DECIMAL)+(nextInt - '0');         /*The sum is
		                          multiplied by 10 and has
		  	                  input added to it.*/
						     
                    /*if reverse operations on sum is not the same as
		    oldsum then there was overflow. Digiterror is
		    called to ask for a new input.*/
		    if (sum/DECIMAL != oldsum){
                        digiterror(nextInt, &sum, CAUSED_OVERFLOW);/*calls
			                          digiterror for overflow
			                          case; asks again for
					          input*/
		        sum=0;/*sum resets to 0*/
			oldsum=0;/*oldsum resets to 0*/
		    }
		    else{
                        oldsum= sum;/*oldsum is updated as sum*/
		    }
	        }
            }
	    /*if nextInt is not in the range of ASCII characters*/
            else if (nextInt<ASCII_ZERO || nextInt >= ASCII_ZERO+DECIMAL){
                digiterror(nextInt, &sum, OUT_OF_RANGE);/*call digiterror for
		                   out of range case; asks again for input*/
		sum=0;/*sum reset to 0*/
		oldsum=0;/*oldsum reset to 0*/
	    }
        }
    } while (nextInt!= '\n');/*loops for every digit inputted until newline*/
    return sum;/*returns the total sum*/
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
Function Name:          digiterror
Purpose:                This function handles erroneous user input.
Description:            This function reinitializes sum, displays and
                        error message to the user, and asks for fresh
                        input.
Input:                  character:  The character that began the problem.
                        sum:  A pointer to the sum to reinitialize.
                        message:  The message to display to the user.
Result:                 The message is displayed to the user.  sum is 0.
--------------------------------------------------------------------------*/
void digiterror (int character, int * sum, const char * message) {

    /* handle error */
    clrbuf (character);

    /* reset sum */
    *sum = 0;

    /* output error message */
    writeline (DIGIT_STRING, stderr);
    fputc (character, stderr);
    writeline (message, stderr);

    writeline (REENTER_NUMBER, stdout);
}


/*--------------------------------------------------------------------------
Function Name:          getaline
Purpose:                This function will read a string from the user.
Description:            This function gets input from the user via
                        calls to fgetc up to some maximum number of
                        characters.  Input is terminated when either the
                        maximum number of characters are entered, or
                        a newline character is detected.  If the user
                        enters more characters than the maximum, clrbuf
                        is called to remove extra characters which are
                        ignored.  Since this is routine accepts input,
                        if EOF is detected EOF is passed back to the main
                        program.
Input:                  message:  the destination array where input is stored.
                        maxlength:  the maximum number of non-NULL characters
                                allowed in the string + 1 for the NULL char.
Result:                 User input is stored in message.
                        EOF is returned when the user enters ^D.
                        Otherwise, the length of the message is returned.
--------------------------------------------------------------------------*/
int getaline (char * message, int maxlength) {
    int messageIndex= 0; /*index for message char array*/
    int newC;/*set equal to input; used to check if EOF*/
    char newChar;/*if input is not EOF, set equal to newC*/


    newC= fgetc(stdin);/*set newC equal to input*/
	
    /*if input is EOF, it returns here*/
    if (newC==EOF){
        return EOF;
    }
    newChar= (char)newC;/*reaching here means input is not EOF; set
                        newChar to the input*/
	
    /*places input in the message array and increments index each time.
    loops until newChar is newline.*/
    while (newChar!='\n'){
        message[messageIndex]=newChar;/*input is put in message array*/
        messageIndex++;
        /*if message buffer is full*/
        if (messageIndex == MAXLENGTH){
            clrbuf(messageIndex-1);/*call clrbuf to clear buffer*/
            messageIndex--;
            message[messageIndex]= '\0';
            return messageIndex;
        }

        newC=fgetc(stdin);/*newC is set to next input*/

        /*if newC is EOF, it goes into this loop until the next input is
        not EOF. Ignores all EOF once stdin is not empty*/
        while (newC==EOF){
            newC= fgetc(stdin);
        }
        newChar= (char)newC;/*reaching here means input is not EOF; set
	                    newChar to the input*/
    }
    message[messageIndex]= '\0';/*adds '\0' to the end of the array*/
    
    if (debug_on){
	fprintf(stderr, "%s %i %s", DEBUG_GETALINE, messageIndex, "]\n");/*debug
                                                                messages*/
    }        
    return messageIndex;
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
    
    /*loops to print a character at a time until the end of message is 
     * reached*/
    while (message[index] && message[index]!= '\0'){
        fputc(message[index], stream);
        index++;
    }
    if (debug_on){
	fprintf(stderr, "%s %i %s", DEBUG_WRITELINE, index, "]\n");/*debug 
                                                               messages*/
    }
    return index;/*returns index as the int*/
}


/*--------------------------------------------------------------------------
Function Name:          main
Description:            This function asks for input and displays output
Purpose:                This program asks the user to enter a string
                        and a number.  Computations are performed on the 
                        strings and numbers, and the results are displayed.
Description:            This function asks for input and displays output
                        in an infinite loop until EOF is detected.  Once EOF
                        is detected, the lengths of the types are displayed.
Input:                  None.
--------------------------------------------------------------------------*/
int main (int argc, char *const* argv) {
    char buffer[MAXLENGTH];      /* to hold string */
    int number;                  /* to hold number entered */
    int strlen;                  /* length of string */

    int array[10];               /* to show user where memory is allocated */
    int * ap = array;		/* to show user about addresses in memory */
    int ** app = &ap;		/* to show user about addresses in memory */
    int * apx = &array[0];	/* to show user about addresses in memory */
    char option;                 /* the command line option */

    /* initialize debug states */
    debug_on = FALSE;

    /* check command line options for debug display */
    while ((option = getopt (argc, argv, "x")) != EOF) {
        switch (option) {
            case 'x': debug_on = TRUE; break;
	}
    }

    /* infinite loop until user enters ^D */
    while (1) {
        writeline ("\nPlease enter a string:  ", stdout);
        strlen = getaline (buffer, MAXLENGTH);
        newline (stdout);

        /* check for end of input */
        if (strlen == EOF)
            break;

        writeline ("The string is:  ", stdout);
        writeline (buffer, stdout);

        writeline ("\nIts length is ", stdout);
        decout (strlen, stdout);
        newline (stdout);

        writeline ("\nPlease enter a decimal number:  ", stdout);
        if ((number = decin ()) == EOF)
            break;

        writeline ("Number entered is:  ", stdout);
        decout (number, stdout);

        writeline ("\nAnd in hexidecimal is:  ", stdout);
        hexout (number, stdout);

        writeline ("\nNumber entered multiplied by 8 is:  ", stdout);
        decout (number << 3, stdout);
        writeline ("\nAnd in hexidecimal is:  ", stdout);
        hexout (number << 3, stdout);

        newline (stdout);
    }

    writeline ("\nThe value of ap is:  ", stdout);
    decout ((int) ap, stdout);
    writeline ("\nAnd in hexidecimal is:  ", stdout);
    hexout ((int) ap, stdout);
    newline (stdout);

    writeline ("The value of app is:  ", stdout);
    decout ((int) app, stdout);
    writeline ("\nAnd in hexidecimal is:  ", stdout);
    hexout ((int) app, stdout);
    newline (stdout);

    writeline ("The value of apx is:  ", stdout);
    decout ((int) apx, stdout);
    writeline ("\nAnd in hexidecimal is:  ", stdout);
    hexout ((int) apx, stdout);
    newline (stdout);

    writeline ("The value of ap + 1 is:  ", stdout);
    decout ((int) (ap+1), stdout);
    writeline ("\nAnd in hexidecimal is:  ", stdout);
    hexout ((int) (ap+1), stdout);
    newline (stdout);

    writeline ("The address of array[0] is:  ", stdout);
    decout ((int) &array[0], stdout);
    newline (stdout);

    writeline ("The address of array[1] is:  ", stdout);
    decout ((int) &array[1], stdout);
    newline (stdout);

    writeline ("The size of a float is:  ", stdout);
    decout (sizeof (float), stdout);
    newline (stdout);

    writeline ("The size of a double is:  ", stdout);
    decout (sizeof (double), stdout);
    newline (stdout);

    writeline ("The size of a long double is:  ", stdout);
    decout (sizeof (long double), stdout);
    newline (stdout);

    writeline ("The size of a char is:  ", stdout);
    decout (sizeof (char), stdout);
    newline (stdout);

    writeline ("The size of an int is:  ", stdout);
    decout (sizeof (int), stdout);
    newline (stdout);

    writeline ("The size of a short is:  ", stdout);
    decout (sizeof (short), stdout);
    newline (stdout);

    writeline ("The size of a short int is:  ", stdout);
    decout (sizeof (short int), stdout);
    newline (stdout);

    writeline ("The size of a long is:  ", stdout);
    decout (sizeof (long), stdout);
    newline (stdout);

    writeline ("The size of a long int is:  ", stdout);
    decout (sizeof (long int), stdout);
    newline (stdout);

    writeline ("The size of a long long is:  ", stdout);
    decout (sizeof (long long), stdout);
    newline (stdout);

    writeline ("The size of a signed is:  ", stdout);
    decout (sizeof (signed), stdout);
    newline (stdout);

    writeline ("The size of a signed char is:  ", stdout);
    decout (sizeof (signed char), stdout);
    newline (stdout);

    writeline ("The size of a signed short is:  ", stdout);
    decout (sizeof (signed short), stdout);
    newline (stdout);

    writeline ("The size of a signed short int is:  ", stdout);
    decout (sizeof (signed short int), stdout);
    newline (stdout);

    writeline ("The size of a signed int is:  ", stdout);
    decout (sizeof (signed int), stdout);
    newline (stdout);

    writeline ("The size of a signed long is:  ", stdout);
    decout (sizeof (signed long), stdout);
    newline (stdout);

    writeline ("The size of a signed long int is:  ", stdout);
    decout (sizeof (signed long int), stdout);
    newline (stdout);

    writeline ("The size of a signed long long is:  ", stdout);
    decout (sizeof (signed long long), stdout);
    newline (stdout);

    writeline ("The size of an unsigned is:  ", stdout);
    decout (sizeof (unsigned), stdout);
    newline (stdout);

    writeline ("The size of an unsigned char is:  ", stdout);
    decout (sizeof (unsigned char), stdout);
    newline (stdout);

    writeline ("The size of an unsigned short is:  ", stdout);
    decout (sizeof (unsigned short), stdout);
    newline (stdout);

    writeline ("The size of an unsigned short int is:  ", stdout);
    decout (sizeof (unsigned short int), stdout);
    newline (stdout);

    writeline ("The size of an unsigned int is:  ", stdout);
    decout (sizeof (unsigned int), stdout);
    newline (stdout);

    writeline ("The size of an unsigned long is:  ", stdout);
    decout (sizeof (unsigned long), stdout);
    newline (stdout);

    writeline ("The size of an unsigned long int is:  ", stdout);
    decout (sizeof (unsigned long int), stdout);
    newline (stdout);

    writeline ("The size of an unsigned long long is:  ", stdout);
    decout (sizeof (unsigned long long), stdout);
    newline (stdout);

    writeline ("The size of a void pointer is:  ", stdout);
    decout (sizeof (void *), stdout);
    newline (stdout);

    writeline ("The size of a character pointer is:  ", stdout);
    decout (sizeof (char *), stdout);
    newline (stdout);

    writeline ("The size of an int pointer is:  ", stdout);
    decout (sizeof (int *), stdout);
    newline (stdout);

    writeline ("The size of a long pointer is:  ", stdout);
    decout (sizeof (long *), stdout);
    newline (stdout);

    writeline ("The size of a float pointer is:  ", stdout);
    decout (sizeof (float *), stdout);
    newline (stdout);

    writeline ("The size of a double pointer is:  ", stdout);
    decout (sizeof (double *), stdout);
    newline (stdout);

    writeline ("The size of a long double pointer is:  ", stdout);
    decout (sizeof (long double *), stdout);
    newline (stdout);

    newline (stdout);

    return 0;
}
