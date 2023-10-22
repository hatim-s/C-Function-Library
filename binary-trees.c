#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "auxillary-functions.c"
// #include "data-structures.c"

struct TreeNode {
    int data;       // data in the node
    char whatType;  // type of tree - 'B', 'A', 'R'
    union {
        char color; // for Red-Black Trees
        int height; // for AVL Trees
    } indicator;
    struct TreeNode *left, *right, *parent;
};

struct BinaryTree {
    struct TreeNode *root;
};

struct TreeListNode {
    struct TreeNode *node;
    struct TreeListNode *next;
};

struct QueueBinaryTree {
    struct TreeListNode *head, *tail;
};

struct TreeNode* createTreeNode (int x) {
    struct TreeNode *temp = malloc(sizeof(struct TreeNode));
    temp->data = x;
    temp->left = temp->right = temp->parent = NULL;
    return temp;
}

struct TreeListNode* createTreeListNode (struct TreeNode* node) {
    struct TreeListNode *new_node = malloc(sizeof(struct TreeListNode));
    new_node->node = node;
    new_node->next = NULL;

    return new_node;
}

void queueTreeEnqueue(struct QueueBinaryTree *queue, struct TreeNode *x){
	struct SinglyLinkedListNode *X = createTreeListNode(x);
	
	if (queue->tail != NULL) queue->tail->next = X;
	else queue->head = X;

	queue->tail = X;
}

struct TreeNode* queueTreeDequeue(struct QueueBinaryTree *queue){
	if (queue->head == NULL) return INT_MIN;

	struct SinglyLinkedListNode *current = queue->head;
	struct TreeNode* node = queue->head->node;
	if (queue->head == queue->tail) queue->tail = NULL;
	queue->head = (queue->head)->next;
	
	free(current);
	current = NULL;
	return node;
}

bool queueTreeEmpty (struct QueueBinaryTree* queue) {
    return queue->head == NULL;
}

// visiting all nodes in a distance of 'k' from a given node
void visitAllNodesInDistance(struct TreeNode *node, int distance, struct TreeNode *comingFrom)
{
    if (node == NULL) return;
    if (distance == 0){
		// end node code here
        printf ("%d ", node->data);
        return;
    }

    if (node->left != comingFrom)
        visitAllNodesInDistance(node->left, distance - 1, node);
    if (node->right != comingFrom)
        visitAllNodesInDistance(node->right, distance - 1, node);
    if (node->parent != comingFrom)
        visitAllNodesInDistance(node->parent, distance - 1, node);
}

// create tree from parenthesis representation:
struct Token {
    bool isInteger; 
    union Data{
        char character;
        int integer;
    } data;   
};

bool isDigit (char ch) {
    return ch >= '0' && ch <= '9';
}

void tokenize(const char* input, struct Token* tokens, int* numTokens) {
    int len = strlen(input);
    int tokenCount = 0;
    char buffer[10]; // Assuming maximum token length of 100 characters, adjust as needed

    for (int i = 0; i < len; i++) {
        if (input[i] == ' ') continue; 

        // Parentheses are added as character tokens
        if (input[i] == '(' || input[i] == ')') {
            tokens[tokenCount].isInteger = 0;
            tokens[tokenCount].data.character = input[i];
        } 
        else { // input[i] == '-' or input[i] is a number
            int j = 0;
            while (i < len && (isdigit(input[i]) || input[i] == '-')) {
                buffer[j++] = input[i++];
            }
            buffer[j] = '\0';
            i--;

            tokens[tokenCount].isInteger = true;
            tokens[tokenCount].data.integer = atoi(buffer);
        }
        tokenCount++;
    }

    *numTokens = tokenCount;
}

struct BinaryTree createTreeFromBrackets(char* str) {
    int tokenCount;
    struct Token tokens[10010];

    tokenize(str, tokens, &tokenCount);
    printf ("Number of Tokens: %d", tokenCount);

    struct BinaryTree tree;
    tree.root = NULL;
    struct TreeNode* current = NULL;
    struct TreeNode* stack[tokenCount];

    int top = -1;
    for (int i = 0; i < tokenCount; i++) {
        if (!tokens[i].isInteger && tokens[i].data.character == '(') {
            // Do nothing
            continue;
        } 
        else if (!tokens[i].isInteger && tokens[i].data.character == ')') {
            current = stack[top];
            top--;

            if (top >= 0) {
                if (stack[top]->left == NULL) 
                    stack[top]->left = current;
                else 
                    stack[top]->right = current;
            } 
            else tree.root = current;
        } 
        else {
            current = createTreeNode(tokens[i].data.integer);
            if (top >= 0) {
                if (stack[top]->left == NULL) {
                    stack[top]->left = current;
                } else {
                    stack[top]->right = current;
                }
            }
        }
        if (tokens[i].isInteger || tokens[i].data.character == ')') {
            top++;
            stack[top] = current;
        }
    }
    return tree;
}

// find maximum size BST from a tree:
struct nodeData
{
	bool flag;
	int size;
	int minTillNow;
	int maxTillNow;
};

struct nodeData maxBSTFinder(struct TreeNode *node){
	struct nodeData nodeStats;
	if (node == NULL) {
		nodeStats.flag = true;
		nodeStats.size = 0;
		nodeStats.minTillNow = INT_MAX;
		nodeStats.maxTillNow = INT_MIN;

		return nodeStats;
	}
	else if (node->left == NULL && node->right == NULL){
		nodeStats.flag = true;
		nodeStats.size = 1;
		nodeStats.minTillNow = node->data;
		nodeStats.maxTillNow = node->data;

		return nodeStats;
	}

	struct nodeData nodeStatsLeft = maxBSTFinder(node->left);
	struct nodeData nodeStatsRight = maxBSTFinder(node->right);

	if (nodeStatsLeft.flag && nodeStatsRight.flag){
		if ((nodeStatsLeft.maxTillNow < node->data) && (nodeStatsRight.minTillNow > node->data)){
			nodeStats.flag = true;
			nodeStats.size = (nodeStatsLeft.size + nodeStatsRight.size + 1);
			nodeStats.minTillNow = nodeStatsLeft.minTillNow;
			nodeStats.maxTillNow = nodeStatsRight.maxTillNow;

			return nodeStats;
		}
	}

	nodeStats.flag = false;
	nodeStats.size = max(nodeStatsLeft.size, nodeStatsRight.size);
	nodeStats.minTillNow = INT_MIN;
	nodeStats.maxTillNow = INT_MAX;

	return nodeStats;
}

// traversal algorithms: ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void inorderTraversal(struct TreeNode *node)
{
	if (node == NULL) return;
    inorderTraversal(node->right);
    printf("%d ", node->data);
    inorderTraversal(node->left);
}
void preorderTraversal(struct TreeNode *node)
{
	if (node == NULL) return;
    printf("%d ", node->data);
    preorderTraversal(node->left);
    preorderTraversal(node->right);
}
void postorderTraversal(struct TreeNode *node)
{
	if (node == NULL) return;
    postorderTraversal(node->left);
    postorderTraversal(node->right);
    printf("%d ", node->data);
}

void depthFirstTraversal(struct BinaryTree *T)
{
    inorderTraversal(T->root);
}

void breadthFirstTraversal(struct BinaryTree *T)
{
	if (T->root == NULL) return;

    struct QueueBinaryTree Q;
    Q.head = NULL;
    Q.tail = NULL;
    struct TreeNode *current = T->root;
    queueTreeEnqueue(&Q, current);
    while (!queueTreeEmpty(&Q))
    {
        current = queueTreeDequeue(&Q);
        printf("%d ", current->data);
        if (current->left != NULL)
            queueTreeEnqueue(&Q, current->left);
        if (current->right != NULL)
            queueTreeEnqueue(&Q, current->right);
    }
}

void treeClear(struct TreeNode *node)
{
	if (node == NULL) return;
    treeClear(node->left);
    treeClear(node->right);
    free(node);
}

void printParenthesisRepresentation(struct TreeNode *node){
	if (node == NULL) return;

    printf("%d ", node->data);
    printf("( ");
    printParenthesisRepresentation(node->left);
    printf(") ( ");
    printParenthesisRepresentation(node->right);
    printf(") ");
}

// searching algorithms: ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool depthFirstSearch(struct BinaryTree *T, int key){
	return dfs(T->root, key);
}

bool dfs(struct TreeNode *node, int key){
	if (node == NULL) return false;

    if (node->data == key) return true;
    return dfs(node->left, key) || dfs(node->right, key);
}

bool breadthFirstSearch(struct BinaryTree *T, int key)
{
	if (T->root == NULL)
		return false;

	struct QueueBinaryTree Q;
	Q.head = NULL;
	Q.tail = NULL;
	struct TreeNode *current = T->root;

    bool found = false;
	queueTreeEnqueue(&Q, current);
	while (!queueTreeEmpty(&Q))
	{
		current = queueTreeDequeue(&Q);
		if (current->data == key)
            found = true;
		if (current->left != NULL)
			queueTreeEnqueue(&Q, current->left);
		if (current->right != NULL)
			queueTreeEnqueue(&Q, current->right);
	}
	return found;
}