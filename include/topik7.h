#ifndef TOPIK7_H
#define TOPIK7_H

#include "topik1.h"

typedef struct BSTNode {
    Transaction data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode *tree_insert(BSTNode *root, const Transaction *transaction);
BSTNode *tree_search(BSTNode *root, int transaction_id);
BSTNode *tree_delete(BSTNode *root, int transaction_id);
void tree_preorder(const BSTNode *root);
void tree_inorder(const BSTNode *root);
void tree_postorder(const BSTNode *root);
void tree_clear(BSTNode *root);
void tree_rebuild(AppState *app);
void tree_menu(AppState *app);

#endif
