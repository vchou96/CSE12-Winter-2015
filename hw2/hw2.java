/**
 * The hw2 class is a direct port of hw2.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.  The getaline function returns a String type.
 */

import java.io.*;        // System.in and System.out
import java.util.*;      // Stack

class MyLibCharacter {
    private Character character;

    public MyLibCharacter (int ch) {
        character = new Character ( (char) ch );
    }

    public char charValue () {
        return character.charValue ();
    }

    public String toString () {
        return "" + character;
    }
}

public class hw2 {
    private static final int ASCII_ZERO = 48;
    private static final int BUFSIZ= 12;   //Size of the char array
    private static final int COUNT = 8;		// # of hex digits

    private static final int CR = 13;		// Carriage Return
    private static final int MAXLENGTH = 80;	// Max string length

    private static final int EOF = -1;		// process End Of File

    private static final int DECIMAL = 10;		// to indicate base 10
    private static final int HEX = 16;		// to indicate base 16

    private static final char digits[] = 	// for ASCII conversion
        new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();

    private static final String DEBUG_GETALINE = 
        "[*DEBUG:  The length of the string just entered is ";

    private static final String DIGIT_STRING = "digit ";
    private static final String REENTER_NUMBER ="\nPlease reenter number: ";
    private static final String OUT_OF_RANGE = " out of range!!!\n";
    private static final String CAUSED_OVERFLOW = " caused overflow!!!\n";
    private static final String DEBUG_WRITELINE =
        "\n[*DEBUG:  The length of the string displayed is ";

    private static Stack<MyLibCharacter> InStream =
        new Stack<MyLibCharacter>();

    private static boolean debug_on = false;
    private static int hexCounter = 0; // counter for the number hex digits

    
    /**
    * Function Name: baseout
    * Purpose: Takes in a positive number and displays in a given base.
    * @param    number is the number to be displayed.
    * @param    base to used to display number.
    * @param    stream is where to display, usually System.out or System.err.
    * Result: Number in the new base is displayed. No return value.
    */
    private static void baseout (int number, int base, PrintStream stream) {
        int newInt;//new integer each time number is divided by base
        String str = "0";//string for substituting 0's in the hexidecimal case
        char[] chars = new char[BUFSIZ];//character array to hold the
	                               //converted characters
        int index=0;//index for chars[]

        newInt= number;//newInt takes value of number

        //loop that divides the number by base, finds the remainder and the
	//corresponding char in digits[]. It is then placed in chars[].
	while ( newInt != 0 ){
            chars[index] = digits[newInt % base];
	    newInt = newInt / base;
	    index++;
	   
	}
        //if base is hexidecimal, 0's will be substituted to ensure 8 digits.
        if (base==HEX){
            //loops until 8 values have been reached in the chars[]
            while(index<COUNT){
                chars[index]= str.charAt(0);
                index++;
            }
        }

	//prints the character array backward so it is now in the right
	//order
	while(index!=0){
            fputc(chars[index-1], stream);
	    index--;
        }
    }


    /**
    * Function Name: clrbuf
    * Purpose: This function clears System.in when called.
    * Description: This function calls a while loop that clears characters one
    * at a time until a newline is reached
    * @param    character is the int.
    * Result: A newline is displayed. No return value.
    */
    public static void clrbuf (int character) {
	while (character!='\n'){
	    character=fgetc(System.in);//clears a character at a time
	}
    }



    /**
    * Function Name: decin
    * Purpose: Converts decimal number from System.in and converts from ASCII to
    * decimal.
    * Description: The function loops until a newline character is reached. 
    * If the inputted character is within the range, the function will update
    * the values of sum and check if the number is overflow by doing reverse
    * operations. If it is overflowed, digiterror is called and will ask for
    * reinput. If it is not in range, then it calls digiterror for the out of
    * range case and asks for input again.
    * Parameters: None
    * Result: Return value is int.
    */
    public static int decin() {
	char nextInt;
	int oldInt=0;
	int sum=0;
	int oldsum=0;
	 
        do{
	    nextInt= (char)fgetc(System.in);/*sets nextInput to the first
	                                      input for decimal*/
            //if EOF is called while stdin is empty the function returns as
	    //EOF
            if (nextInt==(char)EOF){
                return EOF;
            }
            //if nextInt is not newline this will loop
	    if (nextInt!= '\n'){
		//if nextInt is in the range of ASCII characters for decimal
	        //digits. AKA if nextInt is greater or equal to ASCII character
		//for 0 and less than the one for 9
                if (nextInt>=ASCII_ZERO && nextInt < ASCII_ZERO+DECIMAL){
                    //if this is the first input and sum is still 0
		    if (sum==0){
			sum= nextInt-'0';//nextInt is subtracted by '0' to
			                 //convert from ASCII to decimal and
					 //set as sum.
		        oldsum= sum;//old sum set as sum
	            }
		    //if the number has multiple digits and is not on its first
		    //input
		    else{
		        sum=(sum*DECIMAL)+(nextInt - '0');         //The sum is
			                             //multiplied by 10 and has 
						     //input added to it.
						     
                        //if reverse operations on sum is not the same as
			//oldsum then there was overflow. Digiterror is
			//called to ask for a new input.
		        if (sum/DECIMAL != oldsum){
                            digiterror(nextInt, CAUSED_OVERFLOW);//calls
			                              //digiterror for overflow
						      //case; asks again for
						      //input
		            sum=0;//sum resets to 0
			    oldsum=0;//oldsum resets to 0
			}
			else{
                            oldsum= sum;//oldsum is updated as sum
		        }
		    }
                }
		//if nextInt is not in the range of ASCII characters
                else if (nextInt<ASCII_ZERO || nextInt >= ASCII_ZERO+DECIMAL){
                    digiterror(nextInt, OUT_OF_RANGE);//call digiterror for out
		                          //of range case; asks again for input
		    sum=0;//sum reset to 0
		    oldsum=0;//oldsum reset to 0
	        }
	    }
        } while (nextInt!= '\n');//loops for every digit inputted until newline
        return sum;//returns the total sum
    }


    /**
    * Function Name: decout
    * Purpose: Takes in a positive number and displays it in decimal.
    * @param    number is the positive numeric value to be displayed.
    * @param    stream is where to display, likely System.out or System.err.
    * Result: The number in decimal is displayed. No return value.
    */
    public static void decout (int number, PrintStream stream) {
        //calls on baseout and passes in the number, 10 as base, and stream
        baseout(number, DECIMAL, stream);
    }


    /**
    * Function Name: digiterror
    * Purpose:       This function handles erroneous user input. 
    * Description:   This function  displays and error message to the user,
    *                and asks for fresh input.
    *Input:          character:  The character that began the problem.
    *                message:  The message to display to the user.
    *Result:         The message is displayed to the user.
    *                The result in progress needs to be set to 0 in
    *                decin after the call to digiterror.*/
    public static void digiterror (int character, String message) {

        /* handle error */
        clrbuf (character);

        /* output error message */
        writeline (DIGIT_STRING, System.err);
        fputc ( (char)character, System.err);
        writeline (message, System.err);

        writeline (REENTER_NUMBER, System.err);
    }


    /**
    * Function Name: getaline
    * Purpose: The function stores characters from System.in into the message
    *          array. If the message becomes bigger then maxlength clrbuf is
    *          called.
    * Description: The function loops until a newline is found. It stores each
    * inputted character into the message array. If it reaches the max limit,
    * then clrbuf is called to clear the buffer.
    * @param    message[] is the char array that holds the inputted characters
    * @param    maxlength is the maximum size of the array
    * Result: Int return value.
    */
    public static int getaline( char message[], int maxlength ) {
	int messageIndex= 0; //index for message char array
	int newC;//set equal to input; used to check if EOF
	char newChar;//if input is not EOF, set equal to newC


        newC= fgetc(System.in);//set newC equal to input
	//if input is EOF, it returns here
	if (newC==EOF){
	    return EOF;
	}
	newChar= (char)newC;//reaching here means input is not EOF; set
	                    //newChar to the input
	
	//places input in the message array and increments index each time.
	//loops until newChar is newline.
	while (newChar!='\n'){

	    message[messageIndex]=newChar;//input is put in message array
	    messageIndex++;

	    //if message buffer is full
            if (messageIndex == MAXLENGTH){
                clrbuf(messageIndex-1);//call clrbuf to clear buffer
		messageIndex--;
	        message[messageIndex]= '\0';	
		break;
	    }

	    newC=fgetc(System.in);//newC is set to next input

	    //if newC is EOF, it goes into this loop until the next input is
	    //not EOF. Ignores all EOF once System.in is not empty
	    while (newC==EOF){
	        newC= fgetc(System.in);
	    }    
	    newChar= (char)newC;//reaching here means input is not EOF; set
	                        //newChar to the input
    	}
	message[messageIndex]= '\0';//adds '\0' to the end of the array

        if (debug_on){
	    System.err.println(DEBUG_GETALINE + messageIndex + "]");
        }
	return messageIndex;
    }


    /**
    * Function Name: hexout
    * Purpose: Takes in a positive number and displays it in hex.
    * @param    number is a positive numeric value to be displayed in hex.
    * @param    stream is where to display, likely System.out or System.err.
    * Result: Number in base 16 is displayed. No return value.
    */
    public static void hexout (int number, PrintStream stream) {
        // Output "0x" for hexidecimal.
        writeline ("0x", stream);
	//calls on baseout and passes in the number, 16 for base, and stream
        baseout (number, HEX, stream);
    }



    /**
    * Function Name: fgetc
    * Purpose: Returns a character from the input stream.
    * @param: stream is where to display
    * @return  <code>char</code>
    */
    public static int fgetc(InputStream stream) {

        char ToRet = '\0';

        // Check our local input stream first.
        //   If it's empty read from System.in
        if (InStream.isEmpty ()) {

            try {
                // Java likes giving the user the
                // CR character too. Dumb, so just
                // ignore it and read the next character
                // which should be the '\n'.
                ToRet = (char) stream.read ();
                if (ToRet == CR)
                    ToRet = (char) stream.read ();
                                
                // check for EOF
                if ((int) ToRet == 0xFFFF)
                    return EOF;
            }

            // Catch any errors in IO.
            catch (EOFException eof) {

                // Throw EOF back to caller to handle
                return EOF;
            }

            catch (IOException ioe) {

                writeline ("Unexpected IO Exception caught!\n",
                                        System.out);
                writeline (ioe.toString (), System.out);
            }

        }

        // Else just pop it from the InStream.
        else
            ToRet = ((MyLibCharacter) InStream.pop ()).charValue ();
        return ToRet;
    }


    /**
    * Function Name: fputc
    * Purpose: Displays a single character.
    * @param    Character to display.
    * @param    stream is where to display
    * Result: A single character is displayed. No return value.
    */
    public static void fputc(char CharToDisp, PrintStream stream) {

        // Print a single character.
        stream.print (CharToDisp);

        // Flush the system.out buffer, now.
        stream.flush ();
    }


    /**
    * Function Name: newline
    * Purpose: Prints out a newline character.
    * @param    stream is where to display, likely System.out or System.err.
    * Result: A newline is displayed. No return value.
    */
    public static void newline ( PrintStream stream ) {
        //adds a newline to stream
	fputc('\n', stream);
    }


    /**
    * Function Name: writeline
    * Purpose: Prints out a string.
    * @param    message is a string to print out.
    * @param    stream is where to display, likely System.out or System.err.
    * @return   The length of the string is returned as int.
    */
    public static int writeline (String message, PrintStream stream) {
        int index=0; //corresponds to a char in message; serves as message's
	             //"index"

	//loops to print out a character at a time until the end of message is
	//reached or a null char is reached
        while (index < message.length() && message.charAt(index)!= '\0'){
            fputc(message.charAt(index), stream);
            index++;
        }
	if (debug_on){
	    System.err.println(DEBUG_WRITELINE+index+ "]");
        }
	return index;//returns index as the int
    }


    /**
    * Function Name: ungetc
    * Purpose: Places back a character into the input stream buffer.
    * @param    A character to putback into the input buffer stream.
    * Result: A character is placed back into stream buffer. No return value.
    */
    public static void ungetc (int ToPutBack) {

        // Push the char back on our local input stream buffer.
        InStream.push (new MyLibCharacter (ToPutBack));
    }


    public static void main( String[] args ) {

        char buffer[] = new char[MAXLENGTH];       // to hold string 

        int number;                  // to hold number entered 
        int strlen;                  // length of string 

        //initialize debug states
        debug_on = false;

        //check command line options for debug display 
        for (int index = 0; index < args.length; ++index) {
            if (args[index].equals("-x"))
                debug_on = true;
        } 

        // infinite loop until user enters ^D 
        while (true) {
            writeline ("\nPlease enter a string:  ", System.out);

            strlen = getaline (buffer, MAXLENGTH);
            newline (System.out);

            // check for end of input 
            if ( EOF == strlen )
                break;

            writeline ("The string is:  ", System.out);
            writeline ( new String(buffer), System.out);

            writeline ("\nIts length is ", System.out);
            decout (strlen, System.out);
            newline (System.out);

            writeline ("\nPlease enter a decimal number:  ", System.out);
            if ((number = decin ()) == EOF)
                break;

            writeline ("Number entered is:  ", System.out);
	    decout (number, System.out);

            writeline ("\nAnd in hexidecimal is:  ", System.out);
            hexout (number, System.out);

            writeline ("\nNumber entered multiplied by 8 is:  ", System.out);
            decout (number << 3, System.out);
            writeline ("\nAnd in hexidecimal is:  ", System.out);
            hexout (number << 3, System.out);

            newline (System.out);
        }
    }
}
