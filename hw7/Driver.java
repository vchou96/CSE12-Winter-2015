import java.io.*;

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

    private String name;//stores the student name
    private long studentnum;//stores the student number

    public UCSDStudent (String nm, long val) {//
        name = new String (nm);//sets name as the nm passed in
        studentnum = val;//sets studentnum to the val passed in
    }
    public String getName () {//gets name and returns
        return name;//returns name
    }
    public boolean equals (Object object) {//checks if 2 objects are equal
    if (this == object)//if equal
        return true;//returns true if equal
    if (!(object instanceof UCSDStudent))//if object is not UCSDStudent
        return false;//returns false
    UCSDStudent otherUCSD = (UCSDStudent) object;//set otherUCSD as object

    return name.equals (otherUCSD.getName ());//returns if names are equal
    }
    public boolean isGreaterThan (Base base) {//checks which object is greater
        return (name.compareTo (base.getName ()) > 0) ? true : false;//returns
	                                                   //if greater or not
    }
    public String toString () {//gets name and studentnum and prints
        return "name:  " + name + "  studentnum:  " + studentnum;
    }
}

public class Driver {
        private static final short NULL = 0;

        public static void main (String [] args) {
        
                /* initialize debug states */
                Tree.debugOff();

                /* check command line options */
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                Tree.debugOn();
                }


                /* The real start of the code */
                SymTab<UCSDStudent> symtab = 
                                new SymTab<UCSDStudent>("UCSDStudentTree");
                String buffer = null;
                char command;
                long number = 0;

                System.out.println ("Initial Symbol Table:\n" + symtab);

                while (true) {
                        command = NULL; // reset command each time in loop
                        System.out.print ("Please enter a command:  " + 
                                "((a)llocate, (i)nsert, (l)ookup, " +
                                "(r)emove, (w)rite):  ");

                        try {
                        command = MyLib.getchar ();
                        MyLib.clrbuf (command); // get rid of return

                        switch (command) {
                        case 'a':
                                System.out.print
                                ("Please enter name of new Tree to " +
                                 "allocate:  ");
                                
                                buffer = MyLib.getline ();// formatted input
                                symtab = new SymTab<UCSDStudent>(buffer);
                                break;

                        case 'i':
                                System.out.print
                                ("Please enter UCSD student name to insert:  ");

                                buffer = MyLib.getline ();// formatted input

                                System.out.print
                                        ("Please enter UCSD student number:  ");

                                number = MyLib.decin ();
                                MyLib.clrbuf (command); // get rid of return

                                // create student and place in symbol table
                                symtab.insert(new UCSDStudent (buffer, number));
                                break;

                        case 'l': { 
                                UCSDStudent found;      // whether found or not

                                System.out.print
                                ("Please enter UCSD student name to lookup:  ");
                                buffer = MyLib.getline ();// formatted input

                                UCSDStudent stu = new UCSDStudent (buffer, 0);
                                found = symtab.lookup (stu);
                                
                                if (found != null) {
                                        System.out.println("Student found!");
                                        System.out.println(found);
                                }
                                else
                                        System.out.println ("student " + buffer
                                                + " not there!");
                                }
                                break;
                        
                        case 'r': { 
                                UCSDStudent removed; // data to be removed

                                System.out.print
                                ("Please enter UCSD student name to remove:  ");

                                buffer = MyLib.getline ();

                                UCSDStudent stu = new UCSDStudent (buffer, 0);

                                removed = symtab.remove(stu);

                                if (removed != null) {
                                        System.out.println("Student removed!"); 
                                        System.out.println(removed);
                                }
                                else
                                        System.out.println ("student " + buffer
                                                + " not there!");
                                }
                                break;

                        case 'w':
                                System.out.println("The Symbol Table " +
                                "contains:\n" + symtab);
                        }
                        }
                        catch (EOFException eof) {
                                break;
                        }
                }
                System.out.println("\nFinal Symbol Table:\n" + symtab);
        }
}
