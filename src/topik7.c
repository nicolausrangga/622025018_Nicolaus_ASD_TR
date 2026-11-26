#include "topik7.h"

#include "topik2.h"

#include <stdio.h>
#include <stdlib.h>

BSTNode *tree_insert(BSTNode *root, const Transaction *transaction) {
    if (root == NULL) {
        BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
        if (node == NULL) {
            return NULL;
        }
        node->data = *transaction;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    if (transaction->id < root->data.id) {
        root->left = tree_insert(root->left, transaction);
    } else if (transaction->id > root->data.id) {
        root->right = tree_insert(root->right, transaction);
    }
    return root;
}

BSTNode *tree_search(BSTNode *root, int transaction_id) {
    while (root != NULL) {
        if (transaction_id == root->data.id) {
            return root;
        }
        root = transaction_id < root->data.id ? root->left : root->right;
    }
    return NULL;
}

static BSTNode *find_minimum_node(BSTNode *root) {
    while (root != NULL && root->left != NULL) {
        root = root->left;
    }
    return root;
}

BSTNode *tree_delete(BSTNode *root, int transaction_id) {
    BSTNode *successor;
    if (root == NULL) {
        return NULL;
    }
    if (transaction_id < root->data.id) {
        root->left = tree_delete(root->left, transaction_id);
    } else if (transaction_id > root->data.id) {
        root->right = tree_delete(root->right, transaction_id);
    } else {
        if (root->left == NULL) {
            BSTNode *right_child = root->right;
            free(root);
            return right_child;
        }
        if (root->right == NULL) {
            BSTNode *left_child = root->left;
            free(root);
            return left_child;
        }
        successor = find_minimum_node(root->right);
        root->data = successor->data;
        root->right = tree_delete(root->right, successor->data.id);
    }
    return root;
}

void tree_preorder(const BSTNode *root) {
    if (root == NULL) return;
    printf("%d ", root->data.id);
    tree_preorder(root->left);
    tree_preorder(root->right);
}

void tree_inorder(const BSTNode *root) {
    if (root == NULL) return;
    tree_inorder(root->left);
    printf("%d ", root->data.id);
    tree_inorder(root->right);
}

void tree_postorder(const BSTNode *root) {
    if (root == NULL) return;
    tree_postorder(root->left);
    tree_postorder(root->right);
    printf("%d ", root->data.id);
}

void tree_clear(BSTNode *root) {
    if (root == NULL) return;
    tree_clear(root->left);
    tree_clear(root->right);
    free(root);
}

void tree_rebuild(AppState *app) {
    size_t i;
    tree_clear(app->order_root);
    app->order_root = NULL;
    for (i = 0; i < app->transaction_count; i++) {
        app->order_root = tree_insert(app->order_root, &app->transactions[i]);
    }
}

void tree_menu(AppState *app) {
    int choice;
    int transaction_id;
    BSTNode *node;
    int result;

    printf("\n=== BINARY SEARCH TREE TRANSAKSI ===\n");
    printf("1. Traversal Pre-order\n");
    printf("2. Traversal In-order\n");
    printf("3. Traversal Post-order\n");
    printf("4. Search transaksi\n");
    printf("5. Delete / batalkan transaksi milik akun sendiri\n");
    choice = read_int_range("Pilih operasi BST: ", 1, 5);

    switch (choice) {
        case 1:
            printf("Pre-order: ");
            tree_preorder(app->order_root);
            printf("\n");
            break;
        case 2:
            printf("In-order: ");
            tree_inorder(app->order_root);
            printf("\n");
            break;
        case 3:
            printf("Post-order: ");
            tree_postorder(app->order_root);
            printf("\n");
            break;
        case 4:
            transaction_id = read_int_range("Masukkan ID transaksi: ", 1, 1000000000);
            node = tree_search(app->order_root, transaction_id);
            if (node == NULL) {
                printf("Transaksi tidak ditemukan pada BST.\n");
            } else {
                print_transaction(&node->data);
            }
            break;
        case 5:
            transaction_id = read_int_range("Masukkan ID transaksi yang akan dibatalkan: ", 1, 1000000000);
            node = tree_search(app->order_root, transaction_id);
            if (node == NULL) {
                printf("Transaksi tidak ditemukan pada BST.\n");
                break;
            }
            result = cancel_user_transaction(app, transaction_id);
            if (result == 1) {
                printf("Delete BST berhasil. Transaksi dibatalkan dan kuota dikembalikan.\n");
            } else if (result == -1) {
                printf("Pembatalan ditolak. Transaksi tersebut bukan milik akun yang sedang login.\n");
            } else {
                printf("Pembatalan gagal. Pastikan transaksi masih tersedia.\n");
            }
            break;
        default:
            break;
    }
}
