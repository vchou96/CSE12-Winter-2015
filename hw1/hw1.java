/**
 * The hw1 class is a direct port of hw1.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.
 */


/*                                                      Vanessa Chou
                                                        CSE12, Winter 2015
                                                        1/9/2015
                                                        cs12xkf
                                Assignment One

File Name:      hw1.java
Description:    This program tests functions to display output strings and 
                numbers. 
*/


import java.io.*;      // System.out

public class hw1 {
    private static final int COUNT = 8;	//number of hex digits to display
    private static final int DECIMAL = 10;	// to indicate base 10
    private static final int HEX = 16;		// to indicate base 16

    private static final char digits[] = 	// used for ASCII conversion
        new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();
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
        char[] chars = new char[COUNT];//character array to hold the
	                               //converted characters
        int index=0;//index for chars[]

        newInt= number;//newInt takes value of number

        //takes the remainder of number and base and finds the corresponding
	//char in digits[]. Corresponding char is the first input for
	//chars[].
	chars[index]=digits[newInt%base];
        index++;

        //loop that divides the number by base, finds the remainder and places
	//it in the chars[].  
        while ((newInt/base!=0)){
            newInt= newInt/base;
            chars[index]=digits[newInt%base];
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
    * Function Name: fputc
    * Purpose: Displays a single character.
    * @param    CharToDisp is the character to display.
    * @param    stream is where to display, likely System.out or System.err.
    * Result: The character is displayed. No return value.
    */
    public static void fputc(char CharToDisp, PrintStream stream) {

        //Print a single character.
        stream.print (CharToDisp);   

        //Flush the system.out buffer, now. 
        stream.flush ();
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
	//reached
        while (index < message.length()){
            fputc(message.charAt(index), stream);
            index++;
        }
        return index;//returns index as the int
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
    * Function Name: newline
    * Purpose: Prints out a newline character.
    * @param    stream is where to display, likely System.out or System.err.
    * Result: A newline is displayed. No return value.
    */
    public static void newline ( PrintStream stream ) {
        //adds a newline to stream
	fputc('\n', stream);
    }


    public static void main( String[] args ) {
        System.err.print("Zdravei Sviat\n");//
        writeline ("Hello World", System.out);//calls writeline and passes 
	                                      //"Hello World" as message
        newline(System.out);//calls newline
        decout (123, System.out);//calls decout and passes in 123 as number
        newline(System.out);//calls newline
        decout (0, System.out);//calls decout and passes 0 as number
        newline(System.out);//calls newline
        hexout (0xFEEDDAD, System.out);//calls hexout and passes in 0xFEEDDAD
	                               //as number
        newline(System.out);//calls newline
    }
}
