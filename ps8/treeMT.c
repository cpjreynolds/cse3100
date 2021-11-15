#include "treeMT.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline TNode* make_node(int val)
{
    TNode* n = malloc(sizeof(TNode));
    n->left = n->right = NULL;
    pthread_mutex_init(&n->lock, NULL);
    n->val = val;
    return n;
}

Tree* makeEmptyTree()
{
    /*
      TODO: Create a new Tree, setting the root to NULL. Initialize any
      necessary mutexes.

      Return Tree*.
     */
    Tree* t = malloc(sizeof(Tree));
    pthread_mutex_init(&t->lock, NULL);
    t->root = NULL;
    return t;
}

void insertIntoTree(Tree* t, int val)
{
    /*
      TODO: This function should construct a new Node, traverse the BST to find
      the correct insertion location, and insert it. If there is already a node
      with that value, nothing needs to be inserted.

      Use mutexes to prevent threads from interfering with one another.
     */
    TNode** cursor = &t->root;
    TNode** next = NULL;
    pthread_mutex_t* curlock = &t->lock;
    pthread_mutex_lock(curlock);
    // edge case: no root
    if (!t->root) {
        t->root = make_node(val);
        pthread_mutex_unlock(&t->lock);
        return;
    }

    // states
    // 1. cursor points to pointer to null. create new node.
    // 2. cursor points to valid node. do comparison and move cursor.
    while (1) {
        if (val > (*cursor)->val) {   // traverse right
            next = &(*cursor)->right; // update cursor to next node.
        }
        else if (val < (*cursor)->val) { // traverse left
            next = &(*cursor)->left;
        }
        else if (val == (*cursor)->val) { // do nothing
            pthread_mutex_unlock(curlock);
            return;
        }
        else {
            // sanity check.
            assert(0);
        }
        if (!*next) { // hit bottom
            break;
        }
        pthread_mutex_lock(&(*next)->lock);
        pthread_mutex_unlock(curlock);
        curlock = &(*next)->lock;
        cursor = next;
    }

    *next = make_node(val);
    pthread_mutex_unlock(curlock);
}

void printTreeAux(TNode* root)
{
    if (root == NULL)
        return;
    else {
        printf("(");
        printTreeAux(root->left);
        printf(" %d ", root->val);
        printTreeAux(root->right);
        printf(")");
    }
}

void printTree(Tree* t)
{
    printTreeAux(t->root);
}

void destroyTreeAux(TNode* root)
{
    if (root != NULL) {
        destroyTreeAux(root->left);
        destroyTreeAux(root->right);
        pthread_mutex_destroy(&root->lock);
        free(root);
    }
}

void destroyTree(Tree* t)
{
    destroyTreeAux(t->root);
    pthread_mutex_destroy(&t->lock);
    free(t);
}
