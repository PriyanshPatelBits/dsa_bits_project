#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define T INT_MAX
#define MOULD_SIZE 50
#define THRESHOLD 10

typedef struct lnode * Lnode;
typedef struct mnode * Mnode;

struct lnode{
    int value;
    Lnode next;
};
struct mnode {
    Lnode list;
    bool dirty;
    // i have omitted integer c as it was unnecessary for this project
};

Mnode tree[MOULD_SIZE];
int depth = 1;

Lnode createLnode(int value){
    Lnode ln = (Lnode) malloc(sizeof(struct lnode));
    ln->value = value;
    ln->next = NULL;
    return ln;
}
int randLeaf(){
    srand(time(NULL));
    int ran, low = (int) pow(2,depth-1), upp = (int)(pow(2,depth) + 1);
    ran = (rand() % (upp - low + 1)) + low - 1;
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
    int rip=0;
    while (true){
        for (int i=0;i<THRESHOLD;i++){
            rip = randLeaf();
            if (val(tree[rip]) >= value)
                return binarySearchLeaf(rip, value);
        }
        depth++;
    }
}

// void moundify(int ind){
//     Mnode c1 = 
//     if (val(nod) < )
// }

int insert(int value){
    Lnode ln = createLnode(value);
    int c = findInsertPoint(value);
    
    ln->next = tree[c]->list;
    tree[c]->list = ln;
}

int main(){
    for(int i=0;i<MOULD_SIZE;i++){
        tree[i] = (Mnode) malloc(sizeof(struct mnode));
        tree[i]->list = NULL;
        tree[i]->dirty = false;
    }

    return 0;
}