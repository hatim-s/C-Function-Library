// #include "auxillary-functions.c"
#include "binary-trees.c"

// binary search tree: ------------------------------------------------------------------------------------------------------------------------------------

struct TreeNode* createBSTreeNode (int x) {
    struct TreeNode *node = createTreeNode(x);
    node->whatType = 'B';
    return node;
}

struct TreeNode *searchElementTree(struct BinaryTree *T, int key)
{
	struct TreeNode *current = T->root;
	while (current != NULL)
	{
		if (current->data == key)
			return current;
		else if (current->data > key)
			current = current->left;
		else if (current->data < key)
			current = current->right;
	}
	return NULL;
}

struct TreeNode *nodePredecessor(struct TreeNode *node){
    if (node == NULL) return NULL;

	struct TreeNode *current = node;
	if (current->left == NULL)
	{
		while (current->parent != NULL && current->parent->right != current)
			current = current->parent;
		if (current->parent != NULL)
			return current->parent;
		return NULL;
	}

    current = current->left;
    while (current->right != NULL)
        current = current->right;
    return current;
}

struct TreeNode *nodeSuccessor(struct TreeNode *node){
    if (node == NULL) return NULL;

	struct TreeNode *current = node;
	if (current->right == NULL)
	{
		while (current->parent != NULL && current->parent->left != current)
			current = current->parent;
		if (current->parent != NULL)
			return current->parent;
		return NULL;
	}

    current = current->right;
    while (current->left != NULL)
        current = current->left;
    return current;
}

void insertBSTreeNode(struct BinaryTree *T, int key){
	struct TreeNode *node = createBSTreeNode(key);
	struct TreeNode *current = T->root;
	if (current == NULL){ // inserting in an empty tree
		T->root = node;
		return;
	}

	while (current != NULL){
		if (current->data == key) return; // key already exists
		else if (current->data > key){
			if (current->left == NULL){
				node->parent = current;
				current->left = node;
				return;
			}
			else current = current->left;
		}
		else if (current->data < key){
			if (current->right == NULL){
				node->parent = current;
				current->right = node;
				return;
			}
			else current = current->right;
		}
	}
}

void deleteBSTreeNode(struct BinaryTree *T, int key){
	if (T->root == NULL) return;

	struct TreeNode *node = T->root;
    while (true) {
        if (node->data > key) node = node->left;
        else if (node->data < key) node = node->right;
        else { // key node found
            // Case - 1: no children
            if (node->left == NULL && node->right == NULL){ 
                if (node->parent == NULL) T->root = NULL;
                else {
                    if (node->parent->left == node)
                        node->parent->left = NULL;
                    else if (node->parent->right == node)
                        node->parent->right = NULL;
                }
                free(node);
                return;
            }

            // Case 2: left child only
            else if (node->left != NULL && node->right == NULL){
                if (node->parent == NULL){
                    node->left->parent = NULL;
                    T->root = node->left;
                    free(node);
                    return;
                }
                if (node->parent->left == node){
                    node->parent->left = node->left;
                    node->left->parent = node->parent;
                }
                else if (node->parent->right == node){
                    node->parent->right = node->left;
                    node->left->parent = node->parent;
                }
                free(node);
                node = NULL;
                return;
            }

            // Case 3: right child only
            else if (node->left == NULL && node->right != NULL)
            { 
                if (node->parent == NULL){
                    node->right->parent = NULL;
                    T->root = node->right;
                    free(node);
                    return;
                }
                if (node->parent->left == node){
                    node->parent->left = node->right;
                    node->right->parent = node->parent;
                }
                else if (node->parent->right == node){
                    node->parent->right = node->right;
                    node->right->parent = node->parent;
                }
                free(node);
                return;
            }

            // Case 4: Both children
            else{ 
                struct TreeNode *temp = node;
                temp = node->left;
                while (temp->right != NULL)
                    temp = temp->right;
                node->data = temp->data;
                key = temp->data;
                node = temp;
            }
        }
    }
}

// AVL Tree : -------------------------------------------------------------------------------------------------------------------------------------------------

struct TreeNode* createAVLTreeNode(int data)
{
	struct TreeNode *node = createTreeNode(data);
	node->whatType = 'A';
    node->indicator.height = 0;
    return node;
}

int getHeight(struct TreeNode *node) { 
    return node == NULL ? 0 : node->indicator.height;
}
int balanceFactor(struct TreeNode *node) { 
    return absolute(getHeight(node->left) - getHeight(node->right)); 
}

struct TreeNode *rightRotationAVLTree(struct TreeNode *node)
{
	struct TreeNode *X = node->left;
	struct TreeNode *Y = node;
	struct TreeNode *T2 = node->left->right;

	X->right = Y;
	Y->left = T2;
	Y->indicator.height = 1 + max(getHeight(Y->left), getHeight(Y->right));
	X->indicator.height = 1 + max(getHeight(X->left), getHeight(X->right));

	return X;
}

struct TreeNode *leftRotationAVLTree(struct TreeNode *node)
{
	struct TreeNode *X = node;
	struct TreeNode *Y = node->right;
	struct TreeNode *T2 = node->right->left;

	Y->left = X;
	X->right = T2;
	X->indicator.height = 1 + max(getHeight(X->left), getHeight(X->right));
	Y->indicator.height = 1 + max(getHeight(Y->left), getHeight(Y->right));

	return Y;
}

struct TreeNode *insertAVLTreeNode(struct TreeNode *node, int key)
{
	if (node == NULL)
	{
		node = createAVLTreeNode(key);
		node->indicator.height = 1;
		return node;
	}
	else
	{
		if (key == node->data)
			return node;
		else if (key < node->data)
			node->left = insertAVLTreeNode(node->left, key);
		else
			node->right = insertAVLTreeNode(node->right, key);
		node->indicator.height = 1 + max(getHeight(node->left), getHeight(node->right));
		if (balanceFactor(node) > 1)
		{
			if (key < node->data)
			{
				if (key < node->left->data)
					node = rightRotationAVLTree(node); // LL
				else
				{
					node->left = leftRotationAVLTree(node->left);
					node = rightRotationAVLTree(node);
				} // LR
			}
			else
			{
				if (key < node->right->data)
				{
					node->right = rightRotationAVLTree(node->right);
					node = leftRotationAVLTree(node);
				} // RL
				else
					node = leftRotationAVLTree(node); // RR
			}
		}
		return node;
	}
}

struct TreeNode *deleteAVLTreeNode(struct TreeNode *node, int key)
{
	if (node == NULL)
		return NULL;
	else
	{
		if (key < node->data)
			node->left = deleteAVLTreeNode(node->left, key);
		else if (key > node->data)
			node->right = deleteAVLTreeNode(node->right, key);
		else
		{
			if (node->left == NULL && node->right == NULL)
			{
				free(node);
				node = NULL;
				return node;
			}
			else if (node->left == NULL && node->right != NULL)
			{
				struct TreeNode *current = node;
				node = node->right;
				free(current);
				return node;
			}
			else if (node->left != NULL && node->right == NULL)
			{
				struct TreeNode *current = node;
				node = node->left;
				free(current);
				return node;
			}
			else
			{
				struct TreeNode *current = node->left;
				while (current->right != NULL)
					current = current->right;
				node->data = current->data;
				node->left = deleteAVLTreeNode(node->left, current->data);
			}
		}
		node->indicator.height = 1 + max(getHeight(node->left), getHeight(node->right));
		if (balanceFactor(node) > 1)
		{
			if (key < node->data)
			{
				if (key < node->left->data)
					node = rightRotationAVLTree(node); // LL
				else
				{
					node->left = leftRotationAVLTree(node->left);
					node = rightRotationAVLTree(node);
				} // LR
			}
			else
			{
				if (key < node->right->data)
				{
					node->right = rightRotationAVLTree(node->right);
					node = leftRotationAVLTree(node);
				} // RL
				else
					node = leftRotationAVLTree(node); // RR
			}
		}
		return node;
	}
}

// Red-Black Tree:

struct TreeNode *createRBTreeNode(int data)
{
	struct TreeNode *node = createTreeNode(data);
    node->whatType = 'R';
    node->indicator.color = 'R';
	return node;
}

void flipColor(struct TreeNode *node)
{
	node->indicator.color = node->indicator.color == 'R' ? 'B' : 'R';
}

struct TreeNode *nodeSibling(struct BinaryTree *T, struct TreeNode *node)
{
	if (node == NULL || node->parent == NULL)
		return NULL;

    if (node->data < node->parent->data) return node->parent->right;
    else return node->parent->left;
}

struct TreeNode *rightRotationRBTree(struct TreeNode *node)
{
	char child = node->parent == NULL ? 'X' : 
                    node->parent->left == node ? 'L' : 'R';
	
	struct TreeNode *P = node->parent;
	struct TreeNode *X = node->left;
	struct TreeNode *Y = node;
	struct TreeNode *T2 = node->left->right;

	X->right = Y;
	Y->left = T2;

	if (child == 'R') P->right = X;
	else if (child == 'L') P->left = X;

	if (T2 != NULL) T2->parent = Y;
	Y->parent = X, X->parent = P;
	return X;
}

struct TreeNode *leftRotationRBTree(struct TreeNode *node)
{
	char child = node->parent == NULL ? 'X' : 
                    node->parent->left == node ? 'L' : 'R';

	struct TreeNode *P = node->parent;
	struct TreeNode *X = node;
	struct TreeNode *Y = node->right;
	struct TreeNode *T2 = node->right->left;

	Y->left = X;
	X->right = T2;

	if (child == 'R') P->right = Y;
	else if (child == 'L') P->left = Y;

	if (T2 != NULL) T2->parent = X;
	X->parent = Y, Y->parent = P;
	return Y;
}

void insertRBTreeNode(struct BinaryTree *T, int key)
{
	struct TreeNode *node = createRBTreeNode(key);
	struct TreeNode *current = T->root;
	if (current == NULL)
	{
		T->root = node;
		node->indicator.color = 'B';
		return;
	}
	while (current != NULL) {
		if (current->data == key) return;
		else if (current->data > key){
			if (current->left == NULL){
				node->parent = current;
				current->left = node;
				break;
			}
			else current = current->left;
		}
		else if (current->data < key) {
			if (current->right == NULL) {
				node->parent = current;
				current->right = node;
				break;
			}
			else current = current->right;
		}
	}
	current = node;

	while (current != NULL) {
		// Tree was empty
        if (current->parent == NULL) {
			current->indicator.color = 'B';
			break;
		} 

        //  RED node inserted, but parent is BLACK
        else if (current->indicator.color == 'R' && 
                    current->parent->indicator.color == 'B')
            break;
		
        // Red-Red link formed
        struct TreeNode *uncle = nodeSibling(T, current->parent);

        if (uncle == NULL || uncle->indicator.color == 'B')
        {   // uncle is NULL or BLACK
            if (current->data < current->parent->parent->data && current->data < current->parent->data)
            { // LL
                flipColor(current->parent);
                flipColor(current->parent->parent);
                current = current->parent->parent;
                current = rightRotationRBTree(current);
            }
            else if (current->data < current->parent->parent->data && current->data > current->parent->data)
            { // LR
                current = current->parent;
                current = leftRotationRBTree(current);
                flipColor(current);
                flipColor(current->parent);
                current = current->parent;
                current = rightRotationRBTree(current);
            }
            else if (current->data > current->parent->parent->data && current->data < current->parent->data)
            { // RL
                current = current->parent;
                current = rightRotationRBTree(current);
                flipColor(current);
                flipColor(current->parent);
                current = current->parent;
                current = leftRotationRBTree(current);
            }
            else
            { // RR
                flipColor(current->parent);
                flipColor(current->parent->parent);
                current = current->parent->parent;
                current = leftRotationRBTree(current);
            }
            if (current->parent == NULL)
                T->root = current;
        }
        else
        {   // uncle exists and is RED
            flipColor(current->parent);
            flipColor(current->parent->parent);
            flipColor(uncle);
            current = current->parent->parent;
        }
	}
}