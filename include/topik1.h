#ifndef TOPIK1_H
#define TOPIK1_H

#include <stddef.h>

#define MAX_USERNAME 31
#define MAX_PASSWORD 31
#define MAX_CATEGORY 15
#define MAX_DATE 23
#define MAX_INPUT 128
#define HASH_TABLE_SIZE 31
#define QUEUE_CAPACITY 100
#define STACK_CAPACITY 100
#define MAX_GRAPH_VERTICES 20
#define DATA_DIRECTORY "data"
#define USERS_FILE "data/akun.txt"
#define PACKAGES_FILE "data/paket.txt"
#define TRANSACTIONS_FILE "data/transaksi.txt"
#define GRAPH_FILE "data/zona.txt"

typedef struct {
    int id;
    char category[MAX_CATEGORY + 1];
    int presale;
    long price;
    int quota;
    int sold;
} TicketPackage;

typedef struct {
    int id;
    char username[MAX_USERNAME + 1];
    int package_id;
    int quantity;
    long total;
    char created_at[MAX_DATE + 1];
} Transaction;

typedef struct {
    char username[MAX_USERNAME + 1];
    int package_id;
    int quantity;
} PurchaseRequest;

typedef struct {
    int transaction_id;
} SaleRecord;

struct UserNode;
struct BSTNode;

typedef struct AppState {
    TicketPackage *packages;
    size_t package_count;
    size_t package_capacity;

    Transaction *transactions;
    size_t transaction_count;
    size_t transaction_capacity;

    struct UserNode *user_table[HASH_TABLE_SIZE];

    PurchaseRequest purchase_queue[QUEUE_CAPACITY];
    int queue_front;
    int queue_rear;
    int queue_count;

    SaleRecord undo_stack[STACK_CAPACITY];
    int stack_top;

    struct BSTNode *order_root;
    char current_username[MAX_USERNAME + 1];
    int next_transaction_id;
} AppState;

void read_line(const char *prompt, char *buffer, size_t buffer_size);
int read_int_range(const char *prompt, int min_value, int max_value);
void format_currency(long amount, char *buffer, size_t buffer_size);
int find_package_index_linear(const AppState *app, int package_id);
int find_transaction_index_linear(const AppState *app, int transaction_id);
void print_package(const TicketPackage *package);
void print_transaction(const Transaction *transaction);

#endif
