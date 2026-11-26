#include "topik6.h"
#include "topik1.h"
#include "topik8.h"
#include "topik5.h"
#include "topik3.h"
#include "topik2.h"
#include "topik2.h"
#include "topik7.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void show_start_menu(void) {
    printf("\n=====================================================\n");
    printf("     SISTEM PENJUALAN TIKET KONSER\n");
    printf("     Manajemen Harga dan Kuota Presale\n");
    printf("=====================================================\n");
    printf("1. Register akun\n");
    printf("2. Login\n");
    printf("0. Keluar\n");
}

static void show_user_menu(const char *username) {
    printf("\n=====================================================\n");
    printf(" MENU UTAMA | Login sebagai: %s\n", username);
    printf("=====================================================\n");
    printf("1. Lihat paket tiket\n");
    printf("2. Masukkan pembelian ke antrean\n");
    printf("3. Proses antrean pembelian (FIFO)\n");
    printf("4. Undo transaksi terakhir (Stack)\n");
    printf("5. Searching data\n");
    printf("6. Sorting paket tiket\n");
    printf("7. Riwayat transaksi akun\n");
    printf("8. Kelola transaksi dengan BST\n");
    printf("9. Jelajahi graph relasi presale\n");
    printf("0. Logout\n");
}

static void user_menu(AppState *app) {
    int choice;
    int logged_in = 1;

    while (logged_in) {
        show_user_menu(app->current_username);
        choice = read_int_range("Pilih menu: ", 0, 9);
        switch (choice) {
            case 1:
                show_packages(app);
                break;
            case 2:
                (void)submit_purchase_request(app);
                break;
            case 3:
                process_purchase_queue(app);
                break;
            case 4:
                undo_last_sale(app);
                break;
            case 5:
                searching_menu(app);
                break;
            case 6:
                sorting_menu(app);
                break;
            case 7:
                show_user_transactions(app);
                break;
            case 8:
                tree_menu(app);
                break;
            case 9:
                graph_menu(app);
                break;
            case 0:
                printf("Logout berhasil. Sampai jumpa, %s.\n", app->current_username);
                app->current_username[0] = '\0';
                logged_in = 0;
                break;
            default:
                break;
        }
    }
}

static void cleanup_app(AppState *app) {
    tree_clear(app->order_root);
    auth_free_users(app);
    free(app->packages);
    free(app->transactions);
    app->packages = NULL;
    app->transactions = NULL;
}

int main(void) {
    AppState app = {0};
    int choice;
    int running = 1;

    app.stack_top = -1;
    app.queue_front = 0;
    app.queue_rear = 0;
    app.next_transaction_id = 1;

    if (!ensure_data_files()) {
        return EXIT_FAILURE;
    }
    if (!load_packages(&app)) {
        fprintf(stderr, "Gagal memuat data paket tiket.\n");
        cleanup_app(&app);
        return EXIT_FAILURE;
    }
    if (!load_transactions(&app)) {
        fprintf(stderr, "Gagal memuat data transaksi.\n");
        cleanup_app(&app);
        return EXIT_FAILURE;
    }
    auth_load_users(&app);
    tree_rebuild(&app);

    while (running) {
        show_start_menu();
        choice = read_int_range("Pilih menu: ", 0, 2);
        switch (choice) {
            case 1:
                (void)auth_register(&app);
                break;
            case 2:
                if (auth_login(&app)) {
                    user_menu(&app);
                }
                break;
            case 0:
                running = 0;
                break;
            default:
                break;
        }
    }

    cleanup_app(&app);
    printf("Program selesai. Terima kasih telah menggunakan sistem.\n");
    return EXIT_SUCCESS;
}
