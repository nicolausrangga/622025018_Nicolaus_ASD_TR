#include "topik5.h"
#include "topik3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sequential_search_transaction(const AppState *app, int transaction_id) {
    size_t i;
    for (i = 0; i < app->transaction_count; i++) {
        if (app->transactions[i].id == transaction_id) {
            return (int)i;
        }
    }
    return -1;
}

int binary_search_package_id(const TicketPackage *items, size_t count, int package_id) {
    int left = 0;
    int right = (int)count - 1;

    while (left <= right) {
        int middle = left + (right - left) / 2;
        if (items[middle].id == package_id) {
            return middle;
        }
        if (items[middle].id < package_id) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }
    return -1;
}

void searching_menu(const AppState *app) {
    int choice;
    int key;
    int index;
    TicketPackage *sorted_packages;

    printf("\n=== SEARCHING ===\n");
    printf("1. Sequential Search transaksi berdasarkan ID\n");
    printf("2. Binary Search paket berdasarkan ID\n");
    choice = read_int_range("Pilih metode pencarian: ", 1, 2);

    if (choice == 1) {
        key = read_int_range("Masukkan ID transaksi: ", 1, 1000000000);
        index = sequential_search_transaction(app, key);
        if (index >= 0) {
            printf("Transaksi ditemukan:\n");
            print_transaction(&app->transactions[index]);
        } else {
            printf("Transaksi dengan ID %d tidak ditemukan.\n", key);
        }
        return;
    }

    if (app->package_count == 0) {
        printf("Data paket kosong.\n");
        return;
    }
    sorted_packages = (TicketPackage *)malloc(app->package_count * sizeof(TicketPackage));
    if (sorted_packages == NULL) {
        printf("Memori tidak cukup untuk pencarian.\n");
        return;
    }
    memcpy(sorted_packages, app->packages, app->package_count * sizeof(TicketPackage));
    /* Binary Search membutuhkan data terurut; paket diurutkan berdasarkan ID terlebih dahulu. */
    bubble_sort_packages(sorted_packages, app->package_count, 3, 0);

    key = read_int_range("Masukkan ID paket: ", 1, 9999);
    index = binary_search_package_id(sorted_packages, app->package_count, key);
    if (index >= 0) {
        printf("Paket ditemukan:\n");
        print_package(&sorted_packages[index]);
    } else {
        printf("Paket dengan ID %d tidak ditemukan.\n", key);
    }
    free(sorted_packages);
}
