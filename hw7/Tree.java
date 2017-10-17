/**************************************************************************** 
 
                                                       Vanessa Chou 
                                                        CSE 12 Winter 
                                                        2/15/2015 
                                                        cs12xkf 
                                Assignment Seven 
 
File Name:      Tree.java 
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
/*
 * Name: Tree(class) 
 * Purpose: Includes all the methods and variables associated with Tree object.
 * 
 */
public class Tree<Whatever extends Base> extends Base {

    /* data fields */
    private TNode root;
    private long occupancy; 
    private String treeName;

    /* debug flag */
    private static boolean debug;

    /* debug messages */
    private static final String ALLOCATE = " - Allocating]\n";
    private static final String AND = " and ";
    private static final String COMPARE = " - Comparing ";
    private static final String INSERT = " - Inserting ";
    private static final String TREE = "[Tree ";

    /*
    * Name: Tree (Constructor)
    * Purpose: Stores the name and initializes variables related to Tree object.
    */
    public Tree (String name) {
	treeName = name;//stores the name parameter 
	occupancy= 0;//initializes occupancy to 0
        root = null;//initializes root node pointer to null
        if (debug){//if debug messages are on
		System.err.print(TREE + treeName + ALLOCATE);//prints that the tree
	                                             //was allocated
        }
    }

    /**
    * Name: debugOff
    * Purpose: Sets debug as false so messages do not appear
    * Return None
    */
    public static void debugOff () {
        debug = false;//debug messages are off
    }

    /**
    * Name: debugOn
    * Purpose: Sets debug as true so messages appear
    * Return: None
    */
    public static void debugOn () {
        debug= true;//debug messages are on
    }

    /**
    * Returns the tree's name
    * @return name of the tree
    */
    public String getName() {
        return treeName;
    }


    /**
    * Name: insert 
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
    * @return: True if inserted successfully, false if unsuccessful 
    */
    public boolean insert (Whatever element) {
        TNode working= root;//the current TNode, initialized as the root 
        TNode newDup;//temporary node for duplicate case
	boolean update= false;//if true then height/balance need to be updated
	
	//setting up the root
	if(root==null){//if the first TNode hasn't been inserted yet
            root= new TNode(element);//pass element as the new Node 
	    if (debug){//if debug messages are on
		System.err.println(TREE + treeName + INSERT + element.getName()
		    + "]");//prints that element was inserted
	    }
	    return true;//inserted successfully
	}

        //loop for inserting the new node in the correct 
	while(!update){//loops until update is true, which means that TNode was
		       //inserted   
	    if (debug){//if debug messages are on
	        System.err.println(TREE + treeName + COMPARE +
		    element.getName() +AND+ working.data.getName() +
		    "]");//compares element and current node
	    }
	    //duplicate case
	    if(working.data.equals(element)){//if element equals current node
	        newDup= new TNode(element);//new node to replace old one
		newDup.parent=working.parent;//new node has old node's parent
		newDup.left= working.left;//new node has old node's left node
		newDup.right= working.right;//new node has old node's right node
		newDup.height= working.height;//new node has old node's height
		newDup.balance= working.balance;//new node has old node's
		                                //balance
		newDup.hasBeenDeleted= working.hasBeenDeleted;//new node takes
		                                              //whether the
							      //current was
							      //deleted or not
		working.data= newDup.data;//working has the new node's data
		if (debug){//if debug messages are on
		    System.err.println(TREE + treeName + INSERT + 
		        element.getName()+ "]");//prints that element was
			                        //inserted
	        }	
		if (!working.hasBeenDeleted){//replacing an existing node
		    occupancy--;//decrement occupancy since a new Node was not
		                  //added, just updated
		    working= newDup;//set working as the node
		    return true;//returns as height/balance don't need to be
		                //updated
		}
		else{//reinserting a deleted node
		    working.hasBeenDeleted= false;//set working back to
		                                  //undeleted
		    working=newDup;//set working as the node 
		    update= true;//exit loop so height/balance will be adjusted
		}
	    }
            
	    //insert or move right
	    if (element.isGreaterThan(working.data)){//if element is greater
		                                     //than the current node
		if (working.right==null){//if there is nothing to the right
		    working.right= new TNode(element);//insert there
	            working.right.parent= working;//working is new node's parent
		    if (debug){//if debug messages are on
	                System.err.println(TREE + treeName + INSERT +
			    element.getName()+ "]");//prints that element was
			                            //inserted
	            }    
		    working= working.right;//set the right Node as working
		    update=true;//break out of loop and adjust height/balance
		}
		else{//a right node already exists
		    working.right.parent=working;//working is right node's
		                                 //parent
		    working= working.right;//shift right
		}
	    }
	    //insert or move left
	    else if(working.data.isGreaterThan(element)){//element is less than the current node
                if (working.left==null){//if there is nothing to the left
		    working.left=new TNode(element);//insert there
	            if (debug){//if debug messages are on
	                System.err.println(TREE + treeName + INSERT +
		            element.getName()+ "]");//prints that element was
			                            //inserted
	            }
		    working.left.parent=working;//working is new node's parent
		    working=working.left;//set the left Node as working
		    update=true;//break out of loop and adjust height/balance
		}
		else{//a left node already exists
		    working.left.parent= working;//working is left node's
		                                  //parent
		    working= working.left;//shift left
		}
	    }    
	}

	//updates each node's height and balance until working is root
	while(update && working.parent!=null){//if height and balance need to
		                              //be updated
	    working=working.parent;//working is set as working's parent   
	    if(working.left==null){//if there is no left node
	        working.height=working.right.height+1;//the current node's
		                                      //height is 1 more than
						      //the right's height
	        working.balance= -1-working.right.height;//the current node's
		                                         //balance is -1 minus
							 //the right's height
	    }   
	    else if(working.right==null){//if there is no right node
	        working.height=working.left.height+1;//the current node's
		                                     //height is 1 more than
						     //the left's height
	        working.balance=working.left.height+1;//the current node's
		                                      //balance is the left's
						      //height subtracted by -1
	    }
	    else{//if there is both a left and right node
		working.balance= working.left.height-working.right.height;//the
		                      //current node's height is the left's
				      //height minus the right's height
		if(working.left.height > working.right.height||
		    working.right==null){//if the left node is taller than the
		                         //right
	            working.height=working.left.height+1;//the current node's
		                                         //height is 1 more
							 //than the left
	        }
	        else{//the right node is taller than the left
	            working.height=working.right.height+1;//the current node's
		                                          //height is 1 more
							  //than the right
	        }
            }	
	}
	return true;//returns true if the node was inserted successfully 
    }


    /**
    * Name: lookup 
    * Purpose: Looks up whether the parameter element exists in a node in the
    * tree. The code checks from the top of the tree to the bottom and moves
    * according to whether element is greater or less than the current node.
    * If a match is found and the node has not been deleted, the data of the
    * node is returned. Else, the method returns null since the node has been
    * deleted or no match was found.
    * Parameter: element is the information to be looked up in the tree
    * @return: Returns the data of the node. If a match is not found, then the
    *          method returns null. 
    */
    public Whatever lookup (Whatever element) {
        TNode working=root;//the current TNode, initialized as the root
	    
	      
	while(working!=null){//loops until a match is found or not 	    
	    if(debug){//if debug messages are on
		System.out.println(TREE + treeName + COMPARE + element.getName() +
	            AND + working.data.getName() + "]");//compares element and the
		                                        //current node
	    }
	    
	    if (working.data.equals(element)){//if element is equal to the
		                              //current node's data then the
					      //node has been found
		if (!working.hasBeenDeleted){//if the node has not been deleted
		    return working.data;//return the node's data
		}
		else{//if the node was previously deleted
		    return null;//return null since the node does not exist
		                //anymore
		}
	    }
	    else {//the current node is not the one we are looking for  
	        if (element.isGreaterThan(working.data)){//if element is
		                                         //greater than the
							 //current node
		    if(working.right!=null){//if a right node exists
		        working= working.right;//shift right
		    }
		    else{//if there is no node on the right
		        return null;//return null since the node is not there
		    }
		}
		else{//if the element is less than the current node
		    if (working.left!=null){//if a left node exists
		        working= working.left;//shift left
		    }
                    else{//if there is no node on the left
			return null;//return null since the node is not there
		    }
		}
	    }
	}
	return null;//returns null if there are no nodes in the tree
    }
  

    /**
    * Name: remove 
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
    */
    public Whatever remove (Whatever element) {
        TNode working=root;//the current TNode, initialized as root


	while(working!=null){//loops until a match is found or not
	    if(debug){//if debug messages are on
		System.out.println(TREE + treeName + COMPARE + element.getName() +
	            AND + working.data.getName() + "]");//compares element and the
		                                        //current node
	    }
	    if (working.data.equals(element)){//if element is equal to the
		                              //current node's data then the
					      //node has been found
		if (!working.hasBeenDeleted){//if the node has not been deleted
		    working.hasBeenDeleted= true;//the node counts as deleted
		    occupancy--;//decrement occupancy
		    return working.data;//return the node's data
		}
		else{//if the node was previously deleted
		    return null;//return null since the node does not exist
		}
	    }
	    else {//the current node is not the one we are looking for  
	        if (element.isGreaterThan(working.data)){//if element is
			                                 //greater than the
							 //current node
		    if(working.right!=null){//if a right node exists
		        working= working.right;//shift right
		    }
		    else{//the right node does not exist
		        return null;//return null since the node is not there
		    }
		}
		else{//the element is less than the current node
		    if (working.left!=null){//if a left node exists
		        working= working.left;//shift left
		    }
                    else{//the left node does not exist
			return null;//return null since the node is not there
		    }
		}
	    }
	}
        return null;//returns null if there are no nodes in the tree
    }

    /**
    * Creates a string representation of this tree. This method first
    * adds the general information of this tree, then calls the
    * recursive TNode function to add all nodes to the return string 
    *
    * @return  String representation of this tree 
    */
    public String toString () {
        String string = "Tree " + treeName + ":\noccupancy is ";
        string += occupancy + " elements.";

        if(root != null)
            string += root.writeAllTNodes();

        return string;
    }
   /*
    * Name: TNode(class)
    * Purpose: Includes all the methods and variables associated with TNode
    * objects.
    */
    private class TNode {

        public Whatever data;
        public TNode left, right, parent;
        public boolean hasBeenDeleted;

        /* left child's height - right child's height */
        public long balance;
        /* 1 + height of tallest child, or 0 for leaf */
        public long height;
        
	/*
	* Name: TNode (Constructor)
	* Purpose: Creates the TNode and initializes variables associated
	* with it.
	*/
        public TNode (Whatever element) {
            data= element;//stores element parameter in data
            left=null;//initializes pointer to null
	    right=null;//initializes pointer to null
	    parent=null;//initializes pointer to null
	    balance=0;//initializes balance to 0
	    height=0;//initializes height to 0
	    Tree:occupancy++;//increment occupancy
        }

        /**
        * Creates a string representation of this node. Information
        * to be printed includes this node's height, its balance,
        * and the data its storing.
        *
        * @return  String representation of this node 
        */

        public String toString () {
            return "at height:  " + height + "  with balance: " +
                balance + "  " + data;
        }

        /**
        * Writes all TNodes to the String representation field. 
        * This recursive method performs an in-order
        * traversal of the entire tree to print all nodes in
        * sorted order, as determined by the keys stored in each
        * node. To print itself, the current node will append to
        * tree's String field.
        */
        public String writeAllTNodes () {
            String string = "";
            if (left != null)
                string += left.writeAllTNodes ();
            if (!hasBeenDeleted) 
                string += "\n" + this;          
            if (right != null)
                string += right.writeAllTNodes ();

            return string;
        }
    }
}
