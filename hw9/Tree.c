/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        3/12/2015 
                                                        cs12xkf 
                                Assignment Nine 
 
File Name:      Tree.c 
Description:    The program creates a binary tree on disk where objects can be
                inserted as nodes and be organized in alphabetical order. The
                tree starts with root and sorts by placing items earlier in the
                alphabet on the left of nodes and items later in the alphabet
                on the right of nodes. Occupancy of the tree is the number
                of nodes. The methods to insert, remove, lookup, are done
                recursively. The height and balance of nodes are updated each
                time a node is inserted by calling SetHeightAndBalance. If a
                node is out of balance, it is taken out and reinserted in the
                tree. If the node to be removed has two children, it will be
                replaced by its predecessor. AT ^D the cost and operation is
                printed. Cost refers to the amount of times a node was read or
                written while operation is the amount of times Tree's
                lookup/insert/remove was called.
****************************************************************************/ 
#include <stdlib.h>
#include <string.h>
#include "Tree.h"

// debug messages
static const char ALLOCATE[] = " - Allocating]\n";
static const char COST_READ[] = "[Cost Increment (Disk Access): Reading ";
static const char COST_WRITE[] = "[Cost Increment (Disk Access): Writing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char TREE[] = "[Tree ";

template <class Whatever>
int Tree<Whatever>::debug_on = 0;

template <class Whatever>
long Tree<Whatever>::cost = 0;

template <class Whatever>
long Tree<Whatever>::operation = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);
template <class Whatever>
struct  TNode {
/*****************************************************************************
* Name: TNode
* File: Tree.c
* Purpose: Provides the member methods and declares/initializes the offsets/
* data/height/balance of TNode objects. The read and write constructors read
* nodes from disk to memory and write nodes from memory to the binary tree on
* disk. TNode methods such as lookup/insert/remove are called on by Tree
* methods of the same name. Writing and reading nodes will increment the cost.
*****************************************************************************/

// friends:

// data fields:
    Whatever data;/*data of the node*/
    long height;/*height of the node*/
    long balance;/*balance of the node*/
    offset left;/*left offset of the node*/
    offset right;/*right offset of the node*/
    offset this_position;	// current position

// function fields:
    TNode () : height (0), balance (0), left (0), right (0), 
        this_position (0) {}

    // to declare the working TNode in Tree's Remove
    TNode (Whatever & element) : data (element), height (0), balance (0),
        left (0), right (0), this_position (0) {}
	
    TNode (Whatever &, fstream *, long &);	// to add new node to disk
    TNode (const offset &, fstream *);	// to read node from disk
	
    unsigned long Insert (Whatever &, fstream *, long &, offset &);
    // optional recursive Lookup declaration would go here
    unsigned long Lookup(Whatever & element, fstream * fio) const;
    void Read (const offset &, fstream *);	// read node from disk
    unsigned long Remove (TNode<Whatever> &, fstream *, long &, offset &,
        long fromSHB = FALSE);
    void ReplaceAndRemoveMax (TNode<Whatever> &, fstream *, offset &);
    void SetHeightAndBalance (fstream *, offset &);
    void Write (fstream *) const;		// update node to disk

    ostream & Write_AllTNodes (ostream &, fstream *) const;
};

/*****************************************************************************
* Name: Tree (class)
* File: Tree.c
* Purpose: Creates the binary tree on disk. The constructor initializes root,
* tree_count, and occupancy of the tree. If the datafile is empty, root and
* occupancy are written to file. If the datafile has data, root and occupancy
* are read into memory. The destructor deallocates the tree and deletes the
* elements in the tree. Calling on Tree operations lookup/remove/insert will
* increment the operation count.
******************************************************************************/

template <class Whatever>
void Tree<Whatever> :: Set_Debug_On (void)
/************************************************************************
* Name: Set_Debug_On
* File: Tree.c
* Purpose: Turns debug messages on
* Parameter: debug is set to nonzero value so debug messages are on
* @return: None. 
************************************************************************/
{
    debug_on= true;/*turns debug messages on*/
}

template <class Whatever>   
void Tree<Whatever> :: Set_Debug_Off (void)
/**************************************************************************
* Name: Set_Debug_Off
* File: Tree.c
* Purpose: Turns debug messages off
* Parameter: debug is set to zero value so debug messages are off
* @return: None. 
************************************************************************/
{
    debug_on= false;/*turns debug messages off*/
}


template <class Whatever>
unsigned long Tree<Whatever> :: Insert (Whatever & element)
/************************************************************************
* Name: Insert(Tree)
* File: Tree.c
* Purpose: Inserts an element in the binary tree. If tree is empty, the
* method inserts the new TNode as root. If the tree is not empty, the method
* delegates to TNode's insert. IncrementOperation is called since insert counts
* as an operation. Returns true for successful insert and false otherwise.
* Parameter: element is the data to be stored in the TNode.
* @return: Returns true for successful insert and false for failure.
************************************************************************/
{
    if(!occupancy){/*if the first TNode hasn't been inserted; no root*/
        TNode<Whatever> rootNode(element, fio, occupancy);/*create root
                                                            TNode with write
                                                            constructor*/
    }
    else {/*if Tree is not empty, delegates to TNode's Insert*/
        TNode<Whatever> readRootNode(root, fio);/*create temp node with read
                                                  constructor using root
                                                  offset*/
        readRootNode.Insert(element, fio, occupancy, root);/*calls insert
                                                             with temp node*/
    }
    IncrementOperation();/*increment operation since insert was called*/
    return true;/*inserted successfully*/
}

template <class Whatever>
void TNode<Whatever> :: ReplaceAndRemoveMax (TNode<Whatever> & targetTNode, 
	fstream * fio, offset & PositionInParent)
/**************************************************************************
* Name: ReplaceAndRemoveMax (TNode)
* File: Tree.c
* Purpose: Called when removing a TNode with 2 children. The method moves
* left and then all the way right by recursively calling itself until it
* reaches the predecessor node. The TNode with 2 children takes on its
* predecessor's data.
* Parameter: targetTNode is the TNode with 2 children to be removed. It
* will take on it's predecessor's data.
* fio is the filestream corresponding to the datafile where Tree is stored.
* PositionInParent is a reference to the position in parentTNode corresponding
* to currentTNode's offset in datafile.
* @return: No return value.
************************************************************************/
{
    if(right){/*if a right child exists*/
        TNode<Whatever> rightTNode(right, fio);/*create a temp Node with
                                                 read constructor using right
                                                 offset*/
        rightTNode.ReplaceAndRemoveMax(targetTNode, fio, right);/*recursively
                                                                  call itself
                                                                  to get to
                                                                  rightmost
                                                                  child*/
        SetHeightAndBalance(fio,PositionInParent);/*Call SetHeightAndBalance to
                                                    update the nodes after
                                                    recursion*/
    }
    else{/*current node is the rightmost node*/
        targetTNode.data=data;/*targetTNode takes on the data of its
                                predecessor*/
        PositionInParent=left;/*PositionInParent is set to the left*/
    }
}

template <class Whatever>
unsigned long TNode<Whatever> :: Remove (TNode<Whatever> & elementTNode,
	fstream * fio, long & occupancy, offset & PositionInParent,
	long fromSHB)
/**************************************************************************
* Name: Remove (TNode)
* File: Tree.c
* Purpose: Called from Tree's Remove if the tree is not empty. Removes the
* matching data that is passed in from the tree. The method is recursive
* and calls itself each time to shift left or right. If data matching to
* elementTNode's data is found, the function sets elementTNode's data as data,
* calls SetHeightAndBalance if it was not called from there and returns true. 
* Parameter: elementTNode is the TNode to be removed from tree.
* fio is the filestream corresponding to the datafile where Tree is stored.
* occupancy is a reference to the occupancy of the tree. 
* PositionInParent is a reference to the position in parentTNode corresponding
* to currentTNode's offset in datafile.
* long fromSHB is whether or not Remove was called from SetHeightAndBalance.
* @return: Returns true if successfully removed, false if not. 
***************************************************************************/
{
    long returnVal;/*holds return value of each recursive call to remove*/

    if (data == elementTNode.data){/*if element is equal to the current node's
                                     data then the node has been found*/
        elementTNode.data=data;/*set current elementTNode's data as Tnode's
                                 data*/ 

        /*TNode has 2 children case*/
        if(left && right){/*if TNode has left and right children*/
            TNode<Whatever> leftTNode(left,fio);/*create a temp node with left
                                                  offset and read constructor*/
            leftTNode.ReplaceAndRemoveMax(*this, fio, left);/*calls RARM on the
                                                              left child*/
            if (!fromSHB){/*if Remove was NOT called from SHB*/
                if (PositionInParent){/*if the PositionInParent exists*/
                    SetHeightAndBalance(fio, PositionInParent);/*Call SHAB to
                                                                 update height
                                                                 and balance*/
                    return true;/*return true since node was removed*/
                }
            }
            Write(fio);/*writes fio after call to RARM*/
        }

        /*Not 2 children case*/
        else{/*If the TNode does not have 2 children*/
            if(left){/*if the node has a left child*/
                PositionInParent= left;/*reassign PositionInParent to left
                                         offset*/
            }
            else if(right){/*if the node has a right child*/
                PositionInParent= right;/*reassign PositionInParent to right
                                          offset child*/
            }
            else{/*if the node has no children*/
                PositionInParent= 0;/*set the PositionInParent to 0*/
            }
        }

        return true;/*returns true since node was deleted*/
    }

    else {/*the current node is not the one we are looking for*/  
        if (elementTNode.data > data){/*if element is greater than the current
                                        node*/
            if(right){/*if a right node exists*/
                TNode<Whatever> rightTNode(right, fio);/*create a temp node
                                                         with right offset*/
                returnVal=rightTNode.Remove(elementTNode, fio, occupancy,
                right, false);/*recurses to move right*/
            }
            else{/*the right node does not exist*/
                return false;/*return false since the node is not there*/
            }
        }
        else{/*the element is less than the current node*/
            if (left){/*if a left node exists*/
                 TNode<Whatever> leftTNode(left, fio);/*create a temp node
                                                        with left offset*/
                 returnVal=leftTNode.Remove(elementTNode, fio, occupancy, left,
		         false);/*recurses to shift left*/
            }
            else{/*the left node does not exist*/
                return false;/*return false since the node is not there*/
            }
        }
    }

    if (returnVal){/*if the return value of Remove is true*/
        SetHeightAndBalance(fio, PositionInParent);/*Call SetHeightAndBalance
                                                     to update current node's
                                                     height and balance*/
        return true;/*returns true for success*/
    }
    else{/*if remove returned false for failure to find the node*/
        return false;/*returns false for failure*/
    } 
}
	
template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element)
/************************************************************************
* Name: Remove (Tree)
* File: Tree.c
* Purpose: Removes an element from the tree. If the tree is not empty, the
* function calls TNode's Remove. If the node is successfully removed,
* the method returns true. Occupancy is decremented since a node is removed. 
* IncrementOperation is called since remove counts as an operation. 
* Parameter: element is the data to be searched in the tree and removed.
* @return: Returns true if successfully removed, false if not. 
************************************************************************/
{
    long returnVal;/*return value of TNode's remove*/

    if(occupancy){/*if the tree is not empty*/
        TNode<Whatever> elementTNode(element);/*temporary TNode to store
                                                element*/
	    TNode<Whatever> readRoot(root, fio);/*create a temp node with root
                                              offset using read constructor*/
	    returnVal=readRoot.Remove(elementTNode, fio, occupancy, root, false);
        /*calls TNode's Remove to remove element from tree and sets returnVal*/
        element= elementTNode.data;/*set element as elementTNode's data from
                                     the TNode remove method*/
        IncrementOperation();/*increments operation since remove is called*/
        
	    if (returnVal){/*if TNode's remove returns true*/
            if(!root){/*if the root was deleted*/
                ResetRoot();/*call ResetRoot to reset the root to end of
                              datafile*/
            }
            occupancy--;/*decrement occupancy since TNode was removed*/
            return true;/*successfully removed element*/
        }
    }
    return false;/*tree is empty so there is nothing to remove*/
}

template <class Whatever>
void TNode<Whatever> :: SetHeightAndBalance (fstream * fio,
	offset & PositionInParent)
/**************************************************************************
* Name: SetHeightAndBalance
* File: Tree.c
* Purpose: Updates the height and balance of the current TNode. If the
* subsequent balance is greater than an absolute value of
* 2, then the node is removed with TNode's remove and re-inserted into the
* tree with TNode's insert.
* Parameter: fio is the filestream corresponding to the datafile where Tree is
* stored.
* PositionInParent is a reference to the position in parentTNode corresponding
* to currentTNode's offset in datafile.
* @return: No return value.
***************************************************************************/
{
   
    long fakeOccupancy;/*substitute variable for occupancy to be passed into
                         methods since we cannot access actual occupancy*/
    if(!left && right){/*if there is a right node and no left node*/
        TNode<Whatever> tempRight(right, fio);/*create a temp node with right
                                                offset using read constructor*/
	    height=tempRight.height+1;/*set height as 1 more than the right's
                                    height*/
        balance= -1-tempRight.height;/*set balance as -1 minus the right's
                                       height*/
    }   
    else if(!right && left){/*if there is a left node and no right node*/
        TNode<Whatever> tempLeft(left,fio);/*create a temp node with left
                                             offset using read constructor*/
        height=tempLeft.height+1;/*set height as 1 more than the left's
                                   height*/
        balance=tempLeft.height+1;/*set balance as the left's height minus -1*/
    }
	
    else if (right && left){/*if there is both a left and right*/
        TNode<Whatever> tempLeft(left,fio);/*create a temp node with left
                                             offset using read constructor*/
        TNode<Whatever> tempRight(right,fio);/*create temp node with right
                                               offset using read constructor*/
        balance=tempLeft.height-tempRight.height;/*balance is the left's height
                                                   minus right's height*/
        if((tempLeft.height) > (tempRight.height)){/*if left node is taller
                                                     than the right*/
            height=(tempLeft.height)+1;/*height is 1 more than left node's
                                         height*/
        }
        else{/*if right node is taller than left*/
            height=(tempRight.height)+1;/*height is 1 more than right node's
                                          height*/
        }
    }
    else{/*if PositionInParent has no children*/
        height= 0;/*set the height as 0*/
        balance= 0;/*set the balance as 0*/
	
    }
    /*Out of balance case*/
    if(abs(balance) > THRESHOLD){/*if the absolute value of balance is more
                                   than the threshold*/
        TNode<Whatever> origTNode(data);/*create a TNode to hold the data that
                                          will be replaced by predecessor*/
        Remove(*this,fio,fakeOccupancy,PositionInParent,true);/*Call Remove
                                                                which calls
                                                                RARM and
                                                                replaces node
                                                                with
                                                                predecessor*/
        TNode<Whatever> tempNode(PositionInParent, fio);/*create temp Node with
                                                          PositionInParent
                                                          using read
                                                          constructor*/
        tempNode.Insert(origTNode.data,fio,fakeOccupancy,PositionInParent);
        /*Call Insert to reinsert the data of the node*/
    }
    else{
        Write(fio);/*writes fio if the node is not out of balance*/
    }
}

template <class Whatever>
long Tree <Whatever> :: GetCost ()
/************************************************************************
* Name: Read (TNode)
* File: Tree.c
* Purpose: Gets the value of the long cost
* Parameter: None
* Return: Returns the value of the cost variable
*************************************************************************/
{
    return cost;/*returns the cost*/
}

template <class Whatever>
long Tree <Whatever> :: GetOperation ()
/************************************************************************
* Name: GetOperation (TNode)
* File: Tree.c
* Purpose: Gets the value of the long operation
* Parameter: None
* Return: Returns the value of the operation variable
*************************************************************************/
{
    return operation;/*returns the operation*/
}

template <class Whatever>
void Tree <Whatever> :: IncrementCost ()
/************************************************************************
* Name: IncrementCost
* File: Tree.c
* Purpose: Increments the cost when a read or write to disk occurs
* Parameter: None 
* Return: None  
************************************************************************/
{
    cost++;/*increments the cost when a read or write to disk occurs*/
}

template <class Whatever>
void Tree <Whatever> :: IncrementOperation ()
/************************************************************************
* Name: IncrementOperation
* File: Tree.c
* Purpose: Increments operation whenever a Tree operation such as
* insert/lookup/remove is called.
* Parameter: None
* Return: None
*************************************************************************/
{
    operation++;/*increments operation when a Tree operation occurs*/
}

template <class Whatever>
void Tree <Whatever> :: ResetRoot ()
/************************************************************************
* Name: ResetRoot
* File: Tree.c
* Purpose: Called when the root node is deleted from tree. The method seeks to
* the end of the file and sets the root offset at that location.
* Parameter: None
* Return: None
*************************************************************************/
{
    fio->seekg(0,ios::end);/*seek to end of file*/
    root= fio->tellg();/*set root offset as the current position*/
}

template <class Whatever>
unsigned long TNode<Whatever> :: Insert (Whatever & element, fstream * fio,
	long & occupancy, offset & PositionInParent)
/**************************************************************************
* Name: Insert(TNode)
* File: Tree.c
* Purpose: Called from Tree' Insert when tree is not empty. In the case of
* a duplicate insert, the TNode's data is replaced with element. To move down
* the tree, the method calls itself recursively until the node to be inserted
* is in the right spot alphabetically. A new node is then written to disk at
* that location with element as its data. SetHeightAndBalance is called after
* each recursive step to update height and balance.
* Parameter: element is the data to be inserted in the tree
* fio is the filestream corresponding to the datafile where Tree is stored
* occupancy refers to the occupancy of the tree
* PositionInParent is a reference to the position in parentTNode corresponding
* to currentTNode's offset in datafile.
* @return: Returns true if successfully inserted, false otherwise.
************************************************************************/
{

    /*Duplicate case*/
    if(data== element){/*if element equals current node*/
        data=element;/*set data as element*/
        Write(fio);/*writes fio*/
	    return true;/*returns true*/
    }

    /*Insert or move right*/
    if (element > data){/*if element is greater than the current node*/
        if (!right){/*if there is nothing to the right*/
            TNode<Whatever> rightTNode(element, fio, occupancy);/*creates new
                                                                  node using
                                                                  write
                                                                  constructor*/
            right= rightTNode.this_position;/*set right offset as the position
                                              of rightTNode*/
        }
        else{/*a node exists on the right*/
            TNode<Whatever> rightTNode(right, fio);/*makes a temp node with
                                                     right offset using read
                                                     constructor*/
            rightTNode.Insert(element,fio,occupancy, right);/*calls Insert with
                                                              the temp node*/
        }
    }

    /*Insert or move left*/
    else if(data > element){/*if element is smaller than the current node*/
        if (!left){/*if there is no node on the left*/
            TNode<Whatever> leftTNode(element, fio, occupancy);/*create new
                                                                 node using
                                                                 write
                                                                 constructor*/
            left= leftTNode.this_position;/*set left offset as the position of
                                            leftTNode*/
        }
        else{/*a node exists on the left*/
            TNode<Whatever> leftTNode(left, fio);/*makes a temp node with left
                                                   offset using read
                                                   constructor*/
            leftTNode.Insert(element,fio,occupancy, left);/*calls Insert with
                                                            the temp node*/
        }
    }
    SetHeightAndBalance(fio, PositionInParent);/*Call SetHeightAndBalance to
                                                 update height and balance*/
    return true;/*successfully inserted*/
}


template <class Whatever>
unsigned long TNode<Whatever>::Lookup(Whatever & element, fstream * fio) const
/**************************************************************************
* Name: Lookup(TNode)
* File: Tree.c
* Purpose: Looks up if the data is in the tree or not. Recursively calls itself
* as it goes through the tree, going left if the data is smaller than that of
* the current node and right if the data is larger. If a matching node is
* found, the method returns true. If not, then the method returns false for
* unsuccessful search.
* search.
* Parameter: element is the data we are searching for in the tree
* fio is the filestream corresponding to the datafile where Tree is stored
* @return: True if the node was found in the tree, false otherwise. 
************************************************************************/
{
    long returnVal;/*holds the return value of lookup*/
    
    /*Found a match*/
    if(data== element){/*if element equals current node*/
        element=data;/*sets element as the current node's data*/
        return true;/*returns true since node was found*/	
    }
    /*Looking right*/
    else if (element > data){/*if element is greater than the
                               current node*/
        if (!right){/*if there is nothing to the right*/
            return false;/*returns false since node is not found*/  
        }
        else{/*a node exists on the right*/
            TNode<Whatever> rightTNode(right, fio);/*create a temp node with
                                                     read constructor using
                                                     right offset*/
            returnVal= rightTNode.Lookup(element,fio);/*recursively call Lookup
                                                         to move right. Set
                                                         returnVal as Lookup's
                                                         return value*/
	    }
    }
    /*Looking left*/
    else{/*if element is smaller than the current node*/
        if (!left){/*if there is no node on the left*/
            return false;/*returns false since node was not found*/
        }
        else{/*a node exists on the left*/
            TNode<Whatever> leftTNode(left, fio);/*create a temp node with
                                                   read constructor using left
                                                   offset*/
            returnVal=leftTNode.Lookup(element, fio);/*recursively call Insert
                                                       to move left. Set
                                                       returnVal as Lookup's
                                                       return value*/ 
        }
    }

    /*after each recursive call*/
    if (returnVal){/*if the return value of lookup is true*/
        return true;/*returns true since node was found*/
    }
    else{/*if the return values of the recursive lookups are false*/
        return false;/*returns false since node was not found*/
    }
}

template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const
/**************************************************************************
* Name: Lookup(Tree)
* File: Tree.c
* Purpose: Looks up if the data is in the tree or not.If the tree is not empty
* then TNode's lookup is called with the data passed as element. If it is 
* empty then the method returns false since the node is not there.
* IncrementOperation is called since lookup counts as an operation. 
* Parameter: element is the data we are searching for in the tree.
* @return: True if the node was found in the tree, false otherwise. 
************************************************************************/
{
    IncrementOperation();/*increments operation since lookup counts as an
                           operation*/
    if (occupancy){/*if the tree is not empty*/
        TNode<Whatever> rootNode(root, fio);/*create temp node with read
                                              constructor using root offset*/
        return rootNode.Lookup(element, fio);/*delegates to TNode's lookup and
                                               returns its return value*/
    }
    else{/*if the tree is empty*/
        return false;/*return false since the node is not there*/
    }
}

template <class Whatever>
void TNode<Whatever> :: Read (const offset & position, fstream * fio)
/************************************************************************
* Name: Read (TNode)
* File: Tree.c
* Purpose: Reads a TNode from position on the datafile into memory. TNode's
* information overwrites this TNode's data. The method is called from the
* read constructor.
* Parameter: position is the offset for the position of the TNode.
* fio is the filestream corresponding to the datafile where Tree is stored.
************************************************************************/
{
    fio->seekg(position);/*seek to passed-in position*/
    this_position= fio->tellg();/*set this_position as the current position*/
    fio->read((char *)this, sizeof(TNode<Whatever>));/*TNode's information in
                                                       datafile overrides
                                                       current TNode's data*/
    if(Tree<Whatever>::debug_on){/*if debug messages are on*/
        cerr<<COST_READ<<(const char *)this<<"]"<<'\n';/*prints that the
                                                         current node is being
                                                         read*/
    }
    Tree<Whatever>::IncrementCost();/*increment cost since read was performed*/
}

template <class Whatever>
TNode<Whatever> :: TNode (const offset & position, fstream * fio)
/************************************************************************
* Name: Read constructor (TNode)
* File: Tree.c
* Purpose: Called when reading a TNode on the disk into memory.
* Parameter: position is the offset for the position of the TNode.
* fio is the filestream corresponding to the datafile where Tree is stored.
************************************************************************/
{
    Read(position, fio);/*delegates to Read method*/
}

template <class Whatever>
TNode<Whatever> :: TNode (Whatever & element, fstream * fio, long & occupancy):
                        data (element), height (0), balance (0), left (0), 
                        right (0)
/************************************************************************
* Name: Write constructor (TNode)
* File: Tree.c
* Purpose: Called when creating a TNode for the first time. Initializes the
* TNode's data/height/balance/left/right. Calls TNode's write method to write
* this TNode to disk.
* Parameter: element is the data to be stored in TNode.
* fio is the filestream corresponding to the datafile where Tree is stored.
* occupancy is a reference to the Tree's occupancy, in which the TNode will
* be added.
************************************************************************/
{
    fio->seekp(0,ios::end);/*seeks to end*/
    this_position= fio->tellg();/*sets this_position as end*/
    Write(fio);/*delegates to Write method*/
    occupancy++;/*occupancy increments since a TNode was written to disk*/
}

template <class Whatever>
void TNode<Whatever> :: Write (fstream * fio) const
/************************************************************************
* Name: Write (TNode)
* File: Tree.c
* Purpose: Writes this TNode to disk at this_position in datafile. The
* method is called by TNode's write constructor.
* Parameter: fio is the filestream corresponding to the datafile where Tree
* is stored.
************************************************************************/
{
    if(Tree<Whatever>::debug_on){/*if debug messages are on*/
        cerr<<COST_WRITE<<(const char *)data<<"]"<<'\n';/*prints that the TNode
                                                          is being written to
                                                          disk*/
    }
    fio->seekp(this_position);/*seek to this_position*/
    fio->write((const char *)this, sizeof(TNode<Whatever>));/*TNode writes
                                                              itself to disk*/
    Tree<Whatever>::IncrementCost();/*increments cost since write was
                                      performed*/
}

template <class Whatever>
Tree<Whatever> :: Tree (const char * datafile) :
        fio (new fstream (datafile, ios :: out | ios :: in)), 
	     root(0), tree_count(0)
/************************************************************************
* Name: Tree constructor
* File: Tree.c
* Purpose: Allocates the tree object. If the datafile is empty, root and
* occupancy are initialized and written to the file. If the datafile has
* data, root and occupancy are read into memory.
* Parameter: fio is the filestream corresponding to the datafile where Tree
* is stored
************************************************************************/
{
    fio->seekg(0, ios::beg);/*seek to end of file*/
    offset beginning=fio->tellg();/*tells where pointer is*/
    fio->seekg(0, ios::end);/*seek to end of file*/
    offset ending=fio->tellg();/*tells where pointer is*/
    tree_count++;/*increment tree_count since a tree is allocated*/
    if (Tree<Whatever>::debug_on){/*if debug messages are on*/
        cerr<<TREE<<tree_count<<ALLOCATE;/*print that tree was allocated*/
    }
    if (beginning==ending){/*file is empty*/
        occupancy=0;/*initialize occupancy*/
        fio->seekp(0,ios::beg);/*seek to beginning of file*/
        fio->write((const char *)&root, sizeof(root));/*reserve space for root*/
        fio->write((const char *)&occupancy, sizeof(occupancy));/*reserve space
                                                                  for
                                                                  occupancy*/
        root=fio->tellp();/*tells where pointer is*/
    }
    else{/*if there is data in the datafile*/
        fio->seekg(0, ios::beg);/*seek to beginning of file*/
        fio->read((char *)&root, sizeof(root));/*reads in values for root*/
        fio->read((char *)&occupancy, sizeof(occupancy));/*reads in values for
                                                           occupancy*/
    }
}

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
    fio->seekp(0, ios::beg);/*moves pointer to beginning of file*/
    fio->write((const char *)&root, sizeof(root));/*writes root to disk*/
    fio->write((const char *)&occupancy, sizeof(occupancy));/*writes occupancy
                                                              to disk*/
    if(debug_on){/*if debug messages are on*/
        cerr<<TREE<<tree_count<<DEALLOCATE;/*prints that tree was deallocated*/
    }
    delete fio;/*deletes fio*/
}/* end: ~Tree */

template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
	stream << "at height:  :" << nnn.height << " with balance:  "
		<< nnn.balance << "  ";
	return stream << nnn.data << "\n";
}

template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
% 
% Description : This funtion will output the contents of the Tree table
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
        long old_cost = cost;

	stream << "Tree " << tree_count << ":\n"
		<< "occupancy is " << occupancy << " elements.\n";

	fio->seekg (0, ios :: end);
	offset end = fio->tellg ();

	// check for new file
	if (root != end) {
		TNode<Whatever> readRootNode (root, fio);
		readRootNode.Write_AllTNodes (stream, fio);
	}

        // ignore cost when displaying nodes to users
        cost = old_cost;

	return stream;
}

template <class Whatever>
ostream & TNode<Whatever> ::
Write_AllTNodes (ostream & stream, fstream * fio) const {
	if (left) {
		TNode<Whatever> readLeftNode (left, fio);
		readLeftNode.Write_AllTNodes (stream, fio);
	}
	stream << *this;
	if (right) {
		TNode<Whatever> readRightNode (right, fio);
		readRightNode.Write_AllTNodes (stream, fio);
	}

	return stream;
}

