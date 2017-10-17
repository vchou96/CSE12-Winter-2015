import java.io.EOFException;

/**
* 
* This class holds and updates the names and numbers of the of UCSDStudent
* objects(students). toString returns the name and number of the student,
* getName returns the name, equals checks if a passed in other is the same as
* name, and hashCode returns the ASCII value of name. Main calls methods from
* HashTable.java to put the students in a hashTable.
* 
*/
class UCSDStudent extends Base {
    private String name;//holds student name
    private long Studentnum;//the student number

    public String toString () {//puts the name and number in a string
        return "name: " + name + " Studentnum: " + Studentnum;//returns the
	                                        //student's name and number
    }

    public UCSDStudent (String nm, long val) {//UCSD constructor
        name = new String (nm);//sets name as the nm passed in
        Studentnum= val;//sets Studentnum to the val passed in
    }

    public UCSDStudent (UCSDStudent ucsdstudent) {//copy constructor
        name = new String (ucsdstudent.name);//sets name as ucsdstudent's name
        Studentnum = ucsdstudent.Studentnum;//sets number as ucsdstudent's
	                                    //number
    }

    public String getName () {//gets name of student
        return name;//returns name of student
    }

    public boolean equals (Object other) {//checks if this is the same student
	if (this == other)//returns true if other is equivalent to this
            return true;//other is the same as this UCSDStudent
                
        if (!(other instanceof UCSDStudent))//if other is not an instanceof
		                            //UCSDStudent return false
            return false;//other is not the same as this UCSDStudent
                
        UCSDStudent otherUCSD = (UCSDStudent)other;//sets otherUCSD as other
	                                           //casted as UCSDStudent
                
        return name.equals (otherUCSD.getName ());//returns if the name equals
	                                          //otherUCSD's name or not
    }
         
    public int hashCode () {//calculates the hashCode of name and returns it
        int retval = 0;//holds the numerical value of name
        int index = 0;//used to go through each character of the name's length

        while (index != name.length ()) {//loops through the entire name's
		                         //length
            retval += name.charAt (index);//adds each character to retval each
	                                  //time
            index ++;//increment the index
        }

        return retval;//returns the hashCode
    }	
}

public class Driver {

        public static void main (String [] args) {

                /* initialize debug states */
                HashTable.debugOff();

                /* check command line options */
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                HashTable.debugOn();
                }

                /* The real start of the code */
                SymTab symtab = new SymTab (5);
                String buffer = null;
                char command;
                long number = 0;

                System.out.print ("Initial Symbol Table:\n" + symtab);

                while (true) {
                        command = 0;    // reset command each time in loop
                        System.out.print ("Please enter a command:  "
                                + "((i)nsert, (l)ookup, (w)rite):  ");

                        try {
                        command = MyLib.getchar ();
                        MyLib.clrbuf (command); // get rid of return

                        switch (command) {
                        case 'i':
                                System.out.print (
                                "Please enter UCSD Student name to insert:  ");
                                buffer = MyLib.getline ();// formatted input

                                System.out.print (
                                        "Please enter UCSD Student number:  ");

                                number = MyLib.decin ();
                                MyLib.clrbuf (command); // get rid of return

                                // create Student and place in symbol table
                                if(!symtab.insert (
                                        new UCSDStudent (buffer, number), 1)){

                                        System.out.println("Couldn't insert " + 
                                                           "student!!!"); 
                                }
                                break;

                        case 'l': {
                                Base found;     // whether found or not

                                System.out.print (
                                "Please enter UCSD Student name to lookup:  ");

                                buffer = MyLib.getline ();// formatted input

                                UCSDStudent stu = new UCSDStudent (buffer, 0);
                                found = symtab.lookup (stu);
                                
                                if (found != null) {
                                        System.out.println ("Student found!!!");
                                        System.out.println (found);
                                }
                                else
                                        System.out.println ("Student " + buffer
                                                + " not there!");
                                }
                                break;

                        case 'w':
                                System.out.print (
                                    "The Symbol Table contains:\n" + symtab);
                        }
                        }
                        catch (EOFException eof) {
                                break;
                        }
                }

                System.out.print ("\nFinal Symbol Table:\n" + symtab);
        }
}
