#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>      // input/print
#include <stdlib.h>     // alloc

// Node structure
typedef struct Node {
    int key;        // key
    int height;     // depth
    struct Node* left;      // left Node
    struct Node* right;     // right Node
}Node;

Node* getNodeBST(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));    // alloc new Node
    newNode->height = 1;        // depth initialization
    newNode->key = key;     // key initialization
    newNode->left = newNode->right = NULL;      // both of Node init
    return newNode;     // return new Node
}

int insertBST(Node** T, int key) {
    Node* q = NULL;     // parent Node
    Node* p = *T;       // child Node

    while (p != NULL) {     // stop if parent Node is NULL
        if (key == p->key) {
            printf("i %d: The key already exists", key);
            return 0;
        }
        q = p;      // new parent if key doesn't exist
        if (key < p->key) p = p->left;      // if key is less than parent --> move left
        else p = p->right;      // equal
    }

    Node* newNode = getNodeBST(key);        // newNode create   
    if (*T == NULL) *T = newNode;       // insert newNode into child
    else if (key < q->key) q->left = newNode;
    else q->right = newNode;

    return 1;
}

int height(Node* node) {
    if (node == NULL) return 0;     // no node anymore
    int LH = height(node->left);        // height of left nodes
    int RH = height(node->right);       // height of right nodes

    return ((LH > RH) ? (LH + 1) : (RH + 1));       // return height
}

int size(Node* node) {
    if (node == NULL) return 0;
    return (1 + size(node->left) + size(node->right));      // sum of right, left nodes
}

Node* minNode(Node* n) {        // find minimum key
    while (n->left != NULL) {
        n = n->left;
    }

    return n;
}

Node* maxNode(Node* n) {        // equal above
    while (n->right != NULL) {
        n = n->right;
    }

    return n;
}

Node* searchBST(Node* T, int key) {     // find node that has the key
    if (T == NULL) return NULL;
    if (key == T->key) return T;
    if (key < T->key) return searchBST(T->left, key);
    else return searchBST(T->right, key);
}

Node* searchParent(Node* T, int key) {
    Node* p = T;       // child Node
    Node* q = NULL;     // parent Node 

    while ((p != NULL) && (p->key != key)) {
        q = p;
        if (key < p->key) p = p->left;
        else p = p->right;
    }

    return q;
}

int eraseBST(Node** T, int key) {
    Node* p = searchBST(*T, key);        // child
    Node* q = searchParent(*T, key);     // parent

    if (p == NULL) {
        printf("d %d: The key does not exist", key);
        return 0;
    }

    if (p->left == NULL && p->right == NULL) {      // leaf node
        if (q == NULL) *T = NULL;
        else if (q->left == p) q->left = NULL;
        else q->right = NULL;
        free(p);
    }
    else if (p->left == NULL || p->right == NULL) {      // one child
        Node* c = NULL;
        if (p->left != NULL) {
            c = p->left;
        }
        else {
            c = p->right;
        }
        if (q == NULL) *T = c;      // move of child of p
        else if (q->left == p) q->left = c;     // if (left == p) ==> move
        else q->right = c;      // right ==> move
        free(p);
    }
    else if (p->left != NULL && p->right != NULL) {     // two children
        Node* r = NULL;
        int flag = 0;       // var to decide to remove left or right node

        if (height(p->left) > height(p->right)) {
            r = maxNode(p->left); 
            flag = 0;
        }
        else if (height(p->left) < height(p->right)) {
            r = minNode(p->right); 
            flag = 1;
        }
        else if (height(p->left) == height(p->right)) {
            if (size(p->left) >= size(p->right)) {
                r = maxNode(p->left); flag = 0;
            }
            else {
                r = maxNode(p->right); flag = 1;
            }
        }

        p->key = r->key;        // key copy
        if (flag == 0) eraseBST(&(p->left), r->key);
        else eraseBST(&(p->right), r->key);
    }

    return 1;
}

void inorder(Node* T) {
    if (T != NULL) {
        printf("<");
        inorder(T->left);
        printf(" %d ", T->key);
        inorder(T->right);
        printf(">");
    }
}

void freeNode(Node* T) {
    if (T != NULL) {
        freeNode(T->left);
        freeNode(T->right);
        free(T);
    }
}

int main() {
    char ins;
    int key, i = 0;
    Node* T = NULL;

    while (i++ < 1000) {
        scanf(" %c%d", &ins, &key);
        if (ins == 'i') {
            if (insertBST(&T, key))     // insert
                inorder(T);
            printf("\n");
            continue;
        }
        else if (ins == 'd') {
            if (eraseBST(&T, key))      // delete
                inorder(T);
            printf("\n");
            if(T != NULL) continue;
        }
        if (T == NULL)break;        // stop
    }

    freeNode(T);        // free

    return 0;
}
