/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        3/1/2015 
                                                        cs12xkf 
                                Assignment Eight 
 
File Name:      Tree.c 
Description:    The program creates a binary tree where objects can be inserted
                as nodes and be organized in alphabetical order.The tree starts
		with root and sorts by placing items earlier in the alphabet
		on the left of nodes and items later in the alphabet on the
		right of nodes. The occupancy of the tree is the number
		of nodes and in the case of duplicate insertions does not
		change. The methods to insert, remove, lookup, and delete all
		nodes are all done recursively. Deleting nodes will decrease
		the occupancy. The height and balance of nodes are updated each
		time a node is inserted by calling SetHeightAndBalance. If a
		node is out of balance, it is taken out and reinserted in the
		tree. If the node to be removed has two children, it will be
		replaced by its predecessor. After ^D all the nodes in the
		tree are deleted.
****************************************************************************/ 
#include <cstdlib>
#include <string>
#include "Tree.h"
using namespace std;

// messages
static const char AND[] = " and ";
static const char COMPARE[] = " - Comparing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char INSERT[] = " - Inserting ";
static const char REPLACE[] = " - Replacing ";
static const char UPDATE[] = " - Updating ";

template <class Whatever>
int Tree<Whatever>::debug = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);

/****************************************************************************
* Name: TNode (class)
* Purpose: Contains the class methods and pointers/variables (pointers to left
* and right child, height, balance, data) associated with TNode objects. When
* the TNode is created, its balance, height, data, occupancy, left and right
* pointers, and tree_count are initialized.
*****************************************************************************/
template <class Whatever>
struct TNode {
    long balance;/*balance of the TNode*/
    Whatever data;/*data of the TNode of class Whatever*/
    long height;/*height of the TNode*/
    TNode<Whatever> * left;/*pointer to the left child*/
    long & occupancy;/*occupancy of the tree*/
    TNode<Whatever> * right;/*pointer to right child*/
    unsigned long & tree_count;/*which number tree this is*/
        
    TNode (const Whatever & element, Tree<Whatever> & theTree)
        : balance (0), data (element), height (0), left (0), 
        occupancy (theTree.occupancy), right (0), 
        tree_count (theTree.tree_count)
    /*************************************************************************
    * Name: TNode (constructor)
    * File: Tree.c
    * Purpose: Initializes TNode's variables and increments the occupancy for
    * when the first TNode is added as the root in the tree.
    **************************************************************************/
    {
        occupancy++;/*increments the occupancy*/
    }
        

    TNode (const Whatever & element, TNode<Whatever> & parentTNode)
        : balance (0), data (element), height (0), left (0), 
        occupancy (parentTNode.occupancy), right (0), 
        tree_count (parentTNode.tree_count)
    /*************************************************************************
    * Name: TNode (constructor)
    * File: Tree.c
    * Purpose: Initializes TNode's variables and incremenets the occupancy for
    * each node that is created and inserted in the tree after root.
    *****************************************************************8*******/	
    {
	occupancy++;/*increments the occupancy*/
    }


    ~TNode (void) {
    /************************************************************************
    * Name: TNode (destructor)
    * File: Tree.c
    * Purpose: Deallocates memory associated with TNode and decrements
    * occupancy since the TNode is being deleted.
    *************************************************************************/
	occupancy--;/*decrements occupancy*/
    }


    void delete_AllTNodes (void)
    /*************************************************************************
    * Name: delete_AllTNodes 
    * File: Tree.c
    * Purpose: Does a post-order traversal to delete every TNode in the tree
    * by recursively calling itself if there is a left or right child. Once a
    * child-less TNode is reached the TNode destructor is called to delete
    * the current TNode. 
    *************************************************************************/
    {         
        if (this->left){/*if a left node exists*/
	    this->left->delete_AllTNodes();/*call left's delete_AllTNodes
	                                     to shift left*/
	}
	if(this->right){/*if a right node exists*/
	    this->right->delete_AllTNodes();/*call right's delete_AllTNodes
	                                      to shift right*/
	}
	/*Node has no children*/
	delete(this);/*calls destructor for current TNode*/
	
    }

    unsigned long Insert (const Whatever & element, 
                          TNode<Whatever> *& PointerInParent);
    /*OPTIONAL TNode :: Lookup => uncomment if implementing recursively*/
    unsigned long Lookup(Whatever & element) const;


    void ReplaceAndRemoveMax (TNode<Whatever> & targetTNode, 
        TNode<Whatever> *& PointerInParent)
    /**************************************************************************
    * Name: ReplaceAndRemoveMax (TNode)
    * File: Tree.c
    * Purpose: Called when removing a TNode with 2 children. The method moves
    * left and then all the way right by recursively calling itself until it
    * reaches the predecessor node. The TNode with 2 children takes on its
    * predecessor's data and the predecessor is then deleted.
    * Parameter: targetTNode is the TNode with 2 children to be removed. It
    * will take on it's predecessor's data. PointerInParent is the pointer in
    * the parent node that points to its child.
    * @return: No return value.
    ************************************************************************/
    {
        if(Tree<Whatever>::debug){/*if debug messages are on*/
	    cerr<<TREE<<tree_count<<COMPARE<<(const char *)data<<"]"
	    <<'\n';/*prints that the current TNode is being compared*/
	}
	if(this->right){/*if a right child exists*/
	    this->right->ReplaceAndRemoveMax(targetTNode, right);/*recursively
	    call itself to get to the rightmost child*/
	    SetHeightAndBalance(PointerInParent);/*Call SetHeightAndBalance to
	                                           update the nodes after
						   recursion*/
	}
	else{/*current node is the rightmost node*/
	    if(Tree<Whatever>::debug){/*if debug messages are on*/
	        cerr<<TREE<<tree_count<<REPLACE<<(const char *)data<<"]"
		<<'\n';/*prints that the current TNode's data will replace the
		         TNode with 2 children*/
	    }
	    targetTNode.data=data;/*targetTNode takes on the data of its
	                            predecessor*/
	    PointerInParent=left;/*PointerInParent is set to the left*/
	    delete(this);/*the predecessor is deleted*/
	}
    }

    unsigned long Remove (TNode<Whatever> & elementTNode, 
        TNode<Whatever> *& PointerInParent,
        long fromSHB = FALSE)
    /**************************************************************************
    * Name: Remove (TNode)
    * File: Tree.c
    * Purpose: Called from Tree's Remove if the tree is not empty. Removes the
    * matching data that is passed in from the tree. The method is recursive
    * and calls itself each time it shifts the current node. If the matching
    * data is found, the function deletes the node and returns true. 
    * Parameter: elementTNode is the information to be looked up in the tree,
    * PointerInParent is the pointer in the parent node that points to its
    * child.
    * @return: Returns true if successfully removed, false if not. 
    ***************************************************************************/
    {
        long returnVal;/*holds the return value of each Remove*/

        if (Tree<Whatever>::debug){/*if debug messages are on*/
	    cerr<<TREE << tree_count<< COMPARE<<(const char *)elementTNode.data
	    << AND<<(const char *)data<< "]"<< "\n";/*compares element and the
	                                              current node*/
	}
	if (data == elementTNode.data){/*if element is equal to the current
		                         node's data then the node has been
					 found*/
	    elementTNode.data= data;/*set current elementTNode's data as
	                              Tnode's data*/
	    
	    /*TNode has 2 children case*/
	    if(this->left && this->right){/*if the TNode to remove has left
		                            and right children*/
                this->left->ReplaceAndRemoveMax(*this,left);/*calls RARM on the
		                                              left child*/
	    }

	    /*Not 2 children case*/
	    else{/*If the TNode does not have 2 children*/
	        if(this->left){/*if the node has a left child*/
	            PointerInParent= this->left;/*reassign pointer in parent
		                                  to node's left child*/
	        }
	        else if(this->right){/*if the node has a right child*/
		    PointerInParent= this->right;/*reassign pointer in parent
		                                   to node's right child*/
	        }
	        else{/*if the node has no children*/
	            PointerInParent= NULL;/*set the PointerInParent to NULL*/
	        }
	        delete(this);/*deletes the current node*/
	    }
	    if (!fromSHB){/*if Remove was NOT called from SHB*/
		if (PointerInParent){/*if the PointerInParent exists*/
		    SetHeightAndBalance(PointerInParent);/*Call
		                                           SetHeightAndBalance
							   to update
							   PointerInParent*/
		}
	    }
	    return true;/*returns true since node was deleted*/
	}

	else {/*the current node is not the one we are looking for*/  
	    if (elementTNode.data > data){/*if element is greater than the
					    current node*/
	        if(this->right!=NULL){/*if a right node exists*/
	            returnVal=this->right->Remove(elementTNode,right);/*recurses
		                                                        to move
									right*/
		}
	        else{/*the right node does not exist*/
	            return false;/*return false since the node is not there*/
	        }
	    }
	    else{/*the element is less than the current node*/
	        if (this->left!=NULL){/*if a left node exists*/
	            returnVal=this->left->Remove(elementTNode, left);/*recurses
		                                                       to shift
								       left*/
	        }
                else{/*the left node does not exist*/
	 	    return false;/*return false since the node is not there*/
	        }
	    }
	}

	if (returnVal){/*if the return value of Remove is true*/
	    SetHeightAndBalance(PointerInParent);/*Call SetHeightAndBalance
	                                           to update current node's
						   height and balance*/
	    return true;/*returns true for success*/
	}
	else{/*if remove returned false for failure to find the node*/
	    return false;/*returns false for failure*/
	}
    }

    void SetHeightAndBalance (TNode<Whatever> *& PointerInParent)
    /**************************************************************************
    * Name: SetHeightAndBalance)
    * File: Tree.c
    * Purpose: Updates the height and balance of the node that PointerInParent
    * points to. If the subsequent balance is greater than an absolute value of
    * 2, then the node is removed with TNode's remove and re-inserted into the
    * tree with TNode's insert.
    * Parameter: PointerInParent is the pointer in the parent node that points
    * to its child
    * @return: No return value.
    ***************************************************************************/
    {

        if (Tree<Whatever>::debug){/*if debug messages are on*/
	    cerr<<TREE<<tree_count<<UPDATE<<(const char *)data<<"]"<<
	    '\n';/*prints that the PointerInParent's height and balance will be
		   updated*/
	}
	
	if(PointerInParent->left==NULL && PointerInParent->right){/*if there
	                                                            is a right
								    node and no
								    left node*/
            (PointerInParent->height)=(PointerInParent->right->height)+1;
	    /*set height as 1 more than the right's height*/
	    (PointerInParent->balance)= -1-(PointerInParent->right->height);
	    /*set balance as -1  minus the right's height*/
	}   

	/*if there is a left node and no right node*/
	else if(PointerInParent->right==NULL && PointerInParent->left){
	    (PointerInParent->height)=(PointerInParent->left->height)+1;
	    /*set height as 1 more than the left's height*/
	    (PointerInParent->balance)=(PointerInParent->left->height)+1;
	    /*set balance as the left's height minus -1*/
	}
	
	else if (PointerInParent->left && PointerInParent->right){/*if there is
	                                                            both a left
								    and right*/
	    (PointerInParent->balance)=(PointerInParent->left->height)-
	    (PointerInParent->right->height);/*balance is the left's height
		                                   minus right's height*/
	    if((PointerInParent->left->height) > 
	    (PointerInParent->right->height)){/*if left node is taller than
		                                    the right*/
	        PointerInParent->height=(PointerInParent->left->height)+1;
		/*height is 1 more than the left's height*/
	    }
	    else{/*if right node is taller than left*/
		PointerInParent->height=(PointerInParent->right->height)+1;
		/*height is 1 more than right node's height*/
	    }	
        }
	else{/*if PointerInParent has no children*/
	    PointerInParent->height= 0;/*set the height as 0*/
	    PointerInParent->balance= 0;/*set the balance as 0*/
	
	}

	/*Out of balance case*/
	if(abs(PointerInParent->balance) > THRESHOLD){/*if the absolute value
		                                        of balance is more
							than the threshold*/
	    TNode<Whatever> elementTNode(PointerInParent->data, *this);
	    Remove(elementTNode,PointerInParent,true);/*Call Remove to delete
	                                                the current node*/
	    PointerInParent->Insert(elementTNode.data, 
	    PointerInParent);/*Call Insert to reinsert the data of the node*/

	}
    }

    ostream & Write_AllTNodes (ostream & stream) const {
        if (left)
            left->Write_AllTNodes (stream);
        stream << *this;
        if (right)
            right->Write_AllTNodes (stream);
        return stream;
    }
};


/************************************************************************
* Name: Tree (class) 
* File: Tree.c
* Purpose: Contains the class methods for Tree objects. Several methods such
* as lookup, insert, and remove will delegate to TNode's equivalent method.
* Tree's destructor will call the method in TNode to clear the tree upon ^D.
************************************************************************/

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
    debug= true;/*turns debug messages on*/
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
    debug= false;/*turns debug messages off*/
}

template <class Whatever>
unsigned long TNode<Whatever> :: Insert (const Whatever & element, 
                                         TNode<Whatever> *& PointerInParent)
/**************************************************************************
* Name: Insert(TNode)
* File: Tree.c
* Purpose: Called from Tree' Insert when the tree is not empty. In the case of
* a duplicate insert, the TNode's data is replaced with element. To move down
* the tree, the method calls itself recursively until the node to be inserted
* is in the right spot alphabetically. A new node is then created at that
* location with element as its data. SetHeightAndBalance is called after each
* recursive step to update the height and balance.
* Parameter: element is the data to be inserted in the tree, PointerInParent is
the pointer in the parent node pointing to its child 
* @return: Returns true if successfully inserted, false otherwise.
************************************************************************/
{
    if (Tree<Whatever>::debug){/*if debug messages are on*/
	cerr<< TREE<< tree_count<< COMPARE<< (const char *)element<< AND<<
	(const char*)data<< "]"<< "\n";/*comparing element and the current
	                                 node*/
    }

    /*Duplicate case*/
    if(data== element){/*if element equals current node*/
        data=element;/*set data as element*/
	if (Tree<Whatever>::debug){/*if debug messages are on*/
	    cerr<< TREE << tree_count<< INSERT<< (const char *)element<<
	    "]"<< "\n";/*prints that element was inserted*/
	}
	return true;
    }

    /*Insert or move right*/
    if (element > data){/*if element is greater than the current node*/
        if (this->right==NULL){/*if there is nothing to the right*/
	    this->right= new TNode<Whatever>(element, *this);/*create new node
	                                                       there*/
	    if (Tree<Whatever>::debug){/*if debug messages are on*/
	        cerr<< TREE<<tree_count<< INSERT<< (const char *)element<< "]"<<
	        "\n";/*prints that element was inserted*/
	    }
	}
	else{/*a node exists on the right*/
	    this->right->Insert(element, right);/*recursively call Insert to
	                                          move right*/
	}
    }

    /*Insert or move left*/
    else if(data > element){/*if element is smaller than the current node*/
        if (this->left==NULL){/*if there is no node on the left*/
            this->left= new TNode<Whatever>(element, *this);/*create new node
	                                                      there*/
	    if (Tree<Whatever>::debug){/*if debug messages are on*/
	        cerr<<TREE<< tree_count<< INSERT<<(const char *)element<<
		"]"<< "\n";/*prints that element was inserted*/
	    }
        }
        else{/*a node exists on the left*/
            this->left->Insert(element, left);/*recursively call Insert to move
	                                        left*/
        }
    }

    PointerInParent=this;/*set PointerInParent as this current node*/
    SetHeightAndBalance(PointerInParent);/*Call SetHeightAndBalance to update
                                           PointerInParent's height and
					   balance*/
    return true;/*successfully inserted*/
}

template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
        stream << "at height:  :" << nnn.height << " with balance:  "
                << nnn.balance << "  ";
        return stream << nnn.data << "\n";
}

     

template <class Whatever>
unsigned long Tree<Whatever> :: Insert (const Whatever & element)
/**************************************************************************
* Name: Insert (Tree)
* File: Tree.c
* Purpose: If the tree is empty, the method creates a new TNode with element
* as the data to be the tree's root. Calls TNode's insert if the tree is not
* empty.
* Parameter: element is the data to be inserted in the tree. 
* @return: Returns true if sucessfully inserted, false otherwise. 
************************************************************************/
{

    if(root==NULL){/*if the first TNode hasn't been inserted; no root*/
        root= new TNode<Whatever>(element, *this);/*insert new TNode at root*/ 
	if (debug){/*if debug messages are on*/
	    cerr<<TREE<< tree_count<< INSERT<< (const char *)element<< "]"<<
	    "\n";/*prints that element was inserted*/
	}
	return true;/*inserted successfully*/
    }
    else {/*if Tree is not empty, delegates to TNode's Insert*/
        root->Insert(element, root);/*calls root's insert*/
	return true;/*inserted successfully*/
    }
}


template <class Whatever>
unsigned long TNode<Whatever>::Lookup(Whatever & element) const
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
* @return: True if the node was found in the tree, false otherwise. 
************************************************************************/
{
    long returnVal;/*holds the return value of lookup*/
    
    if (Tree<Whatever>::debug){/*if debug messages are on*/
	cerr<< TREE<< tree_count<< COMPARE<< (const char *)element<< AND<<
	(const char*)data<< "]"<< "\n";/*comparing element and the current
	                                 node*/
    }
    /*Found a match*/
    if(data== element){/*if element equals current node*/
        element=data;/*sets element as the current node's data*/
	return true;/*returns true since node was found*/	
    }
    /*Looking right*/
    else if (element > data){/*if element is greater than the
                               current node*/
        if (this->right==NULL){/*if there is nothing to the right*/
	    return false;/*returns false since node is not found*/  
	}
	else{/*a node exists on the right*/
	    returnVal= this->right->Lookup(element);/*recursively call Insert
	                                              to move right. Set
						      returnVal as Lookup's
						      return value*/
	}
    }
    /*Looking left*/
    else{/*if element is smaller than the current node*/
        if (this->left==NULL){/*if there is no node on the left*/
            return false;/*returns false since node was not found*/
        }
        else{/*a node exists on the left*/
            returnVal=this->left->Lookup(element);/*recursively call Insert to
	                                            move left. Set returnVal as
						    Lookup's return value*/ 
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
* Parameter: element is the data we are searching for in the tree 
* @return: True if the node was found in the tree, false otherwise. 
************************************************************************/
{
    if (occupancy){/*if the tree is not empty*/
        return root->Lookup(element);/*delegates to TNode's lookup and returns
	                               its return value*/
    }
    else{/*if the tree is empty*/
	return false;/*return false since the node is not there*/
    }
}



template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element)
/************************************************************************
* Name: Remove (Tree)
* File: Tree.c
* Purpose: Removes an element from the tree. If the tree is not empty, the
* function calls TNode's Remove. If the node is successfully removed,
* the method returns true.
* Parameter: element is the data stored in the TNode.
* @return: Returns true if successfully removed, false if not. 
************************************************************************/

{
    long returnVal;/*return value of TNode's remove*/

    if(occupancy){/*if the tree is not empty*/
        TNode<Whatever> elementTNode(element,*this);/*temporary TNode to store
	                                              element*/
	returnVal= root->Remove(elementTNode, root, false);/*calls on TNode's
	                                                    Remove and passes
							    in elementTNode.
							    Sets returnVal as 
							    the return value*/
	element= elementTNode.data;/*set element as elementTNode's data from
	                             the TNode remove method*/
	if (returnVal){/*if TNode's remove returns true*/
	    return true;/*successfully removed element*/
	}
    }
    return false;/*tree is empty so there is nothing to remove*/

}
                                

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree<Whatever> :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
    if (occupancy){/*if the tree is not empty*/
        root->delete_AllTNodes();/*Calls root's delete_AllTNodes to delete all
	                           nodes in the tree*/
    }
}


template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree<Whatever> :: Write (public)
% File :         Tree.c
% 
% Description : This function will output the contents of the Tree table
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
        stream << "Tree " << tree_count << ":\n"
                << "occupancy is " << occupancy << " elements.\n";

        return (root) ? root->Write_AllTNodes (stream) : stream;
}
