/**************************************************************************** 
 
                                                        Vanessa Chou 
                                                        CSE 12 Winter 
                                                        2/15/2015 
                                                        cs12xkf 
                                Assignment Seven 
 
File Name:      Tree.c 
Description:    The program creates a tree where objects can be inserted as
                nodes and be organized in order. The tree begins with the root
		and sorts by placing smaller items (earlier in the alphabet)
		on the left of nodes and larger items (later in the alphabet)
		on the right of nodes. The occupancy of the tree is the number
		of nodes, which will not decrease when duplicate objects are
		inserted (in these cases, the new node will replace the
		information of the old one). Deleting nodes will decrease the
		occupancy. The height and balance of nodes are updated each
		time a node is inserted. At the end of the program, all the
		memory associated with the tree and nodes is freed.
****************************************************************************/ 
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "Tree.h"
using namespace std;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static const char ALLOCATE[] = " - Allocating]\n";
static const char DEALLOCATE[] = " has been deallocated]\n";
static const char AND[] = " and ";
static const char COMPARE[] = " - Comparing ";
static const char INSERT[] = " - Inserting ";
static const char TREE[] = "[Tree ";

struct TNode {
    Base * data;
    TNode * left, * right, *parent;
    static long occupancy;
    unsigned long hasBeenDeleted;

    // left child's height - right child's height
    long balance;

    // 1 + height of tallest child, or 0 for leaf
    long height;

    TNode (Base * element) : data (element), left (0), right (0),
            parent (0), hasBeenDeleted (FALSE), balance (0), height(0)
    /**************************************************************************
    * Name: TNode(TNode constructor)
    * File: Tree.c 
    * Purpose: Allocates and initializes TNode memory
    * Parameter: Element is base pointer to information
    ***************************************************************************/
    {	
	occupancy++;/*occupancy increments each time a TNode is created*/	
	
    }

    ~TNode (void) 
    /************* *************************************************************
    * Name: ~TNode(TNode destructor) 
    * File: Tree.c 
    * Purpose: Deallocating the TNode
    * Parameter: Void 
    ***************************************************************************/
    {
	delete(parent);/*deallocates memory for parent pointer*/
	delete(left);/*deallocates memory for left pointer*/
        delete(right);/*deallocates memory for right pointer*/
        delete(data);/*deallocates memory for data pointer*/
    }

    ostream & Write (ostream & stream) const {
        stream << "at height:  " << height << "  with balance:  "
	    << balance << "  ";
	return data->Write (stream) << "\n";
    }
    ostream & Write_AllTNodes (ostream & stream) const {
        if (left)
	    left->Write_AllTNodes (stream);
	if (!hasBeenDeleted)
	    Write (stream);
	if (right)
	    right->Write_AllTNodes (stream);
        return stream;
    }

    void delete_AllTNodes (void)
    /**************************************************************************
    * Name: delete_AllTNodes 
    * File: Tree.c 
    * Purpose: Does a post-order traversal of deleting all nodes
    * Parameter: Void
    * @return: Void 
    ***************************************************************************/
    {
	TNode * working=this;/*the current TNode, initialized as root*/
        TNode * delNode;/*node to be deleted*/
        
        while(working){/*loops until root has been deleted*/
	    if (working->left){/*if a left node exists*/
	   	working= working->left;/*shift left*/
	    }
	    else if(working->right){/*if a right node exists*/
		working= working->right;/*shift right*/	
	    }
	    else{/*node has no children*/
		delNode= working;/*set node to be deleted as working*/
		working= working->parent;/*working goes back up one node*/
                if(working!=NULL && working->left){/*if node to be deleted is
			                             left one*/
		    working->left=NULL;/*sets left pointer as NULL*/
		}
                else if(working!=NULL && working->right){/*if node to be
			                                   deleted is right
							   one*/
		    working->right=NULL;/*sets right pointer as NULL*/
		}
		delNode->parent= NULL;/*set the to-be deleted node's parent as
		                        NULL*/
		delete(delNode);/*calls destructor*/
	    }
	}
    }                 
};

// initialization of static data fields of Tree
long TNode :: occupancy = 0;
bool Tree :: debug_on = 0;

void Tree :: Set_Debug (bool option)
/*************************************************************************
* Name: Set_Debug
* File: Tree.c
* Purpose: Turns debug essages on or off
* Parameter: Nonzero option means debug messages are on; option=0 means
* debug messages are off
* @return: None. 
***************************************************************************/
{
    if (option){/*options is nonzero*/
	debug_on=true;/*debug messages are on*/
    }
    else{/*option is 0*/
        debug_on=false;/*debug messages are off*/
    }
}

Tree :: Tree (const char * name) : root (0), tree_name (strdup (name))
/***************************************************************************
% Routine Name : Tree :: Tree  (public)
% File :         Tree.c
% 
% Description :  Initializes root pointer to NULL, and occupancy to 0.
***************************************************************************/

{
    if(debug_on){/*if debug messages are on*/
	cerr<< TREE<< tree_name<< ALLOCATE;/*states which tree is allocated*/
    }
    TNode::occupancy=0;/*initializes occupancy as 0*/
}

Tree :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
    if(root){/*if root exists*/
	root->delete_AllTNodes();/*calls delete_AllTNodes to deallocate all
	                           memory of the nodes*/
    }
    if(debug_on){/*if debug messages are on*/
	cerr<< "["<< tree_name<< DEALLOCATE;/*prints that the tree isi
	                                      deallocated*/
    }
    free((char *)tree_name);/*deallocates memory of the tree*/ 
}	/* end: ~Tree */

unsigned long Tree :: Insert (Base * element) {
/**************************************************************************
* Name: Insert 
* File: Tree.c 
* Purpose: Inserts the element parameter where it belongs on the tree. If
* root does not exist, then element will be the first node. Otherwise, the
* code checks from the top to the bottom of the tree checking where the
* node will be inserted. The occupancy of the tree increases each time a
* node is added. If a node has the same name as the information to be
* inserted, then the new node replaces the old one and occupancy does not
* change. Once the node is inserted, height/balance of affected nodes will
* be adjusted.
* Parameter: element is the information to be inserted in a node on the
*            tree
* @return: 1 if inserted successfully, 0 if unsuccessful 
****************************************************************************/
    TNode * working= root;/*the current TNode, initialized as the root*/
    long update= false;/*if true then height/balance need to be updated*/


    if(root==NULL){/*if the first TNode hasn't been inserted yet*/
        root= new TNode(element);/*pass element as the new Node*/ 
	if (debug_on){/*if debug messages are on*/
	    cerr<<TREE<< tree_name<< INSERT<< (char *)(*element)<< "]"<<
	        "\n";/*prints that element was inserted*/
	}
	return true;/*inserted successfully*/
    }

    /*loops that travels down the tree to find where to insert the new node*/
    while(!update){/*loops until update is true, which means that TNode was
   	             inserted*/  
	if (debug_on){/*if debug messages are on*/
	    cerr<< TREE<< tree_name<< COMPARE<< (char *)(*element)<< AND<<
	        (char *)(*working->data)<< "]"<< "\n";/*comparing element and
		                                        the current node*/	
	}
	/*duplicate case*/
	if(*(working->data)== *element){/*if element equals current node*/
	    delete(working->data);/*free old data*/
	    working->data=element;/*set data as element*/
	    if (debug_on){/*if debug messages are on*/
	        cerr<< TREE << tree_name<< INSERT<< (char *)(*element)<<
		    "]"<< "\n";/*prints that element was inserted*/
	    }	
	    if (!working->hasBeenDeleted){/*replacing an existing node*/
	        return true;/*returns as height/balance don't need to be
	                     updated*/
	    }
	    else{/*reinserting a deleted node*/
	        working->hasBeenDeleted= false;/*set working back to
	                                        undeleted*/
	        TNode::occupancy++;/*increment occupancy*/
		update= true;/*exit loop so height/balance will be adjusted*/
	    }
	}
 
        /*insert or move right*/
	if (*element > *(working->data)){/*if element is greater than the
		                          current node*/
            if (working->right==NULL){/*if there is nothing to the right*/
	        working->right= new TNode(element);/*insert there*/
	        working->right->parent= working;/*working is the right node's
		                                  parent*/
		if (debug_on){/*if debug messages are on*/
	            cerr<< tree_name<< INSERT<< (char *)(*element)<< "]"<<
		        "\n";/*prints that element was inserted*/
	        }    
		working= working->right;/*working is now the right node*/
		update=true;/*set update as true to update height/balance*/
	    }
	    else{/*a node exists on the right*/
	        working->right->parent=working;/*working is the right node's
		                                 parent*/
	        working= working->right;/*working is now the right node*/
	    }
	}
	/*insert or move left*/
	else if(*(working->data) > *element){/*if element is smaller than the
		                            current node*/
            if (working->left==NULL){/*if there is no node on the left*/
	        working->left=new TNode(element);/*insert new node there*/
	        if (debug_on){/*if debug messages are on*/
	            cerr<<TREE<< tree_name<< INSERT<<
		        (char *)(*element)<< "]"<< "\n";/*prints that element
			                                  was inserted*/
	        }
		working->left->parent=working;/*working is left node's parent*/
		working=working->left;/*working is now the left node*/
		update=true;/*set update as true to update height/balance*/
	    }
	    else{/*a node exists on the left*/
	        working->left->parent= working;/*working is the left node's
		                                 parent*/
	        working= working->left;/*shift left*/
	    }
	}    
    }


    /*updates each node's height and balance until working is root*/
    while(update && working->parent!=NULL){/*if height and balance need to
		                                 be updated*/
        working=working->parent;/*working is set as working's parent*/
        
	if(working->left==NULL){/*if there is no left node*/
            (working->height)=(working->right->height)+1;/*the current node's
		                                           height is 1 more than
		    				           the right's height*/
	    (working->balance)= -1-(working->right->height);/*the current node's
		                                              balance is -1
							      minus the right's
							      height*/
	}   
	
	else if(working->right==NULL){/*if there is no right node*/
	    (working->height)=(working->left->height)+1;/*the current node's
	   	                                          height is 1 more than
						          the left's height*/
	    (working->balance)=(working->left->height)+1;/*the current node's
		                                           balance is the left's
						           height minus -1*/
	}
	
	else{/*if there is both a left and right node*/
	    (working->balance)=(working->left->height)-
	        (working->right->height);/*the current node's height is the
		                           left's height minus right's height*/
	    if((working->left->height) > (working->right->height)||
	        working->right==NULL){/*if left node is taller than the right*/
	        (working->height)=(working->left->height)+1;/*the current node's
		                                           height is 1 more
							   than the left*/
	    }
        }	
    }
    return true;/*returns true if the node was inserted successfully*/
}

const Base * Tree :: Lookup (const Base * element) const 
/**************************************************************************
* Name: lookup 
* File: Tree.c
* Purpose: Looks up whether the parameter element exists in a node in the
* tree. The code checks from the top of the tree to the bottom and moves
* according to whether element is greater or less than the current node.
* If a match is found and the node has not been deleted, the data of the
* node is returned. Else, the method returns null since the node has been
* deleted or no match was found.
* Parameter: element is the information to be looked up in the tree
* @return: Returns the data of the node. If a match is not found, then the
*          method returns null. 
***************************************************************************/
{ 
    TNode * working=root;/*the current TNode, initialized as root*/

    while(working!=NULL){/*loops until a match is found or not*/
        if(debug_on){/*if debug messages are on*/
	    cerr<< TREE<< tree_name<< COMPARE<< (char *)(*element)<< AND<<
	        (char *)(*working->data)<< "]"<< "\n";/*compares element and
		                                        the current node*/
	}
	if (*(working->data) == *element){/*if element is equal to the current
		                            node's data then the node has been
					    found*/
	    if (!working->hasBeenDeleted){/*if the node has not been deleted*/
		return working->data;/*return the node's data*/
	    }
	    else{/*if the node was previously deleted*/
	        return NULL;/*return null since the node does not exist*/
	    }
	}
	else {/*the current node is not the one we are looking for*/  
	    if (*element > *working->data){/*if element is greater than the
		                             current node*/
	        if(working->right!=NULL){/*if a right node exists*/
	            working= working->right;/*shift right*/
	        }
	        else{/*the right node does not exist*/
	            return NULL;/*return null since the node is not there*/
	        }
	    }
	    else{/*the element is less than the current node*/
	        if (working->left!=NULL){/*if a left node exists*/
	            working= working->left;/*shift left*/
	        }
                else{/*the left node does not exist*/
	 	    return NULL;/*return null since the node is not there*/
	        }
	    }
	}
    }
    return NULL;/*returns null if there are no nodes in the tree*/
}


Base * Tree :: Remove (const Base * element)
/**************************************************************************
* Name: remove 
* File: Tree.c
* Purpose: Looks up whether the parameter element exists in a node in the
* tree and deletes it. The code checks from the top of the tree to the
* bottom and moves according to whether element is greater or less than
* the current node. If a match is found and the node has not been deleted,
* the node is deleted and the data of the node is returned. Else, the
* method returns null since the node has already been deleted or no match
* was found.
* Parameter: element is the information to be looked up in the tree
* @return: Returns the data of the node. If a match is not found, then the
*          method returns null. 
***************************************************************************/
{  
    TNode * working=root;/*the current TNode, initialized as root*/

    while(working!=NULL){/*loops until a match is found or not*/
        if (debug_on){/*if debug messages are on*/
	    cerr<<TREE <<tree_name<< COMPARE<< (char *)(*element)<< AND<<
	        (char *)(*working->data)<< "]"<< "\n";/*compares element and
		                                        the current node*/
	}
	if (*(working->data) == *element){/*if element is equal to the current
		                            node's data then the node has been
					    found*/
	    if (!working->hasBeenDeleted){/*if the node has not been deleted*/
	        working->hasBeenDeleted= true;/*the node counts as deleted*/
		TNode::occupancy--;/*decrement occupancy*/
		return working->data;/*return the node's data*/
	    }
	    else{/*if the node was previously deleted*/
	        return NULL;/*return null since the node does not exist*/
	    }
	}
	else {/*the current node is not the one we are looking for*/  
	    if (*element > *working->data){/*if element is greater than the
					     current node*/
	        if(working->right!=NULL){/*if a right node exists*/
	            working= working->right;/*shift right*/
	        }
	        else{/*the right node does not exist*/
	            return NULL;/*return null since the node is not there*/
	        }
	    }
	    else{/*the element is less than the current node*/
	        if (working->left!=NULL){/*if a left node exists*/
	            working= working->left;/*shift left*/
	        }
                else{/*the left node does not exist*/
	 	    return NULL;/*return null since the node is not there*/
	        }
	    }
	}
    }
    return NULL;/*returns null if there are no nodes in the tree*/
}


ostream & Tree :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
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
        stream << "Tree " << tree_name << ":\n"
		<< "occupancy is " << TNode :: occupancy << " elements.\n";

	return (root) ? root->Write_AllTNodes (stream) : stream;
}       /* end: Write */


