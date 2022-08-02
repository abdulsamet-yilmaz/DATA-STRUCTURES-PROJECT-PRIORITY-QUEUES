// Abdül Samet Yýlmaz ID:150118059
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
 typedef struct BH_Node {
 	struct BH_Node *parent;
 	char fileName[40];
 	int key;
 	int degree;
 	struct BH_Node *child;
 	struct BH_Node *sibling;
 }BH_Node;
 
 typedef struct BH_Header {
 	BH_Node *head;
 }BH_Header;
 
 /* Function prototypes */
 BH_Header *BH_Create();
 BH_Node *B0_Create(int key, char fN[40]);
 BH_Node *Merge_Root_Lists(BH_Header *BH1, BH_Header *BH2);
 BH_Node Get_BT_k(BH_Node *c, BH_Node *n);
 BH_Node *BH_Merge(BH_Header *BH1, BH_Header *	BH2);
 void enqueue(BH_Header *hdr, int key, char fN[40]);
 void dequeue(BH_Header *heap, BH_Node *head, BH_Node * prev);
 BH_Node *extract_min(BH_Header *heap);
 
 
int main(void)
{
	char keyword[40];
	// Prompt the user to enter a keyword.
	printf("Enter the keyword that will be searched: ");
	scanf("%s", &keyword);
	
	BH_Header *first = BH_Create(); // Head of the heap.
	
    DIR *d;
    struct dirent *dir;
    d = opendir("./files");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
        	if (strlen(dir->d_name) > 2) {
        		FILE *fPtr;
        		char fDirectory[40] = "./files/";
        		
        		strcat(fDirectory, dir->d_name);
        	
        		fPtr = fopen(fDirectory, "r");

        		if (fPtr == NULL) {
        			printf("Error: File could not opened.");
					return 1;
				}
				char c;
				char word[40];
				int count_down = 200;  // Since my heap is a min-heap most relevant file will have the least key.
				
				while ((c = fgetc(fPtr)) != EOF) {
					if ((c >= 32 && c <= 34) || c == '\n' || c == '.' || c == ',' || c == ':' || c == '?' || c == '(' || c ==')') {
						if (strcmp(keyword, word) == 0)
							count_down--;
						word[0] = 0;	// Emptying the word string
					}
					else {
						strncat(word, &c, 1); // Appending the read character to the end of the word.
					}
				}
				enqueue(first, count_down, dir->d_name);
			}
           
        }
        closedir(d);
    }
    
    int n;
    printf("\nEnter the number of relevant files that you want to see: ");
    scanf("%d", &n);
    printf("\nMost relevant %d files:\n", n);
    
    while (n > 0) {
    BH_Node *min = extract_min(first);
    printf("File Name: %s\tNumber of repeated keywords: %d\n", min->fileName, (200-min->key));
	n--;	
	}
}

// Initializes an empty BH header
 BH_Header *BH_Create() {
 	
 	BH_Header *hdr;
 	hdr = malloc(sizeof(BH_Header));
 	hdr->head = NULL;
 	
 	return hdr;
 }
 
 // Creates an B0 Tree
 BH_Node *B0_Create(int key, char fN[40]) {
 	
 	BH_Node *node = malloc(sizeof(BH_Node));
 	strcpy(node->fileName, fN);
 	node->key = key;
 	node->degree = 0;
 	node->parent = node->child = node->sibling = NULL;
 	
 	return node;
 }
 
 // Merges the roots of two binomial heaps
 BH_Node *Merge_Root_Lists(BH_Header *BH1, BH_Header *BH2) {
 	
 	if (BH1->head == NULL)
 		return BH2->head;
 	if (BH2->head == NULL)
 		return BH1->head;
 		
 	BH_Node *h_root;
 	BH_Node *temp;
 	BH_Node *h1_ptr = BH1->head;
 	BH_Node *h2_ptr = BH2->head;
 	
 	if (BH1->head->degree <= BH2->head->degree) {
 		h_root = BH1->head;
 		h1_ptr = h1_ptr->sibling;	
	 }
	 
	 else {
	 	h_root = BH2->head;
 		h2_ptr = h2_ptr->sibling;
	 }
	 temp = h_root;
	 
	 while (h1_ptr != NULL && h2_ptr != NULL) {
	 	if (h1_ptr->degree <= h2_ptr->degree) {
	 		temp->sibling = h1_ptr;
	 		h1_ptr = h1_ptr->sibling;
		 }
		 
		 else {
		 	temp->sibling = h2_ptr;
		 	h2_ptr = h2_ptr->sibling;
		 }
	 	
	 	temp = temp->sibling;
	 }
	 
	 if (h1_ptr != NULL)
	 	temp->sibling = h1_ptr;
	else
		temp->sibling = h2_ptr;
		
 	return h_root;
 }
 

 // Merges the heaps
 BH_Node *BH_Merge(BH_Header *BH1, BH_Header *BH2) {
 	
 	BH_Node *newHead = Merge_Root_Lists(BH1, BH2);
 	
 	BH1->head = NULL;
 	BH2->head = NULL;
 	
 	if (newHead == NULL)
 		return NULL;
 	
 	BH_Node *pre = NULL;
 	BH_Node *current = newHead;
 	BH_Node *next = newHead->sibling;
 	
 	while (next != NULL) {
 		
 		if (current->degree != next->degree || (next->sibling != NULL && next->sibling->degree == current->degree)) {
 			pre = current;
 			current = next;
		 }
		 else {
		 	if (current->key < next->key) {
		 		
		 		current->sibling = next->sibling;
		 		next->parent = current;
				next->sibling = current->child;
				current->child = next;
				current->degree++; 
		 		
			 }
			 else {
			 	if (pre == NULL)
			 		newHead = next;
			 	else
			 		pre->sibling = next;
			 	
				current->parent = next;
				current->sibling = next->child;
				next->child = current;
				next->degree++; 
				
				current = next;	
			 }
		 }
 		next = current->sibling;	
	 }
	 
	 return newHead;
 }
 
 // Insert implementation
 void enqueue(BH_Header *hdr, int key, char fN[40]) {
 	
 	BH_Node *node = B0_Create(key, fN);
 	BH_Header *temp = BH_Create();
 	temp->head = node;
 	hdr->head = BH_Merge(hdr, temp);
 	free(temp);
 }
 
 // Remove implementation
 void dequeue(BH_Header *heap, BH_Node *head, BH_Node * prev) {
 	
 	if( head == heap->head )
		heap->head = head->sibling;
	else
		prev->sibling = head->sibling;

	BH_Node *newHead = NULL;
	BH_Node *chld = head->child;

	while (chld != NULL)
	{
		BH_Node *next = chld->sibling;
		chld->sibling = newHead;
		chld->parent = NULL;
		newHead = chld;
		chld = next;
	}

	BH_Header *temp = BH_Create();
	temp->head = newHead;
	heap->head = BH_Merge(heap, temp);
	free( temp );
 }
 
 // Extracts the node that has mininmum key
 BH_Node *extract_min(BH_Header *heap) {
 	 
 	if (heap->head == NULL) {
 		printf("Heap is empty.");
 		return NULL;
	 }
	 
	 BH_Node *min_pre = NULL;
	 BH_Node *min = heap->head;
	 BH_Node *next = min->sibling;
	 
	 while (next != NULL) {
	 	if (next->key < min->key) {
	 		min_pre = min;
	 		min = next;
		 }
		 
		 next = next->sibling;
	 }
	 
	 dequeue(heap, min, min_pre);
	 
	 return min;
 }
 
