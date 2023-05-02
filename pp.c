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

int mound_size = 127;
int max_depth = 6;
int threshold = 1;

typedef struct lnode * Lnode;

// structure definition of linkedlist node
struct lnode{
    int value;
    Lnode next;
};

// global declaration for tree(mound array (array of LL pointers))
Lnode *tree;
int depth = 0;

void inc_and_recalculate(){
    int old_size = mound_size;
    mound_size = (mound_size + 1) * 8 - 1;
    max_depth = max_depth + 3;
    tree = (Lnode *) realloc(tree, (mound_size * sizeof(Lnode)));
    for (int i=old_size;i<mound_size;i++){
        tree[i] = NULL;
    }
}

// function for creating wrapper Lnode for given value
Lnode createLnode(int value){
    Lnode ln = (Lnode) malloc(sizeof(struct lnode));
    ln->value = value;
    ln->next = NULL;
    return ln;
}

// function to generate random number/index which points to leaf nodes in tree (mound) array
int randLeaf(){
    srand(time(NULL));
    int ran;
    int low = (int) (pow(2,depth) - 1);
    int upp = (int) (pow(2,depth+1) - 2);
    ran = (rand() % (upp - low + 1)) + low;
    return ran;
}

// function to determine value of a mound node
int val(Lnode mn){
    if (mn == NULL) return T;
    return mn->value;
}

// for swapping the Mnodes at two given indices in the array
void swap(int m1, int m2){
    
    Lnode temp = tree[m1];
    tree[m1] = tree[m2];
    tree[m2] = temp;
}

// for selecting insertion point based parent node constraint in mound
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

// for selecting insertion point based child node constraint in mound
int findInsertPoint(int value){
    int rip=mound_size;
    while (true){
        for (int i=0;i<threshold;i++){
            rip = randLeaf();
            if (val(tree[rip]) >= value)
                return binarySearchLeaf(rip, value);
        }
        if (depth == max_depth)
            inc_and_recalculate();
        depth++;
        threshold = threshold * 2;
    }
}

// function for inserting value into tree (mound array)
int insert(int value){
    Lnode ln = createLnode(value);
    int c = findInsertPoint(value);
    ln->next = tree[c];
    tree[c] = ln;
}

// Extracts and returns the minimum element from the tree(mound)
int removeMin() {
    if (tree[0]==NULL){ // tree(mound) is empty, return maximum integer value
        return T;
    }
    int min = tree[0]->value; // Minimum value is at the root node
    Lnode root = tree[0]; // Store the root node temporarily
    tree[0] = tree[0]->next; // Replace root node with its next node
    free(root); // Free the memory allocated for the root node
    moundify(0); // Restore the mound property starting from the root node
    return min; // Return the minimum value
}

// Restores the mound property starting from the given index going recursively
void moundify(int ind) {
    int left = 2 * ind + 1; // Calculate the left child index
    int right = 2 * ind + 2; // Calculate the right child index
    int smallest = ind; // Assume the smallest element is the parent
    if (left < mound_size && val(tree[left]) < val(tree[smallest])) // If the left child is smaller than the parent
        smallest = left;
    if (right < mound_size && val(tree[right]) < val(tree[smallest])) // If the right child is smaller than the parent or left child
        smallest = right;
    if (smallest != ind) { // If the smallest element is not the parent
        swap(ind, smallest); // Swap the parent with the smallest child
        moundify(smallest); // Recursively moundify the subtree rooted at the smallest child
    }
}

// for simultaneously extracting multiple values. Extracts all values as list from the root node
Lnode extractMany(){
    if (tree[0] == NULL) return NULL; // tree(Mound) is empty, return Empty linked list
    Lnode root = tree[0]; // Temporarily store the start of list to be returned
    tree[0] = NULL; // Make the root node empty
    moundify(0); // Restore the Mound property starting from the root node
    return root; // Return the list of values
}

// print all elements of tree (mounnd) in ascending order by extracting the minimum of them multiple times
void emptyMound(){
    printf("\n");
    while (tree[0]!=NULL){
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
    tree = (Lnode *) malloc(mound_size * sizeof(Lnode));
    for(int i=0;i<mound_size;i++){
        tree[i] = NULL;
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