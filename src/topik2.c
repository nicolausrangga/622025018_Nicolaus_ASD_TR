#include "topik2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_PACKAGE_CAPACITY 8
#define INITIAL_TRANSACTION_CAPACITY 16

static int grow_package_array(AppState *app) {
    size_t new_capacity = app->package_capacity == 0 ? INITIAL_PACKAGE_CAPACITY : app->package_capacity * 2;
    TicketPackage *new_items = (TicketPackage *)realloc(app->packages, new_capacity * sizeof(TicketPackage));
    if (new_items == NULL) {
        return 0;
    }
    app->packages = new_items;
    app->package_capacity = new_capacity;
    return 1;
}

static int grow_transaction_array(AppState *app) {
    size_t new_capacity = app->transaction_capacity == 0 ? INITIAL_TRANSACTION_CAPACITY : app->transaction_capacity * 2;
    Transaction *new_items = (Transaction *)realloc(app->transactions, new_capacity * sizeof(Transaction));
    if (new_items == NULL) {
        return 0;
    }
    app->transactions = new_items;
    app->transaction_capacity = new_capacity;
    return 1;
}

int ensure_data_files(void) {
    FILE *file;
    const char *paths[] = {USERS_FILE, PACKAGES_FILE, TRANSACTIONS_FILE, GRAPH_FILE};
    size_t i;

    for (i = 0; i < sizeof(paths) / sizeof(paths[0]); i++) {
        file = fopen(paths[i], "a");
        if (file == NULL) {
            fprintf(stderr, "Gagal mengakses %s. Jalankan program dari folder utama proyek.\n", paths[i]);
            return 0;
        }
        fclose(file);
    }
    return 1;
}

static void seed_default_packages(AppState *app) {
    static const TicketPackage defaults[] = {
        {101, "REGULER", 1, 150000L, 50, 0},
        {102, "REGULER", 2, 175000L, 100, 0},
        {103, "REGULER", 3, 200000L, 150, 0},
        {201, "VIP", 1, 300000L, 20, 0},
        {202, "VIP", 2, 350000L, 40, 0},
        {203, "VIP", 3, 400000L, 60, 0}
    };
    size_t count = sizeof(defaults) / sizeof(defaults[0]);

    app->packages = (TicketPackage *)malloc(count * sizeof(TicketPackage));
    if (app->packages == NULL) {
        app->package_count = 0;
        app->package_capacity = 0;
        return;
    }
    memcpy(app->packages, defaults, count * sizeof(TicketPackage));
    app->package_count = count;
    app->package_capacity = count;
}

int load_packages(AppState *app) {
    FILE *file = fopen(PACKAGES_FILE, "r");
    char line[256];
    TicketPackage package;

    if (file == NULL) {
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d|%15[^|]|%d|%ld|%d|%d",
                   &package.id, package.category, &package.presale,
                   &package.price, &package.quota, &package.sold) == 6) {
            if (app->package_count == app->package_capacity && !grow_package_array(app)) {
                fclose(file);
                return 0;
            }
            app->packages[app->package_count++] = package;
        }
    }
    fclose(file);

    if (app->package_count == 0) {
        seed_default_packages(app);
        if (app->packages == NULL || !save_packages(app)) {
            return 0;
        }
    }
    return 1;
}

int save_packages(const AppState *app) {
    FILE *file = fopen(PACKAGES_FILE, "w");
    size_t i;
    if (file == NULL) {
        return 0;
    }
    for (i = 0; i < app->package_count; i++) {
        const TicketPackage *p = &app->packages[i];
        if (fprintf(file, "%d|%s|%d|%ld|%d|%d\n", p->id, p->category,
                    p->presale, p->price, p->quota, p->sold) < 0) {
            fclose(file);
            return 0;
        }
    }
    return fclose(file) == 0;
}

int load_transactions(AppState *app) {
    FILE *file = fopen(TRANSACTIONS_FILE, "r");
    char line[256];
    Transaction transaction;

    if (file == NULL) {
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d|%31[^|]|%d|%d|%ld|%23[^\r\n]",
                   &transaction.id, transaction.username, &transaction.package_id,
                   &transaction.quantity, &transaction.total, transaction.created_at) == 6) {
            if (app->transaction_count == app->transaction_capacity && !grow_transaction_array(app)) {
                fclose(file);
                return 0;
            }
            app->transactions[app->transaction_count++] = transaction;
            if (transaction.id >= app->next_transaction_id) {
                app->next_transaction_id = transaction.id + 1;
            }
        }
    }
    fclose(file);
    return 1;
}

int save_transactions(const AppState *app) {
    FILE *file = fopen(TRANSACTIONS_FILE, "w");
    size_t i;
    if (file == NULL) {
        return 0;
    }
    for (i = 0; i < app->transaction_count; i++) {
        const Transaction *t = &app->transactions[i];
        if (fprintf(file, "%d|%s|%d|%d|%ld|%s\n", t->id, t->username,
                    t->package_id, t->quantity, t->total, t->created_at) < 0) {
            fclose(file);
            return 0;
        }
    }
    return fclose(file) == 0;
}

#include "topik2.h"

#include "topik4.h"
#include "topik2.h"
#include "topik7.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int ensure_transaction_capacity(AppState *app) {
    size_t new_capacity;
    Transaction *new_items;

    if (app->transaction_count < app->transaction_capacity) {
        return 1;
    }
    new_capacity = app->transaction_capacity == 0 ? 16 : app->transaction_capacity * 2;
    new_items = (Transaction *)realloc(app->transactions, new_capacity * sizeof(Transaction));
    if (new_items == NULL) {
        return 0;
    }
    app->transactions = new_items;
    app->transaction_capacity = new_capacity;
    return 1;
}

static void set_transaction_time(char *buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    if (local_time == NULL || strftime(buffer, buffer_size, "%Y-%m-%d_%H:%M:%S", local_time) == 0) {
        snprintf(buffer, buffer_size, "WAKTU_TIDAK_DIKETAHUI");
    }
}

void show_packages(const AppState *app) {
    size_t i;
    printf("\n================ DAFTAR PAKET TIKET ================\n");
    printf("ID    | Kategori | Tahap    | Harga          | Kuota      | Stok\n");
    printf("---------------------------------------------------------------\n");
    for (i = 0; i < app->package_count; i++) {
        print_package(&app->packages[i]);
    }
    printf("\nCatatan: harga dan kuota meningkat dari Presale 1 ke Presale 3.\n");
}

int submit_purchase_request(AppState *app) {
    PurchaseRequest request;
    int package_id;
    int quantity;

    show_packages(app);
    package_id = read_int_range("Masukkan ID paket yang ingin dibeli: ", 1, 9999);
    if (find_package_index_linear(app, package_id) < 0) {
        printf("Paket dengan ID %d tidak ditemukan.\n", package_id);
        return 0;
    }
    quantity = read_int_range("Jumlah tiket (1-10): ", 1, 10);

    snprintf(request.username, sizeof(request.username), "%s", app->current_username);
    request.package_id = package_id;
    request.quantity = quantity;

    if (!queue_enqueue(app, &request)) {
        printf("Antrean penuh. Permintaan belum dapat dimasukkan.\n");
        return 0;
    }
    printf("Permintaan pembelian berhasil masuk antrean. Transaksi diproses sesuai urutan FIFO.\n");
    return 1;
}

static int complete_one_purchase(AppState *app, const PurchaseRequest *request) {
    int package_index = find_package_index_linear(app, request->package_id);
    TicketPackage *package;
    Transaction transaction;
    SaleRecord sale_record;
    long old_sold;

    if (package_index < 0) {
        printf("Permintaan akun %s gagal: paket tidak ditemukan.\n", request->username);
        return 0;
    }
    package = &app->packages[package_index];
    if (package->quota - package->sold < request->quantity) {
        printf("Permintaan akun %s gagal: stok paket %d tidak mencukupi. Sisa stok: %d.\n",
               request->username, package->id, package->quota - package->sold);
        return 0;
    }
    if (!ensure_transaction_capacity(app)) {
        printf("Permintaan akun %s gagal: memori transaksi tidak cukup.\n", request->username);
        return 0;
    }

    memset(&transaction, 0, sizeof(transaction));
    transaction.id = app->next_transaction_id;
    snprintf(transaction.username, sizeof(transaction.username), "%s", request->username);
    transaction.package_id = package->id;
    transaction.quantity = request->quantity;
    transaction.total = package->price * request->quantity;
    set_transaction_time(transaction.created_at, sizeof(transaction.created_at));

    old_sold = package->sold;
    package->sold += request->quantity;
    app->transactions[app->transaction_count++] = transaction;

    if (!save_packages(app) || !save_transactions(app)) {
        package->sold = (int)old_sold;
        app->transaction_count--;
        (void)save_packages(app);
        (void)save_transactions(app);
        printf("Transaksi gagal disimpan ke file.\n");
        return 0;
    }

    app->next_transaction_id++;
    app->order_root = tree_insert(app->order_root, &transaction);
    sale_record.transaction_id = transaction.id;
    if (!stack_push(app, &sale_record)) {
        printf("Peringatan: stack riwayat undo penuh; transaksi tetap berhasil.\n");
    }

    printf("Pembelian berhasil diproses: ");
    print_transaction(&transaction);
    return 1;
}

void process_purchase_queue(AppState *app) {
    PurchaseRequest request;
    int processed = 0;

    queue_display(app);
    if (app->queue_count == 0) {
        return;
    }

    printf("\nMemproses antrean dari urutan paling awal...\n");
    while (queue_dequeue(app, &request)) {
        (void)complete_one_purchase(app, &request);
        processed++;
    }
    printf("Pemrosesan antrean selesai. Total permintaan diperiksa: %d.\n", processed);
}

static void remove_transaction_at(AppState *app, size_t index) {
    size_t i;
    for (i = index; i + 1 < app->transaction_count; i++) {
        app->transactions[i] = app->transactions[i + 1];
    }
    if (app->transaction_count > 0) {
        app->transaction_count--;
    }
}

static int restore_transaction(AppState *app, int transaction_id, int require_current_user) {
    int transaction_index = find_transaction_index_linear(app, transaction_id);
    int package_index;
    Transaction transaction;

    if (transaction_index < 0) {
        return 0;
    }
    transaction = app->transactions[transaction_index];
    if (require_current_user && strcmp(transaction.username, app->current_username) != 0) {
        return -1;
    }

    package_index = find_package_index_linear(app, transaction.package_id);
    if (package_index >= 0) {
        app->packages[package_index].sold -= transaction.quantity;
        if (app->packages[package_index].sold < 0) {
            app->packages[package_index].sold = 0;
        }
    }
    remove_transaction_at(app, (size_t)transaction_index);

    if (!save_packages(app) || !save_transactions(app)) {
        /* Kembalikan perubahan di memori apabila salah satu file gagal disimpan. */
        if (package_index >= 0) {
            app->packages[package_index].sold += transaction.quantity;
        }
        app->transactions[app->transaction_count++] = transaction;
        (void)save_packages(app);
        (void)save_transactions(app);
        printf("Pembatalan gagal disimpan. Data transaksi dan kuota dikembalikan.\n");
        return 0;
    }

    app->order_root = tree_delete(app->order_root, transaction_id);
    return 1;
}

static void remove_sale_record_from_stack(AppState *app, int transaction_id) {
    int i;
    int write_index = 0;
    for (i = 0; i <= app->stack_top; i++) {
        if (app->undo_stack[i].transaction_id != transaction_id) {
            app->undo_stack[write_index++] = app->undo_stack[i];
        }
    }
    app->stack_top = write_index - 1;
}

void undo_last_sale(AppState *app) {
    SaleRecord record;
    int result;

    if (!stack_peek(app, &record)) {
        printf("Stack undo kosong. Belum ada transaksi baru yang dapat diurungkan pada sesi ini.\n");
        return;
    }

    result = restore_transaction(app, record.transaction_id, 1);
    if (result == -1) {
        printf("Undo ditolak. Transaksi terakhir pada stack bukan milik akun %s.\n", app->current_username);
        printf("Gunakan menu BST untuk membatalkan transaksi milik akun sendiri.\n");
        return;
    }
    if (result == 0) {
        printf("Undo gagal atau transaksi sudah tidak tersedia.\n");
        return;
    }
    (void)stack_pop(app, &record);
    printf("Undo berhasil. Transaksi %d dihapus dan kuota tiket dikembalikan.\n", record.transaction_id);
}

void show_user_transactions(const AppState *app) {
    size_t i;
    int found = 0;

    printf("\n=== RIWAYAT TRANSAKSI: %s ===\n", app->current_username);
    for (i = 0; i < app->transaction_count; i++) {
        if (strcmp(app->transactions[i].username, app->current_username) == 0) {
            print_transaction(&app->transactions[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("Belum ada transaksi untuk akun ini.\n");
    }
}

int cancel_user_transaction(AppState *app, int transaction_id) {
    int result = restore_transaction(app, transaction_id, 1);
    if (result == 1) {
        remove_sale_record_from_stack(app, transaction_id);
    }
    return result;
}
