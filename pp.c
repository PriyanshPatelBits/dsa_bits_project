#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define T INT_MAX
#define MOULD_SIZE 50

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
int randLeaf(int d){
    srand(time(NULL));
    int ran, low = (int) pow(2,d-1), upp = (int)(pow(2,d) + 1);
    ran = (rand() % (upp - low + 1)) + low;
    return ran;
}

int val(Mnode mn){
    if (mn->list == NULL) return T;
    return mn->list->value;
}

int binarySearchLeaf();

int findInsertPoint(int value);

void mouldify(Mnode nod);

int insert(int value){
    Lnode ln = createLnode(value);
    Mnode ip = tree[findInsertPoint(value)];
    ln->next = ip->list;
    ip->list = ln;
    ip->dirty = true;
    mouldify(ip);
}

int main(){
    for(int i=0;i<MOULD_SIZE;i++){
        tree[i] = (Mnode) malloc(sizeof(struct mnode));
        tree[i]->list = NULL;
        tree[i]->dirty = false;
    }
    return 0;
}