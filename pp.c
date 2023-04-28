#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define T INT_MAX
#define MOULD_SIZE 100
#define THRESHOLD 10

typedef struct lnode * Lnode;
typedef struct mnode * Mnode;

struct lnode{
    int value;
    Lnode next;
};
struct mnode {
    Lnode list;
    // bool dirty; omitted as unnecessary for this implementation
    // int c; omitted as unnecessary for this implementation
};

Mnode tree[MOULD_SIZE];
int depth = 0;

Lnode createLnode(int value){
    Lnode ln = (Lnode) malloc(sizeof(struct lnode));
    ln->value = value;
    ln->next = NULL;
    return ln;
}

int randLeaf(){
    srand(time(NULL));
    int ran, low = (int) (pow(2,depth) - 1), upp = (int) (pow(2,depth+1) - 2);
    ran = (rand() % (upp - low + 1)) + low;
    return ran;
}

int val(Mnode mn){
    if (mn->list == NULL) return T;
    return mn->list->value;
}

void swap(int m1, int m2){
    Mnode temp = tree[m1];
    tree[m1] = tree[m2];
    tree[m2] = temp;
}

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

int insert(int value){
    Lnode ln = createLnode(value);
    int c = findInsertPoint(value);
    
    ln->next = tree[c]->list;
    tree[c]->list = ln;
}

// Extracts and returns the minimum element from the heap
int removeMin() {
    if (tree[0]->list == NULL) return T; // Heap is empty, return maximum integer value
    int min = tree[0]->list->value; // Minimum value is at the root node
    Lnode root = tree[0]->list; // Store the root node temporarily
    tree[0]->list = tree[0]->list->next; // Replace root node with its next node
    free(root); // Free the memory allocated for the root node
    moundify(0); // Restore the heap property starting from the root node
    return min; // Return the minimum value
}

// Restores the heap property starting from the given index
void moundify(int ind) {
    int left = 2 * ind + 1; // Calculate the left child index
    int right = 2 * ind + 2; // Calculate the right child index
    int smallest = ind; // Assume the smallest element is the parent
    if (left < MOULD_SIZE && val(tree[left]) < val(tree[smallest])) // If the left child is smaller than the parent
        smallest = left;
    if (right < MOULD_SIZE && val(tree[right]) < val(tree[smallest])) // If the right child is smaller than the parent or left child
        smallest = right;
    if (smallest != ind) { // If the smallest element is not the parent
        swap(ind, smallest); // Swap the parent with the smallest child
        moundify(smallest); // Recursively moundify the subtree rooted at the smallest child
    }
}

Lnode extractMany(){
    if (tree[0]->list == NULL) return NULL; // Heap is empty, return maximum integer value
    Lnode root = tree[0]->list; // Temporarily store the start of list to be returned
    tree[0]->list = NULL; // Make the root node empty
    moundify(0); // Restore the heap property starting from the root node
    return root; // Return the minimum value
}

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
    printf("\nMinimum value is %d\n", removeMin);

    return 0;
}