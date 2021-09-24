#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct TNode {
    char* val;
    struct TNode* left;
    struct TNode* right;
} TNode;

TNode* makeEmptyTree()
{
    TNode* n = malloc(sizeof(TNode));
    return memset(n, 0, sizeof(TNode));
}

static inline TNode* emplace_node(TNode** n, char* str)
{
    assert(!(*n));
    *n = calloc(1, sizeof(TNode));
    (*n)->val = strdup(str);
    return *n;
}

TNode* insertIntoTree(TNode* root, char* string)
{
    /*
      TODO: Insert the string into the tree. First, check if the root is NULL.
      If it is, then create a new TNode, copy the string into it, and return the
      pointer to the new TNode.

      If root is not NULL, then compare root->val and string using strcmp. If
      root->val < string, then call insertIntoTree again on root->left. Keep in
      mind that root->left might be NULL. If root->val > string, then call
      insertIntoTreeAgain on root->right. Again, root->right might be NULL. If
      root->val is equal to string, then the string is already in the tree, and
      you don't need to do anything. In any case, return root.
     */
    if (!root->val) {
        root->val = strdup(string);
        return root;
    }
    int cmp = strcmp(root->val, string);
    if (cmp > 0) {
        if (root->left) {
            return insertIntoTree(root->left, string);
        }
        else {
            return emplace_node(&root->left, string);
        }
    }
    else if (cmp < 0) {
        if (root->right) {
            return insertIntoTree(root->right, string);
        }
        else {
            return emplace_node(&root->right, string);
        }
    }
    else {
        return root;
    }
    assert(false); // unreachable
}

TNode* searchTree(TNode* root, char* string)
{
    if (root) {
        /*
          TODO: Search the tree for the string. First, use strcmp to
          compare root->val to string.

          If string == root->val, return the root.

          If string < root->val, then call searchTree on the right child
          of root.

          If string > root->val, then call searchTree on the left child
          of root.
         */
        int cmp = strcmp(root->val, string);
        if (cmp > 0) {
            return searchTree(root->left, string);
        }
        else if (cmp < 0) {
            return searchTree(root->right, string);
        }
        else {
            return root;
        }
    }
    else {
        return NULL;
    }
}

void printTree(TNode* root)
{
    if (root == NULL)
        return;
    else {
        printf("(");
        printTree(root->left);
        printf(" %s ", root->val);
        printTree(root->right);
        printf(")");
    }
}

void destroyTree(TNode* root)
{
    if (!root)
        return;

    destroyTree(root->left);
    destroyTree(root->right);
    free(root->val);
    free(root);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: ./tree wordList\n");
        return 1;
    }
    /*
      TODO: The path to the file containing the words is stored in
      argv[1]. Call fopen to open it.
     */
    FILE* fp = fopen(argv[1], "r");
    TNode* tree = makeEmptyTree();
    if (!fp) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    char word[256];
    size_t len;
    /*
      TODO: Use fscanf to read a word at a time from the file, and call
      insertIntoTree to insert the word into the tree.
     */
    while (fscanf(fp, "%255s", word) > 0) {
        insertIntoTree(tree, word);
    }

    /*
     *
      TODO: fclose the file.
     */
    if (fclose(fp))
        perror("fclose");

    printTree(tree);

    while (1) {
        printf("word: ");
        int r = scanf("%s", word);
        if (r != 1)
            break;
        len = strlen(word);
        if (len < 1)
            break;
        TNode* node = searchTree(tree, word);
        if (node) {
            printf("PRESENT\n");
        }
        else {
            printf("ABSENT\n");
        }
    }
    destroyTree(tree);
    return 0;
}
