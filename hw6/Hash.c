/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        2/6/2015 
                                                        cs12xkf 
                                Assignment Six 
 
File Name:      Hash.c 
Description:    The program creates a table of a certain size in which student
                names and their corresponding numbers can be inserted according
		to the Fairshare algorithm. Abilities of the program include
		looking up if a certain item is in the table, inserting items,
		and listing all the names and numbers in order of index.
		Inserting a name that already exists in the table will update
		the number with the newly inserted one. If the table reaches
		maximum size and a new name is inserted, the program will list
		all the names and the numbers in order before exiting.
****************************************************************************/ 


#include <cstdlib>
#include <string>
#include "Hash.h"

using namespace std;

static const char AND[] = " and ";
static const char DEBUG_ALLOCATE[] = " - Allocated]\n";
static const char DEBUG_DEALLOCATE[] = " has been deallocated]\n";
static const char DEBUG_LOCATE[] = " - Locate]\n";
static const char DEBUG_LOOKUP[] = " - Lookup]\n";
static const char BUMP[] = "[Bumping To Next Location...]\n";
static const char COMPARE[] = " - Comparing ";
static const char FOUND_SPOT[] = " - Found Empty Spot]\n";
static const char HASH[] = "[Hash Table ";
static const char HASH_VAL[] = "[Hash value is: ";
static const char INSERT[] = " - Inserting ";
static const char PROCESSING[] = "[Processing ";
static const char TRYING[] = "[Trying index ";

bool HashTable :: debug = 0;
int HashTable :: counter = 0;

void HashTable :: Set_Debug (int option)

/***************************************************************************
% Routine Name : HashTable :: Set_Debug (public)
% File :         hash.c
% 
% Description :  This function sets debug mode on or off
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% option             true should set debug mode on, false should set debug
%                    mode off.
***************************************************************************/

{
    if (option){/*if option is not zero*/
	debug= true;/*turns debug messages on*/
    }
}


HashTable :: HashTable (int sz) : size (sz),
	table_count(++counter), occupancy (0), table (new Base *[sz]), 
	probeCount (new int[sz])
/***************************************************************************
% Routine Name : HashTable :: HashTable (public)
% File :         hash.c
% 
% Description :  This function allocates an initializes the memory
%                associated with a hash table.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% size               The number of elements for the table...MUST BE PRIME!!!
***************************************************************************/

{
    /* initialize all table elements */
    for (int i=0; i<sz; i++){/*loops through the array*/
        table[i]= NULL;/*initializes each element*/
    }
    /* initialize all array elements */
    for (int i=0; i<sz; i++){/*loops through the array*/
        (probeCount[i])= NULL;/*initializes each element*/
    }
    if (debug){/*if debug messages are on*/
        cerr<< HASH << table_count << DEBUG_ALLOCATE;/*allocates the table*/
    }
}

HashTable :: ~HashTable (void)
/***************************************************************************
% Routine Name : HashTable :: ~HashTable  (public)
% File :         hash.c
% 
% Description :  deallocates memory associated with the Hash Table.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
    /*deletes each Base * before deleting table*/
    for(int i=0; i<size; i++){/*loops through all items*/
	if (table[i]){/*check that it is not NULL*/
	    delete table[i];/*delete the Base */
        }
    }
    delete[] table;/*frees table*/
    delete[] probeCount;/*frees probeCount array*/
    if (debug){/*if debug messages are on*/
        cerr<< HASH << table_count << DEBUG_DEALLOCATE;/*deallocates the table*/
    }
}

bool HashTable :: Insert (Base * element, int initialCount)
/***************************************************************************
% Routine Name : HashTable :: Insert (public)
% File :         hash.c
% 
% Description : This function will insert the element in the hash table.
%		If the element cannot be inserted, false will be returned.
%		If the element can be inserted, the element is inserted
%		and true is returned.  Duplicate insertions will
%		cause the existing element to be deleted, and the duplicate
%		element to take its place.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert.
% recursiveCall      Whether or not we are entering from recursion
% <return>           true or false indicating success or failure of insertion
***************************************************************************/

{
    Base * bumped;/*holds the element bumped out*/

    count= initialCount;/*set count as initialCount*/

    if(debug){/*if debug messages are on*/
        cerr<< HASH << table_count << INSERT << (char *)(*element)
            << "]" << "\n";/*prints debug message for inserting*/
    }

    if (Locate(element)){/*same item is there, so do duplicate insertion*/
        delete(table[index]);/*delete the old element*/	
	table[index]=element;/*replace old element with new one*/
        return true;/*successfully inserted*/
    }
    else{/*item has not been found*/
            
        if (table[index]==NULL){/*CASE if the space is empty*/
            if (debug){/*if debug messages are on*/
                cerr<< HASH << table_count << FOUND_SPOT;/*found an
		                                        empty spot to insert*/
            }
            table[index]= element;/*insert element at the empty space*/
	    ++occupancy;/*increment occupancy since element was inserted*/
            return true;/*successfully inserted*/
	}
	    
	if (occupancy==size){/*CASE if table is full*/
            return false;/*element was unsuccessfully inserted*/	    
        }
	    
	bumped= table[index];/*sets bumped as item at index*/
	    
	if (probeCount[index]<count){/*CASE if true, item needs to be bumped*/

            if (debug){/*if debug messages are on*/
	        cerr << BUMP;/*prints the message for bumping*/
	    }
	    initialCount= probeCount[index]+1;/*initialCount is probecount
	                                        plus one*/ 
	    table[index]=element;/*puts element in bumped out space*/
	    probeCount[index]= count;/*store the value of count at the
		                           same index as the hashtable*/
            Insert(bumped, initialCount);/*recursively calls itself*/
        }
        else{/*base case*/
	    table[index]=element;/*puts element at index*/
	    return true;/*element was successfully inserted*/
	}         
    }
    return true;/*element was unsuccessfully inserted*/
}


bool HashTable :: Locate (Base * element)
/**********************************************************************
% Routine Name : HashTable :: Locate (private)
% File :         HashTable.c
% 
% Description : This function will locate the location in the
%               table for the insert or lookup.
%
% Parameters descriptions :
%  
% name               description
% ------------------ -------------------------------------------------
% element            The element needing a location.
% <return>           true if tem found, or false if not found
**********************************************************************/

{
    int numeric;/*the sum of ASCII codes*/
    int increment;/*the increment found by modding numeric by size-1 and
                    adding 1*/

    if (debug){/*if debug messages are on*/
        cerr<< HASH << table_count << DEBUG_LOCATE;/*locating index*/
        cerr << PROCESSING << (char *)(*element) << "]" << "\n";/*process
	                                                          the name*/
    }
        
    numeric= (*element);/*gets numeric attribute from element*/
	
    if (debug){/*if debug messages are on*/
        cerr << HASH_VAL << numeric << "]" << "\n";/*prints hash value*/
    }
    if (count==1){/*if true then element is on its first loop*/
        index=numeric%size;/*sets initial hashing location as the mod
	                     of sum of ASCII codes by the hash table size*/
    }
	
    while(true){/*loops until an empty spot is found or there is none*/
        if (count>1){/*index increments if not on first choice*/
	    increment=(numeric%(size-1))+1;/*increment is sum of ASCII
		                             codes modded by size-1 and then
					     added by one*/
	    index=(index+increment)%size;/*subsequent indexes are found by
                adding the previous index and increment and modding by size*/
        }
        if (debug){
            cerr << TRYING << index << "]" << "\n";/*trying to find empty
	                                             spots*/
        }

        if (table[index]==NULL){/*if null is found AKA empty space*/
            probeCount[index]=count;/*store the value of count at the same
		                        index as the hashtable*/
            return false;/*did not find an item with a matching name*/
        }

        if (debug){/*if debug messages are on*/
            cerr<< HASH << table_count << COMPARE <<
	        (char *)(*element) << AND << (char *)(*table[index]) << "]"
		<< "\n";/*comparing the item and element*/
        }

        if(*(table[index])==*(element)){/*element equals the item at index*/
            probeCount[index]=count;/*store the value of count at the same
                                      index as the hashtable*/
            return true;/*found an item with matching name*/
        }

        if (table[index]!=NULL && probeCount[index]<count){/*if item is not
		                           the same as element and the item's
					   probeCount is less than than
		                           element's count*/
            return false;/*did not find item with matching name*/
        }
        ++count;/*increment count for how long the item has been searched*/
    }
}


Base * HashTable :: Lookup (Base * element)
/***************************************************************************
% Routine Name : HashTable :: Lookup (public)
% File :         hash.c
% 
% Description : This function will lookup the element in the hash table.  If
%               found a pointer to the element is returned.  If the element
%               is not found, NULL will be returned to the user.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert or to lookup.
% <return>           A pointer to the element if found, else NULL
***************************************************************************/

{     
    if(debug){/*if debug messages are on*/
        cerr<< HASH << table_count << DEBUG_LOOKUP;/*prints the debug message
	                                             for lookup*/
    }
    count=1;/*set count back to 1*/
    if (Locate(element)){/*if locate returns true, then the item is present*/
        return (Base *)(table[index]);/*returns pointer to element*/
    }
    else{/*if locate returned false*/
        return NULL;/*element was not found*/
    }
}


ostream & HashTable :: Write (ostream & stream)
/***************************************************************************
% Routine Name : HashTable :: Write (public)
% File :         hash.c
% 
% Description : This funtion will output the contents of the hash table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
	stream << "Hash Table " << table_count << ":\n"
	       << "size is " << size << " elements, "
	       << "occupancy is " << occupancy << " elements.\n";

	/* go through all table elements */
	for (int index = 0; index < size; index++)
		if (table[index])
			table[index]->Write(stream << "at index "
			<< index << ": ") << " with probeCount: "
			<< probeCount[index] << "\n";
	return stream;
}	/* end: Write */
