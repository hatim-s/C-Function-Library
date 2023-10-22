#include "auxillary-functions.c"

// binary search:
int binary_search(int arr[], int lo, int hi, int key){
	int mid;
	while (lo <= hi) {
		mid = (lo+hi) / 2;
		if (arr[mid] == key) return mid;
		else if (arr[mid] < key) lo = mid+1;
		else if (arr[mid] > key) hi = mid-1;
	}
	return -1;
}

// sorting algorithms: ----------------------------------------------------------------------------------------------------------------------------------
// insertion sort:
void insertion_sort(int arr[], int n){
	for (int flag = 1; flag < n; flag++){
		int marker = flag;
		while (arr[marker] < arr[marker - 1] && marker > 0){
			swap(&arr[marker], &arr[marker - 1]);
			marker--;
		}
	}
}

// merge sort:
void merge(int arr[], int l, int m, int r){
	int marker1 = l, marker2 = m + 1;
	int combSize = r - l + 1;
	int flag = 0, temp[combSize];

	while (marker1 <= m && marker2 <= r){
		if (arr[marker1] < arr[marker2])
			temp[flag] = arr[marker1++];
		else
			temp[flag] = arr[marker2++];
		flag++;
	}

	while (marker1 <= m)
		temp[flag++] = arr[marker1++];

	while (marker2 <= r)
		temp[flag++] = arr[marker2++];
	
	for (int i = 0; i < combSize; i++)
		arr[l + i] = temp[i];
}
void merge_sort(int arr[], int l, int r){
	if (l < r) {
		int mid = l + (r - l) / 2;
		merge_sort(arr, l, mid);
		merge_sort(arr, mid + 1, r);
		merge(arr, l, mid, r);
	}
}

// quicksort sort:
int partition(int arr[], int p, int r){
	int pivot = arr[r];
	int lo = p, hi = p;
	while (hi <= r) {
		if (arr[hi] <= pivot)
			swap(&arr[hi], &arr[lo++]);
		hi++;
	}
	return (lo-1);
}
void quick_sort(int arr[], int p, int r){
	if (p < r) {
		int q = partition(arr, p, r);
		quick_sort(arr, p, q - 1);
		quick_sort(arr, q + 1, r);
	}
}

// heap sort: (use array indexing from 1 to n)      {most of the times n passed denotes index}
#define left(i) (2*i)
#define right(i) (2*i + 1)
#define parent(i) (i/2)

struct Heap {
	int N, arr[10010];
};

void adjust_up(struct Heap *heap, int i) {
    while (i > 1 && heap->arr[i] > heap->arr[parent(i)]) {
        swap(&heap->arr[i], &heap->arr[parent(i)]);
        i = parent(i);
    }
}

void adjust_down(struct Heap *heap, int i) {
    int maxChild;
    while (left(i) <= heap->N) {
        maxChild = left(i);
        if (right(i) <= heap->N && heap->arr[right(i)] > heap->arr[left(i)]) {
            maxChild = right(i);
        }
        if (heap->arr[i] >= heap->arr[maxChild]) {
            break;
        }
        swap(&heap->arr[i], &heap->arr[maxChild]);
        i = maxChild;
    }
}

void insert(struct Heap *heap, int x) {
    heap->N = heap->N + 1;
    heap->arr[heap->N] = x;
    adjust_up(heap, heap->N);
}

int pop(struct Heap *heap) {
    if (heap->N <= 0) // Return a sentinel value for an empty heap.
        return INT_MIN; 

    int maxVal = heap->arr[1];
    swap(&heap->arr[1], &heap->arr[heap->N]);
    
	(heap->N)--;
    adjust_down(heap, heap->N);
    return maxVal;
}

void max_heapify(struct Heap* heap) {
    for (int i = heap->N / 2; i >= 1; i--) 
        adjust_down(heap, heap->N);
}

void heap_sort(struct Heap *heap) {
    max_heapify(heap);
    for (int len = heap->N; len > 1; len--) {
        swap(&heap->arr[1], &heap->arr[len]);
        adjust_down(heap, len-1);
    }
}


// singly linked list: ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct SinglyLinkedListNode
{
	int data;
	struct SinglyLinkedListNode *next;
};
struct SinglyLinkedList
{
	struct SinglyLinkedListNode *head;
};

// auxillary functions:
struct SinglyLinkedListNode *createNodeSll(int x){
	struct SinglyLinkedListNode *node = malloc(sizeof(struct SinglyLinkedListNode));
	if (node == NULL)
		exit(0);
	node->data = x;
	node->next = NULL;
	return node;
}

struct SinglyLinkedListNode *searchElementSll(struct SinglyLinkedList *SLL, int x){
	if (SLL->head != NULL){
		struct SinglyLinkedListNode *current = SLL->head;
		while (current != NULL){
			if (current->data == x)
				return current;
			current = current->next;
		}
	}
	return NULL;
}

void printLinkedListSll(struct SinglyLinkedList *SLL){
	if (SLL->head != NULL){
		struct SinglyLinkedListNode *current = SLL->head;
		while (current != NULL){
			printf("%d ", current->data);
			current = current->next;
		}
		printf("\n");
	}
}

void deleteLinkedListSll(struct SinglyLinkedList *SLL){
	struct SinglyLinkedListNode *current, *next;
	current = SLL->head;
	while (current != NULL){
		next = current->next;
		free(current);
		current = next;
	}
	SLL->head = NULL;
}

// inserting:
void insertFrontSll(struct SinglyLinkedList *SLL, struct SinglyLinkedListNode *X){
	X->next = SLL->head;
	SLL->head = X;
}
void insertEndSll(struct SinglyLinkedList *SLL, struct SinglyLinkedListNode *X){
	if (SLL->head == NULL)
		SLL->head = X; // empty list
	else{
		struct SinglyLinkedListNode *current = SLL->head;
		while (current->next != NULL)
			current = current->next;
		current->next = X;
		current = NULL;
	}
}

void insertAfterSll(struct SinglyLinkedList *SLL, struct SinglyLinkedListNode *N, struct SinglyLinkedListNode *X){
	if (N != NULL){
		X->next = N->next;
		N->next = X;
	}
}
void insertBeforeSll(struct SinglyLinkedList *SLL, struct SinglyLinkedListNode *N, struct SinglyLinkedListNode *X){
	if (N != NULL){
		if (SLL->head == N){
			X->next = N;
			SLL->head = X;
		}
		else{
			struct SinglyLinkedListNode *current = SLL->head;
			while (current->next != N)
				current = current->next;
			X->next = current->next;
			current->next = X;
		}
	}
}

// deleting:
int deleteHeadSll(struct SinglyLinkedList *SLL){
	if (SLL->head == NULL) return -1;

	int data = SLL->head->data;
	struct SinglyLinkedListNode *current = SLL->head;
	SLL->head = (SLL->head)->next;
	
	free(current);
	return data;
}

void deleteNodeSll(struct SinglyLinkedList *SLL, struct SinglyLinkedListNode *X){
	if (X == NULL || SLL->head == NULL) return;

	if (X == SLL->head) deleteHeadSll(SLL);
	else{
		struct SinglyLinkedListNode *current = SLL->head;
		while (current->next != X)
			current = current->next; // traversing to the previous node
		current->next = X->next;
		
		free(X);
		X = NULL, current = NULL;
	}
}

void deleteTailSll(struct SinglyLinkedList *SLL){
	if (SLL->head == NULL) return -1;
	
	if ((SLL->head)->next == NULL){ // SLL->head == tail
		int data = SLL->head;
		free(SLL->head);
		SLL->head = NULL;
		return data;
	}

	struct SinglyLinkedListNode *current = SLL->head, *tail;
	while (current->next->next != NULL)
		current = current->next;
	
	int data = current->next->data;
	tail = current->next;
	current->next = NULL;
	
	free(tail);
	return data;
}

// doubly linked list: ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct DoublyLinkedListNode
{
	int data;
	struct DoublyLinkedListNode *prev;
	struct DoublyLinkedListNode *next;
};
struct DoublyLinkedList
{
	struct DoublyLinkedListNode *head;
};

struct node *createNodeDll(int x){
	struct DoublyLinkedListNode *X = malloc(sizeof(struct DoublyLinkedListNode));
	if (X == NULL) exit(0);
	
	X->data = x, X->prev = NULL, X->next = NULL;
	return X;
}

struct node *searchElementDll(struct DoublyLinkedList *DLL, int x){
	if (DLL->head != NULL){
		struct DoublyLinkedListNode *current = DLL->head;
		while (current != NULL){
			if (current->data == x)
				return current;
			current = current->next;
		}
	}
	return NULL;
}

// inserting:
void insertFrontDll(struct DoublyLinkedList *DLL, struct DoublyLinkedListNode *X){
	if (DLL->head == NULL) DLL->head = X;
	else{
		X->next = DLL->head;
		(DLL->head)->prev = X;
		DLL->head = X;
	}
}

void insertEndDll(struct DoublyLinkedList *DLL, struct DoublyLinkedListNode *X){
	if (DLL->head == NULL) DLL->head = X;
	else{
		struct DoublyLinkedListNode *current = DLL->head;
		while (current->next != NULL)
			current = current->next;
		current->next = X;
		X->prev = current;
		current = NULL;
	}
}

void insertAfterDll(struct DoublyLinkedList *DLL, struct DoublyLinkedListNode *N, struct DoublyLinkedListNode *X){
	if (N->next != NULL){
		X->next = N->next;
		N->next->prev = X;
		X->prev = N;
		N->next = X;
	}
	else
		X->next = NULL;

	N->next = X;
	X->prev = N;
}

void insertBeforeDll(struct DoublyLinkedList *DLL, struct DoublyLinkedListNode *N, struct DoublyLinkedListNode *X){
	if (DLL->head != N)
		insertAfterDll(DLL, N->prev, X);
	else
		insertFrontDll(DLL, X);
}

// deleting:
int deleteHeadDll(struct DoublyLinkedList *DLL){
	if (DLL->head == NULL) return -1;

	int tmp = DLL->head->data;
	struct DoublyLinkedListNode *current = DLL->head;
	
	if ((DLL->head)->next != NULL)
		(DLL->head)->next->prev = NULL;
	(DLL->head) = (DLL->head)->next;
	
	free(current);
	current = NULL;
	return tmp;
}

void deleteNodeDll(struct DoublyLinkedList *DLL, struct DoublyLinkedListNode *X){
	if (DLL->head == NULL || X == NULL) return;

	if (DLL->head == X)
		deleteHeadDll(DLL);
	else{
		if (X->next == NULL){
			X->prev->next = NULL;
			free(X);
		}
		else{
			X->prev->next = X->next;
			X->next->prev = X->prev;
			free(X);
		}
	}
}

void deleteTailDll(struct DoublyLinkedList *DLL){
	if (DLL->head == NULL) return -1;

	if ((DLL->head)->next == NULL){
		int data = DLL->head;
		free(DLL->head);
		(DLL->head) = NULL;
		return data;
	}
	else{
		struct DoublyLinkedListNode *current = DLL->head;
		while (current->next->next != NULL)
			current = current->next;
		int data = current->next->data;
		current->next = NULL;
		current = current->next;
		
		free(current);
		return data;
	}
}

void printReverseDllX(struct DoublyLinkedListNode *head, int x){
	struct DoublyLinkedListNode *current = searchElementDll(head, x);
	if (current == NULL) return;

	while (current)
	{
		printf("%d ", current->data);
		current = current->prev;
	}
	printf("\n");
}

void deleteLinkedListDll(struct DoublyLinkedList *DLL)
{
	struct DoublyLinkedListNode *current, *next;
	current = DLL->head;
	while (current != NULL){
		next = current->next;
		free(current);
		current = next;
	}
	DLL->head = NULL;
}

// stack: ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct StackArray
{
	int top, size;
	int arr[100010];
};

bool stackArrEmpty(struct StackArray *stack){
	return stack->top == -1;
}
bool stackArrFull(struct StackArray *stack) {
	return stack->top == stack->size - 1;
}

void stackArrPush(struct StackArray *stack, int x) {
	if (stackArrFull(stack)) return;

	(stack->top)++;
	stack->arr[stack->top] = x;
}
int stackArrPeek(struct StackArray *stack){
	return stackArrEmpty(stack) ? -1 : stack->arr[stack->top];
}
int stackArrPop(struct StackArray *stack){
	return stackArrEmpty(stack) ? -1 : stack->arr[stack->top--];
}

// queue: ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct QueueArray
{
	int arr[10010];
	int head, tail;
	int size, outSize;
};

int incrementMarker(struct QueueArray *queue, int marker){
	return marker < queue->outSize -1 ? marker+1 : 0;
}
bool queueArrEmpty(struct QueueArray *queue){
	return queue->size == 0;
}
bool queueArrFull(struct QueueArray *queue){
	return queue->size == queue->outSize;
}

void queueArrEnqueue(struct QueueArray *queue, int x){
	if (queueArrFull(queue)) return;
	
	queue->arr[queue->tail] = x;
	queue->tail = incrementMarker(queue, (queue->tail));
	(queue->size)++;
}
int queueArrDequeue(struct QueueArray *queue){
	if (queueArrEmpty(queue)) return -1;
	
	int tmp = queue->arr[queue->head];
	(queue->size)--;
	queue->head = incrementMarker(queue, queue->head);
	return tmp;
}

// stack using linked list: ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct StackLinkedList
{
	int size;
	struct SinglyLinkedListNode *top;
};

struct SinglyLinkedListNode *createNodeSll(int x){
	struct SinglyLinkedListNode *temp = malloc(sizeof(struct SinglyLinkedListNode));
	if (temp == NULL) exit(0);

	temp->data = x;
	temp->next = NULL;
	return temp;
}

bool stackLLEmpty(struct StackLinkedList *stack){ 
	return stack->size == 0; 
}
int stackLLPeek(struct StackLinkedList *stack){
	return stackLLEmpty(stack) ? -1 : stack->top->data;
}

void stackLLPush(struct StackLinkedList *stack, int x){
	struct SinglyLinkedListNode *X = createNodeSll(x);
	X->next = stack->top;
	stack->top = X;
	(stack->size)++;
}

int stackLLPop(struct StackLinkedList *stack){
	if (stack->top == NULL) return INT_MIN;

	struct SinglyLinkedListNode *temp = stack->top;
	int data = (stack->top)->data;
	stack->top = (stack->top)->next;
	(stack->size)--;
	
	free(temp);
	temp = NULL;
	return data;
}
void stackLLClear(struct StackLinkedList *stack)
{
	struct SinglyLinkedListNode *current, *next;
	current = stack->top;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	stack->top = NULL;
}

// queue using linked list: ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct QueueLinkedList
{
	int size;
	struct SinglyLinkedListNode *head, *tail;
};

struct SinglyLinkedListNode *createNodeSll(int x)
{
	struct SinglyLinkedListNode *temp = malloc(sizeof(struct SinglyLinkedListNode));
	if (temp == NULL) exit(0);

	temp->data = x;
	temp->next = NULL;
	return temp;
}

bool queueLLEmpty(struct QueueLinkedList *queue){ 
	return queue->size == 0; 
}

void queueLLEnqueue(struct QueueLinkedList *queue, int x){
	struct SinglyLinkedListNode *X = createNodeSll(x);
	
	if (queue->tail != NULL) queue->tail->next = X;
	else queue->head = X;

	queue->tail = X;
	(queue->size)++;
}

int queueLLDequeue(struct QueueLinkedList *queue){
	if (queue->head == NULL) return INT_MIN;

	struct SinglyLinkedListNode *current = queue->head;
	int data = queue->head->data;
	if (queue->head == queue->tail) queue->tail = NULL;
	queue->head = (queue->head)->next;
	(queue->size)--;
	
	free(current);
	current = NULL;
	return data;
}

void queueLLClear(struct QueueLinkedList *queue){
	struct SinglyLinkedListNode *current, *next;
	current = queue->head;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	queue->head = NULL, queue->tail = NULL;
}