/*
Mounds : write the description here of the data structure
*/
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define T INT_MAX
#define MOULD_SIZE 127
#define THRESHOLD 32

typedef struct lnode * Lnode;
typedef struct mnode * Mnode;

// Structure definition of linkedlist node and mound node
struct lnode{
    int value;
    Lnode next;
};
struct mnode {
    Lnode list;
    // bool dirty; omitted as unnecessary for this implementation
    // int c; omitted as unnecessary for this implementation
};

// Global declaration for tree(mound nodes ptr array)
Mnode tree[MOULD_SIZE];
int depth = 0;

// Function for creating wrapper Lnode for given value
Lnode createLnode(int value){
    Lnode ln = (Lnode) malloc(sizeof(struct lnode));
    ln->value = value;
    ln->next = NULL;
    return ln;
}

// Function to generate random number/index which points to leaf nodes in tree (mound) array
int randLeaf(){
    srand(time(NULL));
    int ran;
    int low = (int) (pow(2,depth) - 1);
    int upp = (int) (pow(2,depth+1) - 2);
    ran = (rand() % (upp - low + 1)) + low;
    return ran;
}

// Function to determine value of a mound node
int val(Mnode mn){
    if (mn->list == NULL) return T;
    return mn->list->value;
}

// For swapping the Mnodes at two given indices in the array
void swap(int m1, int m2){
    Mnode temp = tree[m1];
    tree[m1] = tree[m2];
    tree[m2] = temp;
}

// For selecting insertion point based parent node constraint in mound
int binarySearchLeaf(int ind, int v){
    if (ind){
        int par = (ind - 1) / 2;
        if (val(tree[par]) <= v)
            return ind;
        else
            binarySearchLeaf(par, v);
    }
    else
        return ind;
}

// For selecting insertion point based child node constraint in mound
int findInsertPoint(int value){
    int rip=MOULD_SIZE;
    while (true){
        for (int i=0;i<THRESHOLD;i++){
            rip = randLeaf();
            if (val(tree[rip]) >= value)
                return binarySearchLeaf(rip, value);
        }
        depth++;
    }
}

// Function for inserting value into tree (mound array)
int insert(int value){
    Lnode ln = createLnode(value);
    int c = findInsertPoint(value);
    
    ln->next = tree[c]->list;
    tree[c]->list = ln;
}

// Extracts and returns the minimum element from the tree(mound)
int removeMin() {
    if (tree[0]->list == NULL) 
        return T;                   // Tree(mound) is empty, return maximum integer value
    int min = tree[0]->list->value; // Minimum value is at the root node
    Lnode root = tree[0]->list;     // Stores the root node temporarily
    tree[0]->list = tree[0]->list->next; // Replaces root node with its next node
    free(root);                     // Free the memory allocated for the root node
    moundify(0);                    // Restores the mound property starting from the root node
    return min;                     // Returns the minimum value
}

// Restores the mound property starting from the given index
void moundify(int ind) {
    int l = 2 * ind + 1;     // Calculates the index of left child
    int r = 2 * ind + 2;    // Calculates the index of right child
    int smallest = ind;         // Assumes the smallest element is the parent
    if (l < MOULD_SIZE && val(tree[l]) < val(tree[smallest])) 
        smallest = l;
    if (right < MOULD_SIZE && val(tree[r]) < val(tree[smallest]))
        smallest = r;
    if (smallest != ind) {      
        swap(ind, smallest);    // Swaps the parent with the smallest child
        moundify(smallest);     // Recursively moundify the subtree rooted at the smallest child
    }
}

// For simultaneously extracting multiple values. Extracts all values as list from the root node
Lnode extractMany(){
    if (tree[0]->list == NULL) return NULL; // Tree(Mound) is empty, return maximum integer value
    Lnode root = tree[0]->list;             // Temporarily stores the start of list to be returned
    tree[0]->list = NULL;                   // Makes the root node empty
    moundify(0);                            // Restores the Mound property starting from the root node
    return root;                            // Returns the list of values
}

// Prints all elements of tree (mounnd) in ascending order by extracting the minimum of them multiple times
void emptyMound(){
    printf("\n");
    while (tree[0]->list!=NULL){
        printf("%d ", removeMin());
    }
    printf("\nMound emptied and printed successfully\n");
}

// reading Data from file for insertion into tree
void readData(FILE* fp){
    int num;
    while (fscanf(fp, "%d", &num) == 1) {
        insert(num);
    }
}

int main(){
    // initializing the tree (array of Mound nodes ptrs)
    for(int i=0;i<MOULD_SIZE;i++){
        tree[i] = (Mnode) malloc(sizeof(struct mnode));
        tree[i]->list = NULL;
    }

    // reading Data from file for insertion into tree
    FILE* fp = fopen("data.txt", "r");
    if (fp==NULL){
        printf("File didn't open successfully");
        exit(1);
    }
    readData(fp);
    fclose(fp);

    //extract and print the min value
    printf("\nMinimum value is %d\n", removeMin());
    
    // extract and print the remaining values in ascending order
    emptyMound();
    
    return 0;
}
