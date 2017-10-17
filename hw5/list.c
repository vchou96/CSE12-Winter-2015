/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        1/28/2015 
                                                        cs12xkf 
                                Assignment Five 
 
File Name:      list.c 
Description:    

****************************************************************************/ 
#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "list.h"

typedef struct Node {
        struct Node * pre;      /* how to access the prior Node */
        struct Node * next;     /* how to access the next Node */
        void * data;            /* the data to store */
} Node;

typedef struct List {
        Node * front;             /* the front of the list */
        long list_count;        /* which list is it */
        long occupancy;
        void *(*copy_func) (void *);
        void (*delete_func) (void *);
        long (*is_greater_than_func) (void *, void *);
        FILE *(*write_func) (void *, FILE *);
} List;

/* private Node function declarations */
static void delete_Node (Node **, void (*delete_func) (void *));
static Node * find_location (List * this_list, void * element);
static Node * insert_Node (Node *, void *, void *(*copy_func) (void *)); 
static Node * new_Node (void *, void *(*copy_func) (void *));
static void * remove_Node (Node *);
static void * view_Node (Node *); 
static FILE * write_Node (Node *, FILE *, 
                          FILE *(*write_func) (void *, FILE *));

/* catastrophic error messages */
static const char ADNEXT_NONEXIST[] = 
                "Advance next from non-existent list!!!\n";
static const char ADNEXT_EMPTY[] = 
                "Advance next from empty list!!!\n";
static const char ADPRE_NONEXIST[] = 
                "Advance pre from non-existent list!!!\n";
static const char ADPRE_EMPTY[] = 
                "Advance pre from empty list!!!\n";
static const char DELETE_NONEXIST[] =
                "Deleting from non-existent list!!!\n";
static const char DELETE_NONEXISTNODE[] =
                "Deleting a non-existent node!!!\n";
static const char ISEMPTY_NONEXIST[] =
                "Is empty check from non-existent list!!!\n";
static const char INSERT_NONEXIST[] =
                "Inserting to a non-existent list!!!\n";
static const char REMOVE_NONEXIST[] =
                "Removing from non-existent list!!!\n";
static const char REMOVE_EMPTY[] =
                "Remove from empty list!!!\n";
static const char VIEW_NONEXIST[] = 
                "Viewing a non-existent list!!!\n";
static const char VIEW_NONEXISTNODE[] = 
                "Viewing a non-existent node!!!\n";
static const char VIEW_EMPTY[] =
                "Viewing an empty list!!!\n";
static const char WRITE_NONEXISTFILE[] =
                "Writing to a non-existent file!!!\n";
static const char WRITE_NONEXISTLIST[] =
                "Writing from a non-existent list!!!\n";
static const char WRITE_MISSINGFUNC[] =
                "Don't know how to write out elements!!!\n";
static const char WRITE_NONEXISTNODE[] =
                "Writing from a non-existent node!!!\n";

/* debug messages */
static const char ADNEXT[] = "[List %ld - Advancing next]\n";
static const char ADPRE[] = "[List %ld - Advancing pre]\n";
static const char INSERT[] = "[List %ld - Inserting node]\n";
static const char REMOVE[] = "[List %ld - Removing node]\n";
static const char VIEW[] = "[List %ld - Viewing node]\n";
static const char LIST_ALLOCATE[] = "[List %ld has been allocated]\n";
static const char LIST_DEALLOCATE[] = "[List %ld has been deallocated]\n";

static int debug_on = FALSE;    /* allocation of debug flag */
static long list_counter = 0;   /* the number of lists allocated so far */
long originalfront=0;

/*
1. front: 1
2. Node 1 data: 1
3. Node 1 pre: 3
4. Node 1 next: 2
5. Node 2 data: 2
6. Node 2 pre: 1
7. Node 2 next:3
8. Node 3 data: 3
9. Node 3 pre: 2
10. Node 3 next: 1
*/
/*--------------------------------------------------------------------------
Function Name:         set_debug_on
Purpose:               The function turns on debug messages
Description:           debug is set to TRUE so the debug messages will be
                       printed
Input:                 No parameter
Result:                No return value.
--------------------------------------------------------------------------*/
void set_debug_on (void) {
    debug_on = TRUE;/*debug messages will show*/
}


/*--------------------------------------------------------------------------
Function Name:         set_debug_off
Purpose:               Turns off debug messages
Description:           debug is set to FALSE so the debug messages will not be
                       printed
Input:                 No parameter
Result:                No return value.
--------------------------------------------------------------------------*/
void set_debug_off (void) {
    debug_on = FALSE;/*debug messages will not show*/
}

/*--------------------------------------------------------------------------
Function Name:         advance_next_List
Purpose:               Shifts the elements of the list forward by one.
Description:           The node pointer is assigned to the next one.
Input:                 this_list is a pointer to the list that will be shifted
Result:                No return value
--------------------------------------------------------------------------*/
void advance_next_List (List * this_list) {
    if (this_list==NULL){/*if this_list does not exist*/
        fprintf(stderr, ADNEXT_NONEXIST);/*nonexist error message is printed*/
    }
    else if (this_list->occupancy==0){/*if this_list is empty*/
        fprintf(stderr, ADNEXT_EMPTY);/*empty error message is printed*/
    }
    else {
        if (debug_on){/*if debug message is on*/
            fprintf(stderr, ADNEXT, this_list->list_count);/*message appears
	                                                   stating the list*/ 
        }
        this_list->front = this_list->front->next;/*sets front node pointer to
                                                next one*/
    }
}


/*--------------------------------------------------------------------------
Function Name:         advance_pre_List
Purpose:               Shifts the elements of the list backward by one.
Description:           The node pointer is assigned to the previous one.
Input:                 this_list is a pointer to the list that will be shifted
Result:                No return value.
--------------------------------------------------------------------------*/
void advance_pre_List (List * this_list) {
    if (this_list==NULL){/*if this_list does not exist*/
        fprintf(stderr, ADPRE_NONEXIST);/*nonexist error message is printed*/
    }
    else if (this_list->occupancy==0){/*if this_list is empty*/
        fprintf(stderr, ADPRE_EMPTY);/*empty error message is printed*/
    }
    else {
        if (debug_on){/*if debug message is on*/
            fprintf(stderr, ADPRE, this_list->list_count);/*message appears
	                                                  stating the list*/ 
        }
        this_list->front = this_list->front->pre;/*sets front node pointer to
                                               previous one*/
    }
}


/*--------------------------------------------------------------------------
Function Name:         delete_List
Purpose:               Deallocates memory from the list, and sets list pointer
                       to NULL
Description:           Deletes item from each end of list each time until one
                       item remains, then that item is deleted.
Input:                 lpp is a double pointer to the list that will be deleted
Result:                No return value.
--------------------------------------------------------------------------*/
void delete_List (List ** lpp) {
    List * this_List;/*local list pointer*/
    Node * working;/*Node pointer created*/

    
    this_List=*lpp;/*this_list set to list pointer*/ 
    working = this_List->front;/*local Node pointer set to front pointer*/
  
    if (this_List==NULL){/*if this_List does not exist*/
	fprintf(stderr, DELETE_NONEXIST);/*nonexisting list message displays*/
    }
    if (this_List->front==NULL){/*if Node does not exist*/
	fprintf(stderr, DELETE_NONEXISTNODE);/*nonexisting node message
	                                       displays*/
    }
    while (!isempty_List(this_List)){
	if (this_List->occupancy==1){/*loops until there is just one object
		                       left*/
	    delete_Node(&working, this_List->delete_func);
	    this_List->occupancy--;
	}
        else{
	    delete_Node(&(working->pre), this_List->delete_func);/*Calls
	         delete node and passes in address to working and delete_func*/
	    this_List->occupancy--;
	    working=working->next;  
	}
	
    }

    if (debug_on){/*if debug messages are on*/
        fprintf(stderr, LIST_DEALLOCATE, this_List->list_count);/*deallocate
	                                           message is printed*/
    }
    list_counter--;
    free(*lpp);
    *lpp=0;
    lpp=0;

}


/*--------------------------------------------------------------------------
Function Name:         find_location
Purpose:               Finds where the node should be in a sorted list
Description:           The function runs a loop where the value of the element
                       to be inserted is checked against the number where the
		       front pointer is currently pointing at. If the number to
		       be inserted is larger, it will move the front pointer to
		       the next Node and keep checking until the second number
		       is larger. That number will then be the "front" and the
		       element will be inserted before it.
Input:                 this_list is pointer to the list that will be checked
                       element is pointer to the object that will be inserted
Result:                Return value is static int to where the object will be
                       inserted
--------------------------------------------------------------------------*/
static Node * find_location (List * this_list, void * element) {
    Node * tempNode;/*temporary node pointer to hold front pointer*/
    Node * sortedNode;/*holds pointer to where new node will be inserted*/
    int nodeIndex=0;/*index for counting nodes*/

    tempNode= this_list->front;/*set tempNode to front pointer*/
    /*if the element is smaller than the first number in the list*/
    if(this_list->is_greater_than_func(view_Node(this_list->front), element)){
        originalfront=1;/*originalfront is set to 1 so advance_pre_List will be
	                  called*/
	return this_list->front;/*returns front pointer*/ 
    }
    /*loops until it returns*/   
    while(TRUE){
        if(this_list->is_greater_than_func(element,
		                           view_Node(this_list->front))){
	      /*calls is_greater_than_func to check if element is larger than
	       * the data for the node at front pointer. If it is, then we
	       * will move to the next Node.*/
            advance_next_List(this_list);/*calls advance_new_List to move the
	                                   front pointer of the list to the
	                                   next Node*/		   
	    nodeIndex++;/*nodeIndex increments*/
	    /*loops until nodeIndex reaches occupancy value. In the case if it
	     * is the largest number to be inserted into list*/
	    if (nodeIndex== this_list->occupancy){
	        sortedNode= this_list->front;/*new front pointer set to
		                               sortedNode*/
	        this_list->front= tempNode;/*front pointer set back to original
		                             place*/
		return sortedNode;/*pointer where new node will be inserted is
		                    returned*/
	    }
	
	} else { 
	/*it is false that element is larger than the data for the Node,
	       so this is where we will insert the the new Node. Return so that
	       the front pointer points to the Node we want to insert before*/
  	    sortedNode= this_list->front;/*new front pointer set to
		                               sortedNode*/
	    this_list->front= tempNode;/*front pointer set back to original
		                             place*/
	    return sortedNode;/*returns true inserting at the front of the
	                        list*/ 
	}
    } 
}


/*--------------------------------------------------------------------------
Function Name:         insert
Purpose:               Inserts element into this_list either at front, end, or
                       in sorted form
Description:           Checks if the element is to be inserted at the front,
                       element, or in sorted form. If it is sorted, then
		       find_location is called to find where to put the
		       element. Insert_Node is called to insert the element in
		       the correct location and the front pointer is changed
		       back accordingly. The method then returns true for
		       success.
Input:                 this_list is pointer to the list that will be checked
                       element is pointer to the object that will be inserted
		       where is the place in the list where element is stored
Result:                Return value is long
--------------------------------------------------------------------------*/
long insert (List * this_list, void * element, long where) {
    Node * localNode;/*the local Node pointer that will hold where front
                       pointer is */
    Node * returnedNode;/*another local Node pointer to hold the return of
                          insert_Node*/
    Node * tempNode;/*holds where the original front pointer is before
                        calling find_location*/
    int resetfront=0;/*if 1 then the front pointer will be moved back one*/
    
    
    if (this_list==NULL){/*if the list does not exist*/
        fprintf(stderr, INSERT_NONEXIST);
	return NULL;
    }
    if (this_list->occupancy==0){/*if the list is empty then it will
	               automatically go to call insert_Node for the null case*/
        localNode = NULL;/*set the Node to null*/
    }
    else{
        if(where == 2){/*if the node will be put in sorted order. Need to call
	             find_location to find out it will be inserted*/
	    tempNode= this_list->front;
	    localNode= find_location(this_list, element);/*Calls find_location
	                               and sets localNode to the return value*/
	}

        else if(where==1){/*if element will be inserted at front. The front
		   pointer will need to be moved back one to signify that the
		   newly inserted Node is the new front*/
	    localNode= this_list->front;/*set local Node to the front pointer*/
	    resetfront=1;/*resetfront set to 1 so advance_pre will be called*/
        }

        else if (where==0){/*if element will be inserted at end. Does not need
		          to shift front pointer anywhere later on.*/
            localNode= this_list->front;/*set local Node to the front pointer*/
	}
    }
    returnedNode= insert_Node(localNode, element, this_list->copy_func);
        /*insert_Node is called to insert the element before where
	 * localNode is. returnedNode takes returned pointer*/
    if (originalfront){/*if originalfront is 1 then the entire list will be
	                 shifted back*/
	advance_pre_List(this_list);
        originalfront=0;/*originalfront set back to 0*/
    }

    if (resetfront){/*runs if the element was inserted at the front*/
        advance_pre_List(this_list);/*the front pointer is shifted one back
	                              so it points at the newly inserted
				      item which is the new front*/
        resetfront= 0;/*resetfront is set back to 0*/
    }
    if (localNode==NULL){/*if it was the first Node to be inserted*/
        this_list->front= returnedNode;/*the front pointer is now set to the
	                              new Node*/
    }
    if (debug_on){/*if debug messages are on*/
        fprintf(stderr, INSERT, this_list->list_count);/*prints that an object
	                               was inserted into the particular stack*/
    }
    this_list->occupancy++;/*occupancy increments since a new Node is
                             inserted*/
    return TRUE;/*the element was successfully inserted*/

}


/*--------------------------------------------------------------------------
Function Name:         isempty_List
Purpose:	       Checks if this_list is empty
Description:           If this_list is NULL, then it does not exist and the
                       error message will be printed. Otherwise, if this_list's
		       occupancy is 0 then it will return true for empty. If
		       not, it is not empty.
Input:                 this_list is a pointer to the list that will be checked
Result:                Returns 1 for empty and 0 for not empty
                       Return value is long
--------------------------------------------------------------------------*/
long isempty_List (List * this_list) {
    /*Checks if the list exists or not. If not, the error message is
     * printed*/
    if (this_list==NULL){
        fprintf(stderr, ISEMPTY_NONEXIST);/*error message prints*/
        return FALSE;/*returns 0 for failure*/
    }
    /*Checks if the list is empty or not. If yes, 1 is returned for
    * true. Else, 0 is returned for false*/
    if (this_list->occupancy==0){
        return TRUE;/*it is true that the list is empty*/
    }
    else{
        return FALSE;/*it is false that the list is empty*/
    }
}


/*--------------------------------------------------------------------------
Function Name:         new_List
Purpose:	       Allocates memory and initializes a new List object
Description:           The method allocates memory for this_list and assigns
                       its pointers. The front pointer is set to NULL,
		       list_count is assigned to list_counter, occupancy is set
		       as NULL, and the list counter increments since a new
		       list thas been made.
Input:                 copy_func is a pointer to the function that makes copies
                       of the elements in this_list
                       delete_func is a pointer to the function that frees
		       memory of the elements in this_list
		       is_greater_than_func is a pointer to the function that
		       compares elements in this_list
		       write_func is a pointer to the function that writes
		       elements in this_list
Result:                Return value is List pointer to the new list.
--------------------------------------------------------------------------*/
List * new_List (
        void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) {
        
    /* allocate memory */
    List * this_list = (List *)malloc(sizeof(List));
    
    list_counter++;/*list counter is incremented since a list was created*/

    /* initialize */
    this_list->front = NULL;/*the front pointer is set to NULL*/
    this_list->list_count = list_counter;/*list count is assigned to list
        counter*/
    this_list->occupancy = NULL;/*the occupancy of the list is set to NULL*/
    this_list->copy_func = copy_func;/*copy_func for the list is set as
        copy_func*/  
    this_list->delete_func = delete_func;/*void pointer for the list is set to
        delete_func*/  
    this_list->is_greater_than_func = is_greater_than_func;
        /*is_greater_than_func for the list is set as is_greater_than_func*/
    this_list->write_func = write_func;/*File pointer for the list is set to
        write_func*/       
    if (debug_on){/*if debug messages are on*/
	fprintf(stderr, LIST_ALLOCATE, list_counter);/*message that a list
	    has been allocated*/
    }
    return this_list;/*returns list pointer to this_list*/
}


/*--------------------------------------------------------------------------
Function Name:         remove_List
Purpose:               Removes the element in this_list at where
Description:           Checks if this_list is exists and is not empty before
                       continuing. Removing is set as the item to be removed
		       and the front pointer will shift to the next one if we
		       are deleting the first item. Occupancy is decremented
		       and the data from the deleted Node is returned.
Input:                 this_list is a pointer to the list that will be affected
                       where is the place in the list that holds the element
Result:                No return value.
--------------------------------------------------------------------------*/
void * remove_List (List * this_list, long where) {
    Node * removing;/*Local node pointer holds where the element is that will
        be removed*/

    if (this_list==NULL){/*if the list does not exist*/
	fprintf(stderr, REMOVE_NONEXIST);/*error message for nonexisting list
	    prints*/
        return NULL;/*returns NULL since it is all types*/
    }
    else if (this_list->occupancy==0){/*if the list is empty*/
        fprintf(stderr, REMOVE_EMPTY);/*error message for empty list prints*/
        return NULL;/*returns NULL since it is all types*/
    }
    else{
        if (where == 1){/*if we are removing the item at the front*/
            removing= this_list->front;/*set removing as the front pointer*/
            advance_next_List(this_list);
        }
        if(where == 0){/*if we are removing the item at the end*/
            removing= this_list->front->pre;/*move the front pointer a node
	        before so it is looking at the last item*/
        }
	fprintf(stderr, REMOVE, this_list->list_count);/*remove print message
	    is displayed*/
	if (this_list->occupancy==1){/*if there is only one object left*/
	    this_list->front= NULL;/*set front pointer to NULL*/
	}
    
        this_list->occupancy--;/*occupancy is decremented since an object was
	    deleted*/
    }
    return remove_Node(removing);/*the data from the removed node is returned*/
}


/*--------------------------------------------------------------------------
Function Name:         view
Purpose:               Returns pointer to object stored at where to view it
Description:           Checks if this_list exists and is not empty before
                       continuing. If we are viewing the front then the local
		       pointer is set to the front pointer; if we are viewing
		       the end then it is set to the node before front.
		       The data of the particular Node is then returned
Input:                 this_list is a pointer to the list that will be checked
                       where is the place in the list that holds the element
		       to view
Result:                Return value is a void pointer .
--------------------------------------------------------------------------*/
void * view (List * this_list, long where) {
    Node * viewing;/*Local node pointer holds where the element is that will
        be viewed*/
    
    
    if (this_list ==NULL){/*if the list does not exist*/
	fprintf(stderr, VIEW_NONEXIST);/*nonexist message for list displays*/
        return NULL;/*returns NULL since it is all types*/   
    }
    else if (this_list->front==NULL){/*if the node does not exist*/
        fprintf(stderr, VIEW_NONEXISTNODE);/*nonexist message for node
	    displays*/
        return NULL;/*returns NULL since it is all types*/   
    }
    else if (this_list->occupancy==0){/*if the list is empty*/
	fprintf(stderr, VIEW_EMPTY);/*message for empty list displays*/
        return NULL;/*returns NULL since it is all types*/   
    }
    else{
        if (where == 1){/*if we are viewing the item at the front*/
            viewing= this_list->front;/*local pointer is assigned to
	        front pointer*/
	}
        if(where == 0){/*if we are viewing the item at the end*/
            viewing= this_list->front->pre;/*move the front pointer a node
	        before so it is looking at the last item*/
        }
        if (debug_on){/*if debug messages are on*/
	    fprintf(stderr, VIEW, this_list->list_count);/*prints the list
	        being viewed*/
	}
    }
    return view_Node(viewing);/*returns a void pointer*/ 
}



FILE * write_List (List * this_list, FILE * stream) {
        long count;             /* to know how many elements to print */
        Node * working;                 /* working node */

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_list) {
                fprintf (stderr, WRITE_NONEXISTLIST);
                return NULL;
        }

        if (stream == stderr)
                fprintf (stream, "List %ld has %ld items in it.\n",
                        this_list->list_count, this_list->occupancy);

        if (!this_list->write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        if (this_list->occupancy >= 1)
                working = this_list->front;

        for (count = 1; count <= this_list->occupancy; count++) {
                if (stream == stderr)
                        fprintf (stream, "\nelement %ld:  ", count);
                write_Node (working, stream, this_list->write_func);
                working = working->next;
        }
                
        return stream;
}


/*--------------------------------------------------------------------------
Function Name:         write_reverse_List
Purpose:               Writes this_list's elements backward and prints to
                       stream
Description:           The method will begin with the last element of the list
                       and print them backwards so that the first element is
		       printed last.
Input:                 this_list is a pointer to the list to write
                       stream is where to display, stdout or stderr
Result:                Return value is FILE
--------------------------------------------------------------------------*/
FILE * write_reverse_List (List * this_list, FILE * stream) {
    /* YOUR CODE GOES HERE */
    long count;             /* to know how many elements to print */
    Node * working;                 /* working node */

    if (!stream) {/*If the stream does not exist*/
        fprintf (stderr, WRITE_NONEXISTFILE);
        return NULL;/*returns NULL since it is all types*/   
    }

    if (!this_list) {/*if this_list does not exist*/
        fprintf (stderr, WRITE_NONEXISTLIST);
        return NULL;/*returns NULL since it is all types*/   
    }

    if (stream == stderr)/*if stream is stderr*/
        fprintf (stream, "List %ld has %ld items in it.\n",
        this_list->list_count, this_list->occupancy);/*prints the list and
	    number of items*/

    if (!this_list->write_func) {/*if there is no write_func*/
        fprintf (stream, WRITE_MISSINGFUNC);/*missing write func prints*/
        return stream;/*stream is returned*/
    }

    if (this_list->occupancy >= 1)
        working = this_list->front->pre;/*Node pointer working is set as the
	    node before the front*/
    /*loops as long as there are elements. Prints them in reverse order*/
    for (count = this_list->occupancy; count >= 1; count--) {
        if (stream == stderr)
	    fprintf (stream, "\nelement %ld:  ", count);/*prints the element*/
            write_Node (working, stream, this_list->write_func);/*Calls
	        write_Node and passes in local pointer*/
            working = working->pre;/*working is set as the node before each
	        time*/
    }
                
    return stream;

}

static void delete_Node (Node ** npp, void (*delete_func) (void *)) {

        /* does the node exist??? */
        if (!npp || !*npp) {
                fprintf (stderr, DELETE_NONEXISTNODE);
                return;
        }

        /* call function to delete element */
        if (delete_func && (*npp)->data)
                (*delete_func) (&((*npp)->data));

        /* delete element */
        free (*npp);

        /* assign node to NULL */
        *npp = NULL;
}


/*--------------------------------------------------------------------------
Function Name:         insert_Node
Purpose:               Creates a new node to hold element or a copy of it
Description:           If this_Node is NULL, then we are inserting the first
                       node and pre and next points to itself. Otherwise, the
		       new node is integrated into the list before this_Node
		       and its pre and next are assigned correctly to the Nodes
		       before and after it.//originalfront= 1;

Input:                 this_Node is a pointer to the Node that the new node is
                       inserted before
		       element is a pointer to the object that will be stored
		       copy_func is a pointer to the copy function to be
		       passed into list constructor
Result:                Return value is node pointer.
--------------------------------------------------------------------------*/
static Node * insert_Node (Node * this_Node, void * element, 
    void * (*copy_func) (void *)) {
    Node * working= new_Node(element, * copy_func);/*creates new Node*/
    
    
    if (this_Node==NULL){/*if this_Node is NULL then we are inserting the
		               first Node*/
        working->next = working;/*next points to working itself*/
        working->pre = working;/*pre points to working itself*/

    }
    else{/*the Node we want to insert before*/
        working->next = this_Node;/*puts working on RTS before this_Node*/
        working->pre = this_Node->pre;/*attaches the new Node to the list
	    by setting this_Node's pre as working's pre*/
	this_Node->pre = working;/*workign is set as this_Node's pre*/ 
	working->pre ->next = working;/*next node after working's pre is
	    working*/
    }
    return working;/*returns Node pointer to node inserted*/
}

static Node* new_Node (void * element, void * (*copy_func) (void *)) {

        /* allocate memory */
        Node *this_Node = (Node *) malloc (sizeof (Node));

        /* initialize memory */
        this_Node->next = this_Node->pre = NULL;
        this_Node->data = (copy_func) ? (*copy_func) (element) : element;

        return this_Node;
}


/*--------------------------------------------------------------------------
Function Name:         remove_Node
Purpose:               Frees the memory associated with the Node but does not
                       delete Node's data
Description:           Datas is set as this_Node's data. The pointers of the
                       objects around this_Node are assigned so that they do
		       not point to this_Node. this_Node is deleted and the
		       data is returned.
Input:                 this_Node is a pointer to the Node to be removed
Result:                Return value is void pointer to the data
--------------------------------------------------------------------------*/
static void * remove_Node (Node * this_Node) {
    void * datas;/*the data from the removed Node*/
    
    datas= this_Node->data;/*sets datas to this_Node's data*/
    this_Node->next->pre=this_Node->pre;/*sets pre pointer of the element
        after this_Node to the element before this_Node*/
    this_Node->pre->next=this_Node->next;/*sets next pointer of the element
        in front of this_Node to the element after this_Node*/  
    delete_Node(&this_Node, NULL);/*deletes the node*/
    free(this_Node);
    return datas;/*returns the data of the deleted node*/

}


/*--------------------------------------------------------------------------
Function Name:         view_Node
Purpose:               Returns pointer to this_Node's data
Description:           The function returns a pointer to this_Node's data
Input:                 this_Node is a pointer to the Node to be viewed
Result:                Return value is null pointer
--------------------------------------------------------------------------*/
static void * view_Node (Node * this_Node) {
    return this_Node->data;/*returns the data of the node we want to see*/
}

static FILE* write_Node (Node * this_Node, FILE * stream,
        FILE * (*write_func) (void *, FILE *)) {

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_Node) {
                fprintf (stream, WRITE_NONEXISTNODE);
                return stream;
        }

        if (!write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        return (*write_func) (this_Node->data, stream);
}
