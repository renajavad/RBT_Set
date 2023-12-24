#include <iostream>
#include <vector>

class RBTree
{
	public:
	class Node
	{
		public:
		bool isRed;
		int val;
		Node* left;
		Node* right;
		Node* parent;
		Node():val(0), isRed(true), left(NULL), right(NULL) {}
		Node(int v):val(v), isRed(true), left(NULL), right(NULL) {}
	};
	Node* root;
	std::vector<Node*> arr;
	RBTree(): root(NULL) {}
	RBTree(int v): root(new Node(v)) {}
	~RBTree()
	{
		if( root != NULL )
		{
			destruct(root);
			delete root;
		}
		arr.clear();
	}
		void destruct( Node* node )
		{
			if( node -> left != NULL )
			{
				destruct( node -> left );
				delete node -> left;
			}
			if( node -> right != NULL )
			{
				destruct( node -> right );
				delete node -> right;
			}
		}
		bool isRed( Node* node )
		{
			if( node == NULL )
			{
				return false;
			}
			else
			{
				return node -> isRed;
			}
		}
		void rotateRight( Node* &node )
		{
			Node* temp = node -> left;
			node -> left = temp -> right;
			temp -> right = node;
			temp -> isRed = node -> isRed;
			node -> isRed = true;
			node = temp;
		}
		void rotateLeft( Node* &node )
		{
			Node *temp = node -> right;
			node -> right = temp -> left;
			temp -> left = node;
			temp -> isRed = node -> isRed;
			node -> isRed = true;
			node = temp;
		}
		void flipColor( Node* &node )
		{
			node -> isRed = true;
			node -> left -> isRed = false;
			node -> right -> isRed = false;
		}
		void buildTree()
		{
			int tempArr[10] = { 3, 4, 7, 1, 2, 9, 8, 6, 5, 10 };
			for( int i = 0; i != 10; ++i )
			{
				arr.push_back( new Node ( tempArr[i] ) );
				insert( root, arr[i] );
			}
			display(root);
		}
		void insert( Node* &node, Node* &vNode)
		{
			if( node == NULL )
			{
				node = vNode;
			}
			if( node -> val > vNode -> val )
			{
				insert( node -> left, vNode );
			}
			else if( node -> val < vNode -> val )
			{
				insert( node -> right, vNode );
			}
			if( isRed( node -> right ) && !isRed( node -> left ) )
			{
				rotateLeft(node);
			}
			if( isRed( node -> left ) && isRed( node -> left -> left ) )
			{
				rotateRight(node);
			}
			if( isRed( node -> left ) && isRed( node -> right ) )
			{
				flipColor(node);
			}
		}
		void display( Node* node )
		{
			if( node == NULL )
			{
				return;
			}
			display( node -> left );
			std::cout << node -> val << " ";
			display( node -> right );
		}
		
		Node* parent(Node* root, Node* n)
		{
			if (root == NULL || root == n) 
			{
				return NULL;
			}

			if (root -> left == n || root -> right == n)
			{
				return root;
			}
			Node* leftParent = parent(root -> left, n);
			if (leftParent != NULL) 
			{
				return leftParent;
			}
			return parent(root -> right, n);
      }
    
    Node* parent(Node* n)
    {
		if (n != nullptr && n != root) 
        {
            return parent(root, n);
        }
        else 
        {
            return NULL;
        }
    }
    Node* sibling(Node* n) 
    {
        Node* p = parent(n);
        if (n == p -> left) 
        {
            return p -> right;
        }
        else 
        {
            return p -> left;
        }
    }
};

class Set: public RBTree
{
	private:
	RBTree mySet;
	public:
	Set(int x, int* tempArr) : RBTree(x) 
	{
		mySet.root = new Node(x);
		for( int n = 0; n != sizeof(tempArr); ++n)
		{
				mySet.arr.push_back( new Node (tempArr[n]) );
				mySet.insert( mySet.root, mySet.arr[n] );
			}
	}
	 bool contains(int x) 
    { 
        if (x == mySet.root -> val) 
        { 
            return true; 
        } 
        for (const auto& m : mySet.arr) 
        { 
            if (m -> val == x) 
            { 
                return true; 
            } 
        } 
        return false; 
    } 
	void insertSet(int x)
	{
        if (!contains(x)) 
        { 
            mySet.arr.push_back(new Node(x)); 
            mySet.insert(mySet.root, mySet.arr.back()); 
        } 
        else 
        { 
            std::cout << x << " already  exists" << "\n"; 
        } 
    } 
    void displaySet() 
    { 
        if (mySet.root == NULL) 
        { 
            std::cout << "array is empty..." << "\n"; 
            return; 
        } 
 
        RBTree::display(mySet.root); 
    }
	void deleteCase1(Node* n) 
	{
		if(n != root)
		{
			deleteCase2(n);
		}
	}
	void deleteCase2(Node* n)
	{
		Node* s = sibling(n);
		Node* p = parent(n);
		if(s -> isRed == true)
		{
			p -> isRed = true;
			s -> isRed = false;
			if(n == p -> left)
				rotateLeft(p);
			else 
				rotateRight(p);
		}
	}
	void deleteCase3(Node* n)
	{
		Node* s = sibling(n);
		Node* p = parent(n);
		if(p -> isRed == false && s -> isRed == false && s -> left -> isRed == false && s -> right -> isRed == false)
		{
		s -> isRed = true;
		deleteCase1(p);
		}
		else
			deleteCase4(n);
	}
	void deleteCase4(Node* n)
	{
		Node* s = sibling(n);
		Node* p = parent(n);
		if(p -> isRed == true && s -> isRed == false && s -> left -> isRed == false && s -> right -> isRed == false)
		{
		 s -> isRed = true;
		 p -> isRed = false;
		}
		else 
			deleteCase5(n);
	}
	void deleteCase5(Node* n)
	{
		Node* s = sibling(n);
		Node* p = parent(n);
		if(s -> isRed == false)
		{
			if(n == p -> left && s -> right -> isRed == false && s -> left -> isRed == true)
			{
				s -> isRed = true;
				s -> left -> isRed = false;
				rotateRight(s);
			}
			else if(n == p -> right && s -> left -> isRed == false && s -> right -> isRed == true)
			{
				s -> isRed == true;
				s -> right -> isRed = false;
				rotateLeft(s);
			
			}
		}
		deleteCase6(n);
	}
	void deleteCase6(Node* n)
	{
		Node* s = sibling(n);
        Node* p = parent(n);
		s -> isRed = p -> isRed;
		p -> isRed = false;
		if(n == p -> left)
		{
			s -> right -> isRed = false;
			rotateLeft(p);
		}
		else
		{
			s -> left -> isRed = false;
			rotateRight(p);
		}
	}
	Node* sibling(Node *n) 
	{
		Node* p = parent(n);
		if(n == p -> left)
			return p -> right;
		else 
			return p -> left;
	}
	
};

	int main()
	{
		/*RBTree rbtree;
		rbtree.buildTree();*/
	int array[] = {3, 4, 7, 1, 2, 9, 8, 6, 10}; 
    Set set(13, array); 
    
    set.insertSet(7); 
    set.insertSet(10); 
    set.insertSet(0); 
    set.insertSet(12); 
    set.insertSet(11); 
    if(set.contains(5))
    std::cout << "Set contains 5" << "\n";
    else
    std::cout << "Set does not contain 5" << "\n";
    if(set.contains(10))
    std::cout << "Set contains 10" << "\n";
    else
    std::cout << "Set does not contain 10" << "\n";
    set.displaySet();  
	return 0;
	}
	
