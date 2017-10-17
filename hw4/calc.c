/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        1/23/2015 
                                                        cs12xkf 
                                Assignment Four 
 
File Name:      calc.c 
Description:    This program will act as a calculator and calculate inputted
                digits with inputted operators. 2 stacks are used to hold the
		digits and operators and the 2nd stack, a local variable, is
		deleted after each function ends to free memory. Intopost puts
		the input into stack1 in postfix notation and eval will
		evaluate the math expression from postfix notation. 
		Setupword converts the ASCII values of operators and fact and
		exponent acts as the factorial and exponential components of
		the calculator. 
****************************************************************************/  
#include <ctype.h>
#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"

#define CHARACTER(word) ((char) ((word) & 0x000000FF))
#define INDEX(word) (((word) & 0x0000FF00) >> 8)
#define PRIORITY(word) ((word) & 0x0000FE00)
#define BYTE 8
#define SIGN_BIT 0x80000000

static char operators[] = "()+-*/^ !";/*operators array*/


static long add (long, long);
static long divide (long, long);
static long exponent (long, long);
static long fact (long, long);
static long mult (long, long);
static long setupword (int);
static long sub (long, long);

static long (*functions[]) (long, long) =
    { NULL, NULL, add, sub, mult, divide, exponent, NULL, fact };


/*--------------------------------------------------------------------------
Function Name:         eval
Purpose:               Evaluates the math expression from postfix notation
Description:           The function first transfers everything from stack1 in
                       reverse to stack2. Digits are popped from stack2 and
		       returned to stack1 until a non-digit is found. If it is
		       a factorial, then it will only pass one number in to the
		       functions array for its factorial to be found. 
		       Otherwise, it will send two numbers into functions for
		       them to be evaluated by the operator. At the end, the
		       final math result is returned.
Input:                 stack1 is stack pointer pointing to stacks with postfix
                       expressions
Result:                The result of the math expression is returned.
                       Return value is long.
--------------------------------------------------------------------------*/
long eval (Stack * stack1) {
    long op1=0;/*acts as first operand*/
    long op2 =0;/*acts as second operand*/
    long character;/*character holds the operator*/
    Stack * stack2; /*local stack*/
    long result = 0;/*holds the result of the math expression*/
    
    
    stack2= new_Stack(CALCSTACKSIZE);/*call new_stack to make a stack of size
    CALCSTACKSIZE*/

    /*while stack1 is not empty, reverse everything from stack1 onto stack2*/
    while(!isempty_Stack(stack1)){
	pop(stack1, &character);/*pop the top character from stack1 and set
	                          character equal to it*/
	push(stack2, character);/*push character onto stack2*/
    }

    /*while stack2 is not empty*/
    while(!isempty_Stack(stack2)){
	pop(stack2, &character);/*pop the top character from stack2 and set
	                          character equal to it*/
	if (character>0){/*check if the character is bigger than 0, and
		           therefore not an operator*/
	    push(stack1, character);/*push character onto stack1*/
	}
	else{/*if the character is non-numbers*/
	    if (character==setupword('!')){/*if the character is the factorial
		                             symbol*/
	        pop(stack1, &op1);/*only pop one numebr from the stack since
		                    factorial only affects one number*/
 		result= functions[INDEX(character)](op1, 0);/*2nd number is
		                                              ignored*/
                push(stack1, result);/*push character onto stack1*/	    
	    }
	    else{/*if the operator is not factorial*/
	    
	        pop(stack1, &op1);/*pop the top from stack1. This will be one
		                  of the operands to be evaluated with the
				  operator from stack2.*/
	        pop(stack1, &op2);/*pop the top from stack1. This will be
		                  another operand to be evaluated with the
				  operator from stack2.*/	    
                result= functions[INDEX(character)](op1, op2);/*character is 
		                  the operator. Result is the result after op1
				  and op2 are evaluated using character.*/
                push(stack1, result);/*push character onto stack1*/	    
	    }
	}
    }
    if (!isempty_Stack(stack1)){/*Check if stack1 is empty or not*/ 
        pop(stack1, &result);/*the final item on stack1 is the result of the
                         expression. Set result variable to this*/
    }
    delete_Stack(&stack2);/*deletes the stack2 local variable to free memory*/
    return result;/*result is used as the return value*/
}


/*--------------------------------------------------------------------------
Function Name:         intopost
Purpose:               Converts infix math expressions to postfix form
Description:           The function will place inputs into either stack1 or
                       stack2 and will eventually pop everything in stack2 and
		       push it to stack1. loops until newline is entered. With
		       each loop, the input is checked. If it is EOF, it will
		       return as EOF. If it is blank, the input is ignored. If
		       the input is a digit, then it is returned to buffer and
		       the entire number is calculated by calling decin and
		       then is pushed onto stack1. In the case of open
		       parantheses, stack2 will hold it until a close
		       parantheses is read. Stack2 is then popped until the
		       open parantheses is found, will all the popped items
		       being pushed to stack1. At the end, stack2 will be
		       completely popped and deleted, and stack1 will hold the
		       post-fix expression in reverse.   
Input:                 stack1 is the stack pointer pointing to empty stacks to
                       hold postfix expressions
Result:                Either EOF is returned to signify that EOF was entered,
                       or 1 is returned to signify success.
                       Return value is long.
--------------------------------------------------------------------------*/
long intopost (Stack * stack1) {
    Stack * stack2;/*local stack*/
    long character;/*set this to fgetc each time*/
    long numberInput;/*the entire number that decin returns*/
    long operand;/*other variable to compare priority of operand with
                   character*/

    
    stack2= new_Stack(CALCSTACKSIZE);/*call new_stack to make a stack of size
    CALCSTACKSIZE*/

    /*while there is still input the loop will run*/
    while((character=fgetc(stdin))!='\n'){
        /*if the character is EOF (AKA if the ctrl D is pressed) then the
	 *program exists*/
        if (character==EOF){
            delete_Stack(&stack2);/*deletes stack2 to prevent memory leak*/
	    return EOF;/*returns EOF*/
        }

        /* if the character is blank then we ignore it*/
        else if (character== ' '){
        }

        /* else if the character is a digit then convert all the digits until
        * the next non-digit into decimal. The decimal number is put on
	* stack1*/
        else if (isdigit(character)){
            ungetc(character, stdin);/*ungets the number and puts it back in
            buffer*/
            numberInput= decin();/*reads the entire number*/
            push(stack1, numberInput);/*pushes the entire number on stack1*/
        }

        /* if character is not blank or EOF or digit, then it goes into here*/
        else {
	    /*if the character is open parantheses then convert it to
            * hexadecimal and then push it onto stack2*/	
            if (character == '('){
                character= setupword(character);/*sets character to hexadecimal
                value of open parantheses*/	
                push(stack2, character);/*pushes new value of character to
		stack2*/
            }
        
            /* else if the character is close parantheses ')' then keep popping
            * stack 2, pushing all items from stack 2 onto stack 1 until the
	    * open parantheses is reached. Both parantheses symbols are then 
            * discarded.*/
            else if(character == ')'){
                top(stack2, &character);/*get the top of stack2 and set it to
		                          character*/

		/*loops if stack2 is not empty and the character is not
                * parantheses*/    
	        while(CHARACTER(character)!='(' && !isempty_Stack(stack2)){
	            pop(stack2, &character);/*pops top item off stack 2 and 
		                             sets character to the popped
					     item*/
   		    push(stack1, character);/*pushes the item popped from
		                              stack2 to stack1*/
		    /*Checks if stack is empty. If not, then it will get the
		    * top item from stack2*/
		    if (!isempty_Stack(stack2)){
		        top(stack2, &character);/*updates character as the
			current top item on stack2*/
                    }   	
	    	}
	        pop(stack2, &character);/*parantheses are discarded*/
            }

	    /* Else keep popping from stack 2 and pushing to stack1 until
	     * stack2 is empty or stack2's top symbol has lower priority than
	     * the character entered.  The character is then pushed onto
	     * stack2.*/
	    else{
		/*Checks if the stack is empty. If not, operand is set as the
                *top item*/
	        if (!isempty_Stack(stack2)){
	            top(stack2, &operand);/*operand set as top item on stack2*/
		}
	        character= setupword(character);/*sets character to hexadecimal
	        value*/	

		/*loops when stack2 is not empty and the priority of operand
		* is higher than character*/ 
                while(!isempty_Stack(stack2) && (PRIORITY(operand)>=
		       PRIORITY(character))){
	            pop(stack2, &operand);/*pops top of stack 2 and sets
		                            operand as the top item*/
		    push(stack1, operand);/*pushes the item popped from stack2
		    to stack1*/

		    /*Checks if stack2 is empty. If not, then set operand as
		    * the top item*/
		    if (!isempty_Stack(stack2)){
	                top(stack2, &operand);/*set operand as the top item
			                        from stack2*/
	            }
	        }
	        push(stack2, character);/*character is pushed onto stack 2*/
              	
	    }
        }
    }

    /* Everything else on stack 2 is popped and pushed to stack1. Stack1
     * contains post-fix expression in reverse order*/
    while (!isempty_Stack(stack2)){/*Checks that stack2 is not empty*/
	pop(stack2, &character);/*pop the top item from stack2 and set
	                          character equal to it*/
        push(stack1, character);/*push character onto stack1*/
    }
    delete_Stack(&stack2);/*deletes stack2 to prevent memory leak*/
    return 1;/*returns nonzero for success*/


}

static long add (long augend, long addend) {
    return augend + addend;
}

static long divide (long divisor, long dividend) {
    return dividend / divisor;

}

/*--------------------------------------------------------------------------
Function Name:         exponent
Purpose:               Calculates the base to the power exponent
Description:           To calculate exponents, a while loop keeps track of
                       power and stops looping when power is 0. Baseresult is
		       first 1 but is multiplied by the base with each loop
		       and is set to the new number each time. An exponent
		       represents base being multiplied by itself power times.
		       Power decrements with each time baseresult is
		       multiplied by base. When power is 0, the final value is
		       returned.
Input:                 power is the power of the base
                       base is the base number in the exponential calculation 
Result:                Base to the power expoenent is returned through
                       baseresult.
                       Return value is static long.
--------------------------------------------------------------------------*/
static long exponent (long power, long base) {
    long baseresult =1;/*baseresult will hold the result of base to
                                the power exponent*/
    /*loops until power is 0*/
    while(power!=0){
        baseresult= baseresult*base;/*baseresult is multiplied by base and
	                              updated with each loop*/
	--power;/*power is decremented*/
    }
    return baseresult;/*base to the power exponent is returned*/

}


/*--------------------------------------------------------------------------
Function Name:         fact
Purpose:               Calculates the xxx factorial
Description:           To calculate factorial, xxx will be multiplied by itself
                       and be decremented with each loop until xxx is 0.
		       Factorialresult holds the overall value of xxx with each
		       time it is multiplied. When the loop ends, xxx! is
		       returned through factorialresult.
Input:                 xxx is the number we will find the factorial of
                       ignored is op2 in eval and will not be used
Result:                The resulting factorial of xxx is returned through
                       factorialresult
                       Return value is static long.
--------------------------------------------------------------------------*/
static long fact (long xxx, long ignored) {
    long factorialresult = 1;/*this number will hold the result of the
                                      xxx factorial*/
    
    /*loops until xxx is 0*/
    while (xxx!=0){
	factorialresult= factorialresult*xxx;/*each time factorialresult is
	                                       multiplied by xxx and updated*/
	--xxx;/*xxx decrements*/
    }
    return factorialresult;/*the final factorial result is returned*/
}


static long mult (long factorx, long factory) {
    return factorx * factory;
}

/*--------------------------------------------------------------------------
Function Name:         setupword
Purpose:               Converts character from ASCII to hexadecimal
Description:           The loop checks which operator character matches and
                       will stop once the maximum index is passed. It then
		       returns the hexdecimal value of the operator 
Input:                 character is the ASCII code for operators
Result:                Returns the hexadecimal of the operators
                       Return value is static long.
--------------------------------------------------------------------------*/
static long setupword (int character) {
    int index=0;/*index for the operators array*/
    
    /*loops until index is out of the array*/
    while(operators[index]){
        if(character==operators[index]){/*if character is equal to the operator
		                          at the array index array*/ 
            break;/*exits the while loop*/
	 }
	 index++;/*increment index*/     
    }

    return SIGN_BIT | index<<BYTE|character; /*returns hexadecimal of the
                                               operators*/


}


static long sub (long subtrahend, long minuend) {
    return minuend - subtrahend;    
}

