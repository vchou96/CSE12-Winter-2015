/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        2/6/2015 
                                                        cs12xkf 
                                Assignment Six 
 
File Name:      HashTable.java 
Description:    The program creates a table of a certain size in which student
                names and their corresponding numbers can be inserted according
		to the Fairshare algorithm. Abilities of the program include
		looking up if a certain item is in the table, inserting items,
		and listing all the names and numbers in order of index.
		Inserting a name that already exists in the table will update
		the number with the newly inserted one. Items cannot be
		inserted once the table reaches its maximum size, unless it is
		replacing an item with the same name.

****************************************************************************/ 

public class HashTable extends Base {

    private static int counter = 0;	// number of HashTables so far
    private static boolean debug;        // allocation of debug state

    // set in locate, last location checked in hash table 
    private int index = 0; 

    // set in insert/lookup, count of location in probe sequence
    private int count = 0;
        
    private int probeCount[];   // where we are in the probe sequence 
    private long occupancy;     // how many elements are in the Hash Table
    private int size;           // size of Hash Table
    private Base table[];       // the Hash Table itself ==> array of Base
    private int tableCount;     // which hash table it is

    // messages
    private static final String AND = " and ";
    private static final String DEBUG_ALLOCATE = " - Allocated]\n";
    private static final String DEBUG_LOCATE = " - Locate]\n";
    private static final String DEBUG_LOOKUP = " - Lookup]\n";
    private static final String BUMP = "[Bumping To Next Location...]\n";
    private static final String COMPARE = " - Comparing ";
    private static final String FOUND_SPOT = " - Found Empty Spot]\n";
    private static final String HASH = "[Hash Table ";
    private static final String HASH_VAL = "[Hash Value Is ";
    private static final String INSERT = " - Inserting ";
    private static final String PROCESSING = "[Processing ";
    private static final String TRYING = "[Trying Index ";
        
    /**
    * This method turns debug messages on.
    * @param  None
    * @return None
    */
    public static void debugOn () {
        debug= true;//debug messages will show
    }

    /**     
    * This method turns debug messages off.
    * @param  None
    * @return None
    */ 
    public static void debugOff () {
        debug= false;//debug messages will not show
    }

    /**
    * This method allocates and initializes the memory
    * associated with a hash table.
    *
    * @param  sz   The number of elements for the table...MUST BE PRIME!!!
    */
    public HashTable (int sz) {
	size= sz;//size is set as sz
	probeCount= new int[size];//initializes probeCount
	table= new Base[sz];//initializes table
        occupancy=0;//hashtable is empty
	++counter;//increments the number of hash tables allocated
	++tableCount;//increments what tableCount this is
	if (debug){//if debug messages are on
	    System.err.print(HASH + tableCount + DEBUG_ALLOCATE);//prints 
	                          //message for allocating the hash table
        }

    }


    /**
    * This method will insert the element in the hash table.
    * If the element cannot be inserted, false will be returned.
    * If the element can be inserted, the element is inserted
    * and true is returned.  Duplicate insertions will
    * cause the existing element to be deleted, and the duplicate
    * element to take its place.
    *
    * @param   element       The element to insert.
    * @param   initialCount  Where to start in probe seq (recursive calls) 
    * @return  true or false indicating success or failure of insertion
    */
    public boolean insert (Base element, int initialCount) {
	Base bumped;//holds the element bumped out

        count= initialCount;//set count as initialCount

        if(debug){//if debug messages are on
            System.err.println(HASH + tableCount + INSERT + element.getName()
	                       + "]");//prints debug message for inserting
        }

        if (locate(element)){//same item is there, so do duplicate insertion
            table[index]=element;//replace old element with new one 
	    return true;//successfully inserted
        }
        else{//item has not been found
            
	    if (table[index]==null){//CASE if the space is empty
                if (debug){//if debug messages are on
                    System.err.print(HASH + tableCount + FOUND_SPOT);//found an
		                                        //empty spot to insert
                }
                table[index]= element;//insert element at the empty space
		++occupancy;//increment occupancy since element was inserted
                return true;//successfully inserted
	    }
	    
	    if (occupancy==size){//CASE if table is full
                return false;//element was unsuccessfully inserted	    
            }
	    
	    bumped= table[index];//sets bumped as item at index
	    
	    if (probeCount[index]<count){//CASE if true, item needs to be bumped

		if (debug){//if debug messages are on
	            System.err.print(BUMP);//prints the message for bumping
	        }
	        initialCount= probeCount[index]+1;//initialCount is probecount
		                                  //plus one 
		table[index]=element;//puts element in bumped out space
		probeCount[index]= count;//store the value of count at the
		                         //sameindex as the hashtable
                insert(bumped, initialCount);//recursively calls itself
	    }
            else{//base case
		table[index]=element;//puts element at index
		return true;//element was successfully inserted
	    }		            
        }
        return true;//element was unsuccessfully inserted
    }


    /**
    * This method will locate the location in the
    * table for the insert or lookup.
    *
    * @param   element  The element needing a location.
    * @return  true if tem found, or false if not found
    */
    private boolean locate (Base element) {
        int numeric;//the sum of ASCII codes     
	int increment;//the increment found by modding numeric by size-1 and
	              //adding 1

	if (debug){//if debug messages are on
	    System.err.print(HASH + tableCount + DEBUG_LOCATE);//locating index
	    System.err.println(PROCESSING + element.getName() + "]");//process
	                                                            //the name
        }
        
	numeric= element.hashCode();//gets numeric attribute from element
	
	if (debug){//if debug messages are on      
	    System.err.println(HASH_VAL + numeric + "]");//prints hash value
        }
        if (count==1){//if true then element is on its first loop
	    index=numeric%size;//sets initial hashing location as the mod
	                       //of sum of ASCII codes by the hash table size
        }
	
	while(true){//loops until an empty spot is found or there is none
	    if (count>1){//index increments if not on first choice
		increment=(numeric%(size-1))+1;//increment is sum of ASCII
		                //codes modded by size-1 and then added by one
	        index=(index+increment)%size;//subsequent indexes are found by
                  //adding the previous index and increment and modding by size
            }
	    if (debug){
		System.err.println(TRYING + index + "]");//prints message for
		                                   //trying to find empty spots
            }

	    if (table[index]==null){//if null is found AKA empty space
	        probeCount[index]=count;//store the value of count at the same
		                        //index as the hashtable
                return false;//did not find an item with a matching name
	    }

	    if (debug){//if debug messages are on
	        System.err.println(HASH+ tableCount + COMPARE +
		                   element.getName() + AND +
				   table[index].getName() + "]");//comparing
				                      //the item and element
            }

	    if(table[index].equals(element)){//element equals the item at index
                probeCount[index]=count;//store the value of count at the same
		                        //index as the hashtable
                return true;//found an item with matching name
            }
          
            if (table[index]!=null && probeCount[index]<count){//if item is not
		                           //the same as element and the item's
					   //probeCount is less than than
					   //element's count
                return false;//did not find item with matching name
	    }
	    ++count;//increment count for how long the item has been searched
        }
    }


    /**
    * This method will lookup the element in the hash table.  If
    * found a pointer to the element is returned.  If the element
    * is not found, NULL will be returned to the user.
    *
    * @param   element  The element to look up.
    * @return  A pointer to the element if found, else NULL
    */
    public Base lookup (Base element) {
	if(debug){//if debug messages are on
	    System.err.print(HASH + tableCount + DEBUG_LOOKUP);//print message
	                                                       //for lookup
        }
	count=1;//set count back to 1 for locate
        if (locate(element)){//if locate returns true, then the item is
	                     //present
	    return table[index];//returns pointer to element
	}
	else{//if locate returned false
	    return null;//element was not found
	}
    }


    /**
    * Creates a string representation of the hash table. The method 
    * traverses the entire table, adding elements one by one ordered
    * according to their index in the table. 
    *
    * @return  String representation of hash table
    */
    public String toString () {
        String string = "Hash Table " + tableCount + ":\n";
        string += "size is " + size + " elements, "; 
        string += "occupancy is " + occupancy + " elements.\n";

        /* go through all table elements */
        for (int index = 0; index < size; index++) {

            if (table[index] != null) {
                string += "at index " + index + ": ";
                string += "" + table[index];
                string += " with probeCount: "; 
                string += probeCount[index] + "\n";
            }
        }

        return string;
    }
}
